#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

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


	static glm::mat4 matrixFromTransform(Transform transform, bool correctRotation)
	{
		glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), transform.position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);

		glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.x - 90.0f), glm::vec3(1, 0, 0));
		glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
		glm::mat4 rotationMatrix = rotationMatrixZ * rotationMatrixY * rotationMatrixX;

		return positionMatrix * rotationMatrix * scaleMatrix;
	}
};