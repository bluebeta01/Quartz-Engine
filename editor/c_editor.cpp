#include "c_editor.h"

Entity* plane;

Editor::Editor() : m_engine(NULL, 1280, 720)
{
	m_camera = m_engine.m_world.createEntity("Camera");
	m_camera->addComponent(new CameraComponent());

	dynamic_cast<CameraComponent*>(m_camera->getComponent(Component::COMPONENT_TYPE_CAMERA_COMPONENT))->m_camera
		= Camera(glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::radians(40.0f), 0.1f, 1000.0f,
		(float)m_engine.m_renderer.m_width / (float)m_engine.m_renderer.m_height);

	m_gizmo.initialize(&m_engine.m_renderer, &dynamic_cast<CameraComponent*>(m_camera->getComponent(Component::COMPONENT_TYPE_CAMERA_COMPONENT))->m_camera, &m_engine.m_world);

	//This model should remain loaded at ALL TIMES
	m_engine.m_renderer.getModel("debugplane");


	plane = m_engine.m_world.createEntity("plane", Transform(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0, 180, 0), glm::vec3(1.0, 1.0, 1.0)));
	plane->addComponent(new RenderComponent(m_engine.m_renderer.getModel("debugplane")));
	Entity* ball = m_engine.m_world.createEntity("ball", Transform(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(1.0, 1.0, 1.0)));
	ball->addComponent(new RenderComponent(m_engine.m_renderer.getModel("woodball")));
	plane->addChild(ball);

}

void Editor::tick()
{
	m_engine.update();
	cameraMovement();
	m_gizmo.update();

	plane->transform.rotation.x += 0.3f;
}

void Editor::render()
{
	m_engine.m_renderer.setCamera(dynamic_cast<CameraComponent*>(m_camera->getComponent(Component::COMPONENT_TYPE_CAMERA_COMPONENT))->m_camera);
	m_engine.m_renderer.render();
	m_engine.m_renderer.present();
}

void Editor::terminate()
{
	m_engine.terminate();
}

void Editor::cameraMovement()
{
	float cameraSpeed = 0.3f;
	float cameraSensitivity = 0.1f;

	if (input::isMouseButtonDown(input::MOUSEBUTTON_RIGHT))
	{
		input::lockMouse = true;
		m_camera->transform.rotation.x -= input::mouseDelta.y * cameraSensitivity;
		m_camera->transform.rotation.y -= input::mouseDelta.x * cameraSensitivity;

		glm::vec3 forward = dynamic_cast<CameraComponent*>(m_camera->getComponent(Component::COMPONENT_TYPE_CAMERA_COMPONENT))->m_camera.getForward();
		glm::vec3 right = dynamic_cast<CameraComponent*>(m_camera->getComponent(Component::COMPONENT_TYPE_CAMERA_COMPONENT))->m_camera.getRight();

		if (input::isKeyDown('W'))
		{
			m_camera->transform.position += forward * cameraSpeed;
		}
		if (input::isKeyDown('S'))
		{
			m_camera->transform.position -= forward * cameraSpeed;
		}
		if (input::isKeyDown('A'))
		{
			m_camera->transform.position += right * cameraSpeed;
		}
		if (input::isKeyDown('D'))
		{
			m_camera->transform.position -= right * cameraSpeed;
		}
	}
	else
	{
		input::lockMouse = false;
	}
}