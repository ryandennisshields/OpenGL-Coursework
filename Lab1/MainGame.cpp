#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>

Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Audio* audioDevice();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	whistle = audioDevice.loadSound("..\\res\\bang.wav");
	backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	mesh1.loadModel("..\\res\\cube.obj");
	mesh2.loadModel("..\\res\\sphere.obj");
	mesh3.loadModel("..\\res\\cone.obj");
	mesh4.loadModel("..\\res\\ground.obj");
	mesh1Pos = vec3(0.0f, 5.0f, 0.0f);
	mesh2Pos = vec3(-1.5f, 5.0f, 0.0f);
	mesh3Pos = vec3(1.5f, 5.0f, 0.0f);
	mass1 = 1.0f;
	mass2 = 2.0f;
	mass3 = 3.0f;
	jumpAllowed = false;
	jumpVelo = 0.001f;
	upwardVelo = 0.0f;
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\Rim.vert", "..\\res\\Rim.frag");

	textures.init("..\\res\\textures.vert", "..\\res\\textures.frag");	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	texture1.load("..\\res\\rock.jpg"); //load texture
	normals1.load("..\\res\\rocknormal.jpg");
	displacement1.load("..\\res\\rockdisp.jpg");

	texture2.load("..\\res\\metal.jpg"); //load texture
	normals2.load("..\\res\\metalnormal.jpg");
	displacement2.load("..\\res\\metaldisp.jpg");
	metal1.load("..\\res\\metalshine.jpg");

	texture3.load("..\\res\\marble.jpg"); //load texture
	normals3.load("..\\res\\marblenormal.jpg");
	displacement3.load("..\\res\\marbledisp.jpg");

	myCamera.initCamera(vec3(0, 0, 0), 40.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius(), mesh3.getSpherePos(), mesh3.getSphereRadius());
		//playAudio(backGroundMusic, vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
				case SDLK_LSHIFT:
					myCamera.setFOV(70.0f);
					break;
				case SDLK_w:
					mesh1Pos.z += 0.1f;
					mesh2Pos.z += 0.1f;
					mesh3Pos.z += 0.1f;
					break;
				case SDLK_s:
					mesh1Pos.z -= 0.1f;
					mesh2Pos.z -= 0.1f;
					mesh3Pos.z -= 0.1f;
					break;
				case SDLK_d:
					mesh1Pos.x -= 0.1f;
					mesh2Pos.x -= 0.1f;
					mesh3Pos.x -= 0.1f;
					break;
				case SDLK_a:
					mesh1Pos.x += 0.1f;
					mesh2Pos.x += 0.1f;
					mesh3Pos.x += 0.1f;
					break;
				case SDLK_SPACE:
					if (jumpAllowed) 
					{
						upwardVelo = jumpVelo;
						jumpAllowed = false;
					}
					break;
				}
				break;
			case SDL_KEYUP:
				switch (evnt.key.keysym.sym)
				{
				case SDLK_LSHIFT:
					myCamera.setFOV(40.0f);
					break;
				}
				break;
		}
	}
}

