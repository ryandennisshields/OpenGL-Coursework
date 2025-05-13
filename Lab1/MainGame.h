#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <conio.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"

using namespace glm;

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void linkMapping(int textureID);
	void linkFogShader();
	void linkToon();
	void linkRimShader();
	void SetTexture(int textureID);
	bool collision(vec3 m1Pos, float m1Rad, vec3 m2Pos, float m2Rad, vec3 m3Pos, float m3Rad);
	void playAudio(unsigned int Source, vec3 pos);

	Display _gameDisplay;
	GameState _gameState;

	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Mesh mesh4;
	vec3 mesh1Pos;
	vec3 mesh2Pos;
	vec3 mesh3Pos;

	float mass1;
	float mass2;
	float mass3;
	bool jumpAllowed;
	float jumpVelo;
	float upwardVelo;

	Camera myCamera;
	Shader fogShader;
	Shader toonShader;
	Shader rimShader;
	Shader textures;

	Texture texture1;
	Texture texture2;
	Texture texture3;
	Texture normals1;
	Texture normals2;
	Texture normals3;
	Texture displacement1;
	Texture displacement2;
	Texture displacement3;
	Texture metal1;
	
	int currentTextureID;
	
	Audio audioDevice;

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;
};

