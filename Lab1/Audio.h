#pragma once

#include <glm\glm.hpp>
#include <AL\al.h>
#include <AL\alc.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <SDL\SDL.h>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

class Audio
{
	struct data {
		int sourceID, bufferID;
		char* buffer;
		string name;
		data(unsigned int sI, unsigned int bI, char* b, const char* n)
		{
			this->sourceID = sI;
			this->bufferID = bI;
			this->buffer = b;
			this->name = n;
		}

	};

	struct Vector3
	{
		float x, y, z;
	};
	vector<data> datas;
	ALCcontext* context;
	ALCdevice *device;
	bool isBigEndian();
	int convertToInt(char* buffer, int length);
	char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);

public:
	Audio();
	~Audio();
	unsigned int loadSound(const char* filename);
	void deleteSound(unsigned int id);
	void playSound(unsigned int id);
	void playSound(unsigned int id, vec3& pos);
	void stopSound(unsigned int id);
	void setlistener(vec3& pos, vec3& camLookAt);

private:
	
};

