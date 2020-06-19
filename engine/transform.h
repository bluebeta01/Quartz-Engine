#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/quaternion.hpp>

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
		
		glm::quat rotationQuat = glm::quat(glm::vec3(glm::radians(transform.rotation.x), glm::radians(transform.rotation.y), glm::radians(transform.rotation.z)));
		glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);

		return positionMatrix * rotationMatrix * scaleMatrix;
	}
};