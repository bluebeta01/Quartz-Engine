#include "c_camera.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 location, glm::vec3 rotation, float fov, float nearPlane, float farPlane, float aspect)
{
	this->transform.position = location;
	this->transform.rotation = rotation;
	this->fov = fov;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->aspect = aspect;
}

Camera::~Camera()
{

}

glm::mat4 Camera::getViewMatrix()
{
	glm::vec3 camFront;
	camFront.x = cos(glm::radians(transform.rotation.x)) * cos(glm::radians(transform.rotation.y));
	camFront.y = sin(glm::radians(transform.rotation.x));
	camFront.z = cos(glm::radians(transform.rotation.x)) * sin(glm::radians(transform.rotation.y));
	camFront = glm::normalize(camFront);

	return glm::lookAt(transform.position, transform.position - camFront, glm::vec3(0, 1, 0));
}

glm::vec3 Camera::getForward()
{
	glm::vec3 camFront;
	camFront.x = cos(glm::radians(transform.rotation.x)) * cos(glm::radians(transform.rotation.y));
	camFront.y = sin(glm::radians(transform.rotation.x));
	camFront.z = cos(glm::radians(transform.rotation.x)) * sin(glm::radians(transform.rotation.y));
	camFront = glm::normalize(camFront);
	return camFront;
}