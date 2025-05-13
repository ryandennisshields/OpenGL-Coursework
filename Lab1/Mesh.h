#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "obj_loader.h"

struct Vertex
{
public:
	Vertex(const vec3& pos, const vec2& texCoord, const vec3& normal)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
		this->tangent = vec3(0.0f);
		this->bitangent = vec3(0.0f);
	}

	vec3* GetPos() { return &pos; }
	vec2* GetTexCoord() { return &texCoord; }
	vec3* GetNormal() { return &normal; }

	vec3 pos;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
};

struct Sphere
{
public:

	Sphere() {}

	Sphere(vec3& pos, float radius)
	{
		this->pos = pos;
	}

	vec3 GetPos() { return pos; }
	float GetRadius() { return radius; }

	void SetPos(vec3 pos)
	{
		this->pos = pos;
	}

	void SetRadius(float radius)
	{
		this->radius = radius;
	}

private:
	vec3 pos;
	float radius;
};

struct Box
{
public:

	Box() {}

	Box(vec3& pos, vec3& size)
	{
		this->pos = pos;
	}

	vec3 GetPos() { return pos; }
	vec3 GetSize() { return size; }

	void SetPos(vec3 pos)
	{
		this->pos = pos;
	}

	void SetSize(vec3& size)
	{
		this->size = size;
	}

private:
	vec3 pos;
	vec3 size;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void drawVertexes();
	void loadVertexs(Vertex* vertices, unsigned int numVertices);
	void draw();
	void init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void loadModel(const string& filename);
	void initModel(IndexedModel& model);
	void calculateTangentsAndBitangents(vector<vec3>& positions, vector<vec2>& texCoords, vector<vec3>& normals, vector<vec3>& tangents, vector<vec3>& bitangents, const vector<unsigned int>& indices);
	void updateSphereData(vec3 pos, float radius);
	void updateBoxData(vec3 pos, vec3 size);
	vec3 getSpherePos() { return meshSphere.GetPos(); }
	float getSphereRadius() { return meshSphere.GetRadius(); }

private:
	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		TANGENT_VB,
		BITANGENT_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	Sphere meshSphere;
	Box meshBox;
	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS]; // create our array of buffers
	unsigned int drawCount; //how much of the vertexArrayObject do we want to draw
};