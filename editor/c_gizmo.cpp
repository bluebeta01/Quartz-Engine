#include "c_gizmo.h"

//extern Renderer* renderer;
//extern Camera camera;
//extern Framebuffer* frameBuffer;
//extern World* world;

Gizmo::Gizmo()
{
	
}

void Gizmo::initialize(Renderer* renderer, Camera* camera, World* world)
{
	this->renderer = renderer;
	this->camera = camera;
	this->world = world;

	frameBuffer = renderer->createFramebuffer(GameWindow::s_renderAreaSize.x, GameWindow::s_renderAreaSize.y);
	createGizmo();
}

void Gizmo::update()
{
	if (GameWindow::s_resizeEvent)
		frameBuffer->rebuildFramebuffer(GameWindow::s_clientSize.x, GameWindow::s_clientSize.y);

	selectEntity();
	setupGizmo();
	gizmoSelect();
	gizmoDrag();

	if (m_selectedEntity)
		m_gizmo->setWorldPosition(m_selectedEntity->getWorldPosition());
}

void Gizmo::createGizmo()
{
	m_gizmo = world->createEntity("Gizmo");
	m_transformGizmo = world->createEntity("Transformation Gizmo");
	m_gizmo->addChild(m_transformGizmo);


	m_transformGizmoXArrow = world->createEntity("gizmoXArrow", Transform(glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::vec3(0.01, 0.01, 0.01)));
	m_transformGizmo->addChild(m_transformGizmoXArrow);
	RenderComponent* rc = dynamic_cast<RenderComponent*>(m_transformGizmoXArrow->addComponent(new RenderComponent(renderer->getModel("xarrow"))));
	rc->m_onTop = true;

	m_transformGizmoYArrow = world->createEntity("gizmoYArrow", Transform(glm::vec3(0, 0, 0), glm::vec3(-90, 0, 0), glm::vec3(0.01, 0.01, 0.01)));
	m_transformGizmo->addChild(m_transformGizmoYArrow);
	rc = dynamic_cast<RenderComponent*>(m_transformGizmoYArrow->addComponent(new RenderComponent(renderer->getModel("yarrow"))));
	rc->m_onTop = true;

	m_transformGizmoZArrow = world->createEntity("gizmoZArrow", Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.01, 0.01, 0.01)));
	m_transformGizmo->addChild(m_transformGizmoZArrow);
	rc = dynamic_cast<RenderComponent*>(m_transformGizmoZArrow->addComponent(new RenderComponent(renderer->getModel("zarrow"))));
	rc->m_onTop = true;
}

void Gizmo::selectEntity()
{
	if (input::isMouseButtonClicked(input::MOUSEBUTTON_LEFT))
	{
		Entity* e = renderer->colorPick(GameWindow::s_cursorPosInRenderArea);
		if (e) {
			if (e->m_uid == m_transformGizmoXArrow->m_uid || e->m_uid == m_transformGizmoYArrow->m_uid || e->m_uid == m_transformGizmoZArrow->m_uid)
				return;
			m_selectedEntity = e;
			if (m_selectedEntity)
			{
				/*m_transformGizmoXArrow->transform.position = m_selectedEntity->getWorldPosition();
				m_transformGizmoYArrow->transform.position = m_selectedEntity->getWorldPosition();
				m_transformGizmoZArrow->transform.position = m_selectedEntity->getWorldPosition();*/
				m_gizmo->setWorldPosition(m_selectedEntity->getWorldPosition());
			}
		}
	}
}

void Gizmo::setupGizmo()
{
	if (!m_selectedEntity)
		return;
	float gizmoSize = 0.004f;
	float distance;

	distance = glm::distance(m_gizmo->getWorldPosition(), camera->transform.position);
	m_transformGizmoXArrow->transform.scale = glm::vec3(distance * gizmoSize, distance * gizmoSize, distance * gizmoSize);
	m_transformGizmoYArrow->transform.scale = glm::vec3(distance * gizmoSize, distance * gizmoSize, distance * gizmoSize);
	m_transformGizmoZArrow->transform.scale = glm::vec3(distance * gizmoSize, distance * gizmoSize, distance * gizmoSize);
}

