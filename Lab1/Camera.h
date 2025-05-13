#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

struct Camera
{
public:
	Camera()
	{
	}

	void initCamera(const vec3& pos, float fov, float aspect, float nearClip, float farClip)
	{
		this->pos = pos;
		this->aspect = aspect;
		this->nearClip = nearClip;
		this->farClip = farClip;
		this->forward = vec3(0.0f, 0.0f, 1.0f);
		this->up = vec3(0.0f, 1.0f, 0.0f);
		this->projection = perspective(fov, aspect, nearClip, farClip);
	}

	vec3 getPos()
	{
		return this->pos;
	}

	inline mat4 getViewProjection() const
	{
		return projection * lookAt(pos, pos + forward, up);
	}

	inline mat4 getProjection() const
	{
		return projection;
	}

	inline mat4 getView() const
	{
		return lookAt(pos, pos + forward, up);
	}

	void setLook(vec3 modelPos)
	{
		forward = vec3(normalize(modelPos - pos));
	}

	void setPos(vec3 pos)
	{
		this->pos = pos;
	}

	void setFOV(float fov) 
	{
		this->fov = fov;
		this->projection = perspective(fov, aspect, nearClip, farClip);
	}

	//void MoveForward(float amt)
	//{
	//	pos += forward * amt;
	//}

	//void MoveRight(float amt)
	//{
	//	pos += cross(up, forward) * amt;
	//}

	//void Pitch(float angle)
	//{
	//	vec3 right = normalize(cross(up, forward));

	//	forward = vec3(normalize(rotate(angle, right) * vec4(forward, 0.0)));
	//	up = normalize(cross(forward, right));
	//}

	//void RotateY(float angle)
	//{
	//	static const vec3 UP(0.0f, 1.0f, 0.0f);

	//	mat4 rotation = rotate(angle, UP);

	//	forward = vec3(normalize(rotation * vec4(forward, 0.0)));
	//	up = vec3(normalize(rotation * vec4(up, 0.0)));
	//}

protected:
private:
	mat4 projection;
	vec3 pos;
	vec3 forward;
	vec3 up;
	float fov;
	float aspect;
	float nearClip;
	float farClip;
};


