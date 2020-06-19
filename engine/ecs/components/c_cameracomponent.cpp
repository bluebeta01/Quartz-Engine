#include "pch.h"
#include "c_cameracomponent.h"
#include "ecs/entity/c_entity.h"
#include "c_gamewindow.h"

CameraComponent::CameraComponent() : Component(COMPONENT_TYPE_CAMERA_COMPONENT)
{
	m_camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::radians(40.0f), 0.01f, 1000.0f, (float)GameWindow::s_clientSize.x / (float)GameWindow::s_clientSize.y);
}

void CameraComponent::update()
{
	m_camera.transform = m_entity->transform;
}