void Gizmo::gizmoSelect()
{
	if (input::isMouseButtonClicked(input::MOUSEBUTTON_LEFT))
	{
		Entity* e = renderer->colorPick(GameWindow::s_cursorPosInRenderArea);

		if (e)
		{
			if (e->m_uid == m_transformGizmoXArrow->m_uid)
			{
				renderer->clearFramebuffer(frameBuffer);
				renderer->setCustomViewport(GameWindow::s_renderAreaSize.x, GameWindow::s_renderAreaSize.y);

				Entity* debugPlane = world->createEntity("debugplane", Transform(m_gizmo->getWorldPosition(), glm::vec3(-90, 0, 0), glm::vec3(1000, 1000, 1000)));
				debugPlane->addComponent(new RenderComponent(renderer->getModel("debugplane")));
				/*RenderComponent* rc = new RenderComponent();
				rc->setModel(renderer->getModel("debugplane"));
				debugPlane->addComponent(rc);*/

				float d = debugPlane->transform.position.z - camera->transform.position.z;
				if (signbit(d))
					debugPlane->transform.rotation.x = camera->transform.rotation.x;
				else
					debugPlane->transform.rotation.x = -camera->transform.rotation.x;

				renderer->renderModel(((RenderComponent*)debugPlane->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT))->getModel(),
					Transform::matrixFromTransform(debugPlane->transform, true), camera->getViewMatrix(), camera->getPerspectiveMatrix(),
					nullptr, frameBuffer, true);

				world->destoryEntity(debugPlane);

				m_selectedGizmo = m_transformGizmoXArrow;

				m_gizmoSelectLocation = renderer->screenToWorldPosition(GameWindow::s_cursorPosInRenderArea, frameBuffer) - m_gizmo->getWorldPosition();
				
				renderer->resetViewport();
			}

			if (e->m_uid == m_transformGizmoYArrow->m_uid)
			{
				renderer->clearFramebuffer(frameBuffer);
				renderer->setCustomViewport(GameWindow::s_renderAreaSize.x, GameWindow::s_renderAreaSize.y);

				Entity* debugPlane = world->createEntity("debugplane", Transform(m_gizmo->getWorldPosition(), glm::vec3(0, 0, 0), glm::vec3(1000, 1000, 1000)));
				debugPlane->addComponent(new RenderComponent(renderer->getModel("debugplane")));

				debugPlane->transform.rotation.y = -camera->transform.rotation.y - 90;

				renderer->renderModel(((RenderComponent*)debugPlane->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT))->getModel(),
					Transform::matrixFromTransform(debugPlane->transform, true), camera->getViewMatrix(), camera->getPerspectiveMatrix(),
					nullptr, frameBuffer, true);

				world->destoryEntity(debugPlane);

				m_selectedGizmo = m_transformGizmoYArrow;

				m_gizmoSelectLocation = renderer->screenToWorldPosition(GameWindow::s_cursorPosInRenderArea, frameBuffer) - m_gizmo->getWorldPosition();
				renderer->resetViewport();
			}

			if (e->m_uid == m_transformGizmoZArrow->m_uid)
			{
				renderer->clearFramebuffer(frameBuffer);
				renderer->setCustomViewport(GameWindow::s_renderAreaSize.x, GameWindow::s_renderAreaSize.y);

				Entity* debugPlane = world->createEntity("debugplane", Transform(m_gizmo->getWorldPosition(), glm::vec3(-90, 0, 0), glm::vec3(1000, 1000, 1000)));
				debugPlane->addComponent(new RenderComponent(renderer->getModel("debugplane")));

				float d = debugPlane->transform.position.x - camera->transform.position.x;
				if (signbit(d))
					debugPlane->transform.rotation.z = -camera->transform.rotation.x + 90;
				else
					debugPlane->transform.rotation.z = camera->transform.rotation.x + 90;

				renderer->renderModel(((RenderComponent*)debugPlane->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT))->getModel(),
					Transform::matrixFromTransform(debugPlane->transform, true), camera->getViewMatrix(), camera->getPerspectiveMatrix(),
					nullptr, frameBuffer, true);

				world->destoryEntity(debugPlane);

				m_selectedGizmo = m_transformGizmoZArrow;

				m_gizmoSelectLocation = renderer->screenToWorldPosition(GameWindow::s_cursorPosInRenderArea, frameBuffer) - m_gizmo->getWorldPosition();
				renderer->resetViewport();
			}

		}
		else
		{
			m_selectedGizmo = nullptr;
			m_selectedEntity = nullptr;
			m_gizmo->setWorldPosition(glm::vec3(0, -10000000, 0));
		}

	}
}

void Gizmo::gizmoDrag()
{
	if (input::isMouseButtonDown(input::MOUSEBUTTON_LEFT) && m_selectedEntity && m_selectedGizmo)
	{
		if (m_selectedGizmo == m_transformGizmoXArrow)
		{
			glm::vec3 newDepthPos = renderer->screenToWorldPosition(input::cursorPosition, frameBuffer);
			float newX = newDepthPos.x - m_gizmoSelectLocation.x;
			m_gizmo->setWorldPosition(glm::vec3(newX, m_transformGizmoYArrow->getWorldPosition().y, m_transformGizmoYArrow->getWorldPosition().z));
			glm::vec3 entityWorldPos = m_selectedEntity->getWorldPosition();
			m_selectedEntity->setWorldPosition(glm::vec3(newX, entityWorldPos.y, entityWorldPos.z));
		}

		if (m_selectedGizmo == m_transformGizmoYArrow)
		{
			glm::vec3 newDepthPos = renderer->screenToWorldPosition(input::cursorPosition, frameBuffer);
			float newY = (newDepthPos.y - m_gizmoSelectLocation.y);
			m_gizmo->setWorldPosition(glm::vec3(m_transformGizmoYArrow->getWorldPosition().x, newY, m_transformGizmoYArrow->getWorldPosition().z));
			glm::vec3 entityWorldPos = m_selectedEntity->getWorldPosition();
			m_selectedEntity->setWorldPosition(glm::vec3(entityWorldPos.x, newY, entityWorldPos.z));
		}

		if (m_selectedGizmo == m_transformGizmoZArrow)
		{
			glm::vec3 newDepthPos = renderer->screenToWorldPosition(input::cursorPosition, frameBuffer);
			float newZ = (newDepthPos.z - m_gizmoSelectLocation.z);
			m_gizmo->setWorldPosition(glm::vec3(m_transformGizmoYArrow->getWorldPosition().x, m_transformGizmoYArrow->getWorldPosition().y, newZ));
			glm::vec3 entityWorldPos = m_selectedEntity->getWorldPosition();
			m_selectedEntity->setWorldPosition(glm::vec3(entityWorldPos.x, entityWorldPos.y, newZ));
		}
	}
	else
		m_selectedGizmo = nullptr;
}