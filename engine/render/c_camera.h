#pragma once
#include "pch.h"

class Camera
{
public:
	Camera(glm::vec3 location, glm::vec3 rotation, float fov, float nearPlane, float farPlane, float aspect);
	Camera() {};
	~Camera() {};
	glm::mat4 getViewMatrix();
	glm::vec3 getForward();
	glm::vec3 getRight();
	glm::mat4 getPerspectiveMatrix();
	Transform transform;
	float fov;
	float nearPlane;
	float farPlane;
	float aspect;
};