bool MainGame::collision(vec3 m1Pos, float m1Rad, vec3 m2Pos, float m2Rad, vec3 m3Pos, float m3Rad)
{
	float distance1 = glm::distance(m1Pos, m2Pos) - (m1Rad + m2Rad);
	float distance2 = glm::distance(m1Pos, m3Pos) - (m1Rad + m3Rad);
	float distance3 = glm::distance(m2Pos, m3Pos) - (m2Rad + m3Rad);
	
	if (distance1 <= 0 || distance2 <= 0 || distance3 <= 0)
	{
		audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::playAudio(unsigned int Source, vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL/	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
		audioDevice.playSound(Source, pos);
}
void MainGame::linkRimShader()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	rimShader.setMat4("modelMatrix", transform.GetModel());
	//rimShader.setMat4("viewMatrix", myCamera.getView());
	rimShader.setFloat("rimPower", 3.0f);
	rimShader.setVec3("rimColor", vec3(0.8f, 0.0f, 0.0f));
	rimShader.setVec3("camPos", myCamera.getPos());
}

void MainGame::linkFogShader()
{
	//fogShader.setMat4("u_pm", myCamera.getProjection());
	//fogShader.setMat4("u_vm", myCamera.getProjection());
	fogShader.setFloat("maxDist", 20.0f);
	fogShader.setFloat("minDist", 0.0f);
	fogShader.setVec3("fogColor", vec3(0.0f, 0.0f, 0.0f));
}

void MainGame::linkToon()
{
	toonShader.setMat4("modelMatrix", transform.GetModel());
	toonShader.setVec3("lightDir", vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkMapping(int textureID)
{
	if (textureID == 0)
	{
		GLuint t1L = glGetUniformLocation(textures.getID(), "diffuseT");
		GLuint t2L = glGetUniformLocation(textures.getID(), "normalT");
		GLuint t3L = glGetUniformLocation(textures.getID(), "displacementT");
		GLuint dispScale = glGetUniformLocation(textures.getID(), "displacementScale");

		//set textures
		glActiveTexture(GL_TEXTURE0); //set active texture unit
		glBindTexture(GL_TEXTURE_2D, texture1.getID());
		glUniform1i(t1L, 0);

		glActiveTexture(GL_TEXTURE1); //set active texture unit
		glBindTexture(GL_TEXTURE_2D, normals1.getID());
		glUniform1i(t2L, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, displacement1.getID());
		glUniform1i(t3L, 2);

		glUniform1f(dispScale, 0.1f);
		GLuint opacityEdit = glGetUniformLocation(textures.getID(), "opacity");
		glUniform1f(opacityEdit, 1.0f);
	}
	if (textureID == 1) 
	{
		GLuint t1L = glGetUniformLocation(textures.getID(), "diffuseT");
		GLuint t2L = glGetUniformLocation(textures.getID(), "normalT");
		GLuint t3L = glGetUniformLocation(textures.getID(), "metallicT");
		GLuint t4L = glGetUniformLocation(textures.getID(), "displacementT");
		GLuint dispScale = glGetUniformLocation(textures.getID(), "displacementScale");

		//set textures
		glActiveTexture(GL_TEXTURE3); //set acitve texture unit
		glBindTexture(GL_TEXTURE_2D, texture2.getID());
		glUniform1i(t1L, 3);

		glActiveTexture(GL_TEXTURE4); //set acitve texture unit
		glBindTexture(GL_TEXTURE_2D, normals2.getID());
		glUniform1i(t2L, 4);

		glActiveTexture(GL_TEXTURE5); //set acitve texture unit
		glBindTexture(GL_TEXTURE_2D, metal1.getID());
		glUniform1i(t3L, 5);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, displacement2.getID());
		glUniform1i(t4L, 6);

		glUniform1f(dispScale, 0.1f);
		GLuint opacityEdit = glGetUniformLocation(textures.getID(), "opacity");
		glUniform1f(opacityEdit, 1.0f);
	}
	if (textureID == 2)
	{
		GLuint t1L = glGetUniformLocation(textures.getID(), "diffuseT");
		GLuint t2L = glGetUniformLocation(textures.getID(), "normalT");
		GLuint t3L = glGetUniformLocation(textures.getID(), "displacementT");
		GLuint dispScale = glGetUniformLocation(textures.getID(), "displacementScale");

		//set textures
		glActiveTexture(GL_TEXTURE7); //set active texture unit
		glBindTexture(GL_TEXTURE_2D, texture3.getID());
		glUniform1i(t1L, 7);

		glActiveTexture(GL_TEXTURE8); //set active texture unit
		glBindTexture(GL_TEXTURE_2D, normals3.getID());
		glUniform1i(t2L, 8);

		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, displacement3.getID());
		glUniform1i(t3L, 9);

		glUniform1f(dispScale, 0.1f);
		GLuint opacityEdit = glGetUniformLocation(textures.getID(), "opacity");
		glUniform1f(opacityEdit, 0.5f);
	}
}

void MainGame::SetTexture(int textureID)
{
	textures.Bind();
	if (textureID == 0)
		linkMapping(0);
	if (textureID == 1)
		linkMapping(1);
	if (textureID == 2)
		linkMapping(2);
	textures.Update(transform, myCamera);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour

	float gravity = 0.00000918f;

	float weight1 = mass1 * gravity;
	mesh1Pos.y -= (weight1 - upwardVelo);
	mesh1Pos.y = std::max(mesh1Pos.y, -1.8f);
	if (mesh1Pos.y <= -1.8f)
	{
		jumpAllowed = true;
		mass1 = 1.0f;
		upwardVelo = 0.0f;
	}
	else if (mass1 != 10.0f)
		mass1 += 0.01f;

	float weight2 = mass2 * gravity;
	mesh2Pos.y -= weight2;
	mesh2Pos.y = std::max(mesh2Pos.y, -1.8f);
	if (mesh2Pos.y <= -1.8f)
		mass2 = 2.0f;
	else if (mass2 != 10.0f)
		mass2 += 0.02f;

	float weight3 = mass3 * gravity;
	mesh3Pos.y -= weight3;
	mesh3Pos.y = std::max(mesh3Pos.y, -1.8f);
	if (mesh3Pos.y <= -1.8f)
		mass3 = 3.0f;
	else if (mass3 != 10.0f)
		mass3 += 0.03f;

	myCamera.setPos(vec3(mesh1Pos.x, mesh1Pos.y, mesh1Pos.z - 4));

	transform.SetPos(mesh1Pos);
	transform.SetRot(vec3(0.0, 0.0, 0.0));
	transform.SetScale(vec3(0.6, 0.6, 0.6));
	SetTexture(0);
	mesh1.draw();
	mesh1.updateSphereData(*transform.GetPos(), 0.65f);

	transform.SetPos(mesh2Pos);
	transform.SetRot(vec3(0.0, 0.0, 0.0));
	transform.SetScale(vec3(0.6, 0.6, 0.6));
	SetTexture(1);
	mesh2.draw();
	mesh2.updateSphereData(*transform.GetPos(), 0.65f);

	transform.SetPos(mesh3Pos);
	transform.SetRot(vec3(0.0, 0.0, 0.0));
	transform.SetScale(vec3(0.6, 0.6, 0.6));
	SetTexture(2);
	mesh3.draw();
	mesh3.updateSphereData(*transform.GetPos(), 0.65f);

	transform.SetPos(vec3(0.0, -3, 0.0));
	transform.SetRot(vec3(0.0, 0.0, 0.0));
	transform.SetScale(vec3(0.6, 0.6, 0.6));
	SetTexture(1);
	mesh4.draw();
	mesh4.updateBoxData(*transform.GetPos(), vec3(3.0, 1.0, 3.0));

	counter = counter + 0.0001f;

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();
}