#include "pch.h"
#include "c_cameracomponent.h"
#include "screen/screen.h"
#include "ecs/entity/c_entity.h"

CameraComponent::CameraComponent() : Component(COMPONENT_TYPE_CAMERA_COMPONENT)
{
	m_camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::radians(40.0f), 0.01f, 1000.0f, (float)screen::clientAreaSize.x / (float)screen::clientAreaSize.y);
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::update()
{
	m_camera.transform = m_entity->transform;
}