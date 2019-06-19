#pragma once
#include <glm.hpp>

struct Transform
{
	Transform()
	{
		glm::vec3 s;
		s.x = 1.0f;
		s.y = 1.0f;
		s.z = 1.0f;
		scale = s;
	}
	Transform(glm::vec3 position)
	{
		this->position = position;
		this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	Transform(glm::vec3 position, glm::vec3 rotation)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};