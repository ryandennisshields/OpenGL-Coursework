#pragma once
#include <string>
#include <GL\glew.h>

using namespace std;

class Texture
{
public:
	Texture();
	
	void load(const string& fileName);
	void Bind(unsigned int unit); // bind upto 32 textures
	GLuint getID() { return textureHandler; }
	~Texture();

protected:
private:

	GLuint textureHandler;
};

