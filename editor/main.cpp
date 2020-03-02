#include "framework/framework.h"
#include <thread>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/matrix_decompose.hpp>
#include <chrono>

Camera camera;
Entity* selectedEntity;
Entity* selectedGizmo;
Renderer* renderer;
Framebuffer* frameBuffer;
World* world;

Entity* gizmoXArrow;
Entity* gizmoYArrow;
Entity* gizmoZArrow;

glm::vec3 gizmoSelectLocation;
glm::vec3 gizmoOriginalLocation;

void cameraMovement()
{
	float cameraSpeed = 0.3f;
	float cameraSensitivity = 0.1f;

	if (input::isMouseButtonDown(input::MOUSEBUTTON_RIGHT))
	{
		input::lockMouse = true;
		camera.transform.rotation.x -= input::mouseDelta.y * cameraSensitivity;
		camera.transform.rotation.y -= input::mouseDelta.x * cameraSensitivity;

		if (input::isKeyDown('W'))
		{
			camera.transform.position += camera.getForward() * cameraSpeed;
		}
		if (input::isKeyDown('S'))
		{
			camera.transform.position -= camera.getForward() * cameraSpeed;
		}
		if (input::isKeyDown('A'))
		{
			camera.transform.position += camera.getRight() * cameraSpeed;
		}
		if (input::isKeyDown('D'))
		{
			camera.transform.position -= camera.getRight() * cameraSpeed;
		}
	}
	else
		input::lockMouse = false;
}

void selectEntity()
{
	if (input::isMouseButtonClicked(input::MOUSEBUTTON_LEFT))
	{
		Entity* e = renderer->colorPick(input::mousePosition);
		if (e) {
			if (e->m_uid == gizmoXArrow->m_uid || e->m_uid == gizmoYArrow->m_uid || e->m_uid == gizmoZArrow->m_uid)
				return;
			selectedEntity = e;
			if (selectedEntity)
			{
				gizmoXArrow->transform.position = selectedEntity->transform.position;
				gizmoYArrow->transform.position = selectedEntity->transform.position;
				gizmoZArrow->transform.position = selectedEntity->transform.position;
			}
		}
	}
}

void setupGizmo()
{
	if (!selectedEntity)
		return;
	float gizmoSize = 0.004f;
	float distance;

	distance = glm::distance(gizmoXArrow->transform.position, camera.transform.position);
	gizmoXArrow->transform.scale = glm::vec3(distance * gizmoSize, distance * gizmoSize, distance * gizmoSize);
	gizmoYArrow->transform.scale = glm::vec3(distance * gizmoSize, distance * gizmoSize, distance * gizmoSize);
	gizmoZArrow->transform.scale = glm::vec3(distance * gizmoSize, distance * gizmoSize, distance * gizmoSize);
}

void gizmoSelect()
{
	if (input::isMouseButtonClicked(input::MOUSEBUTTON_LEFT))
	{
		Entity* e = renderer->colorPick(input::mousePosition);

		if (e)
		{
			if (e->m_uid == gizmoXArrow->m_uid)
			{
				renderer->clearFramebuffer(frameBuffer);

				Entity* debugPlane = world->createEntity("debugplane", Transform(gizmoXArrow->transform.position, glm::vec3(-90, 0, 0), glm::vec3(1000, 1000, 1000)));
				RenderComponent* rc = new RenderComponent();
				rc->setModel(renderer->getModel("debugplane"));
				debugPlane->addComponent(rc);

				debugPlane->transform.rotation.x = -camera.transform.rotation.x + 180;

				renderer->renderModel(((RenderComponent*)debugPlane->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT))->getModel(),
					Transform::matrixFromTransform(debugPlane->transform, true), camera.getViewMatrix(), camera.getPerspectiveMatrix(),
					nullptr, frameBuffer, true);

				world->destoryEntity(debugPlane->m_uid);

				selectedGizmo = gizmoXArrow;

				gizmoOriginalLocation = gizmoXArrow->transform.position;
				gizmoSelectLocation = renderer->screenToWorldPosition(input::mousePosition, frameBuffer);

			}

			if (e->m_uid == gizmoYArrow->m_uid)
			{
				renderer->clearFramebuffer(frameBuffer);

				Entity* debugPlane = world->createEntity("debugplane", Transform(gizmoYArrow->transform.position, glm::vec3(0, 0, 0), glm::vec3(1000, 1000, 1000)));
				RenderComponent* rc = new RenderComponent();
				rc->setModel(renderer->getModel("debugplane"));
				debugPlane->addComponent(rc);

				debugPlane->transform.rotation.y = -camera.transform.rotation.y - 90;

				renderer->renderModel(((RenderComponent*)debugPlane->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT))->getModel(),
					Transform::matrixFromTransform(debugPlane->transform, true), camera.getViewMatrix(), camera.getPerspectiveMatrix(),
					nullptr, frameBuffer, true);

				world->destoryEntity(debugPlane->m_uid);

				selectedGizmo = gizmoYArrow;

				gizmoOriginalLocation = gizmoYArrow->transform.position;
				gizmoSelectLocation = renderer->screenToWorldPosition(input::mousePosition, frameBuffer);

			}

			if (e->m_uid == gizmoZArrow->m_uid)
			{
				renderer->clearFramebuffer(frameBuffer);

				Entity* debugPlane = world->createEntity("debugplane", Transform(gizmoZArrow->transform.position, glm::vec3(-90, 0, 0), glm::vec3(1000, 1000, 1000)));
				RenderComponent* rc = new RenderComponent();
				rc->setModel(renderer->getModel("debugplane"));
				debugPlane->addComponent(rc);

				debugPlane->transform.rotation.z = -camera.transform.rotation.y - 90;

				renderer->renderModel(((RenderComponent*)debugPlane->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT))->getModel(),
					Transform::matrixFromTransform(debugPlane->transform, true), camera.getViewMatrix(), camera.getPerspectiveMatrix(),
					nullptr, frameBuffer, true);

				world->destoryEntity(debugPlane->m_uid);

				selectedGizmo = gizmoZArrow;

				gizmoOriginalLocation = gizmoZArrow->transform.position;
				gizmoSelectLocation = renderer->screenToWorldPosition(input::mousePosition, frameBuffer);

			}

		}
		else
		{
			selectedGizmo = nullptr;
			selectedEntity = nullptr;
			gizmoXArrow->transform.position.y = -100000;
			gizmoYArrow->transform.position.y = -100000;
			gizmoZArrow->transform.position.y = -100000;
		}
		
	}
}

void gizmoDrag()
{
	if (input::isMouseButtonDown(input::MOUSEBUTTON_LEFT) && selectedEntity && selectedGizmo)
	{
		if (selectedGizmo == gizmoXArrow)
		{
			glm::vec3 newDepthPos = renderer->screenToWorldPosition(input::mousePosition, frameBuffer);
			float newX = gizmoOriginalLocation.x + (newDepthPos.x - gizmoSelectLocation.x);
			gizmoXArrow->transform.position.x = newX;
			gizmoYArrow->transform.position.x = newX;
			gizmoZArrow->transform.position.x = newX;
			selectedEntity->transform.position.x = newX;
		}

		if (selectedGizmo == gizmoYArrow)
		{
			glm::vec3 newDepthPos = renderer->screenToWorldPosition(input::mousePosition, frameBuffer);
			float newY = gizmoOriginalLocation.y + (newDepthPos.y - gizmoSelectLocation.y);
			gizmoXArrow->transform.position.y = newY;
			gizmoYArrow->transform.position.y = newY;
			gizmoZArrow->transform.position.y = newY;
			selectedEntity->transform.position.y = newY;
		}

		if (selectedGizmo == gizmoZArrow)
		{
			glm::vec3 newDepthPos = renderer->screenToWorldPosition(input::mousePosition, frameBuffer);
			float newZ = gizmoOriginalLocation.z + (newDepthPos.z - gizmoSelectLocation.z);
			gizmoXArrow->transform.position.z = newZ;
			gizmoYArrow->transform.position.z = newZ;
			gizmoZArrow->transform.position.z = newZ;
			selectedEntity->transform.position.z = newZ;
		}
	}
	else
		selectedGizmo = nullptr;
}

void planeDebug()
{
	Entity* debugPlane = world->createEntity("debugplane", Transform(gizmoXArrow->transform.position, glm::vec3(-90, 0, 0), glm::vec3(1, 1, 1)));
	RenderComponent* rc = new RenderComponent();
	rc->setModel(renderer->getModel("debugplane"));
	debugPlane->addComponent(rc);

	debugPlane->transform.rotation.z = -camera.transform.rotation.y - 90;

	renderer->renderModel(((RenderComponent*)debugPlane->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT))->getModel(),
		Transform::matrixFromTransform(debugPlane->transform, true), camera.getViewMatrix(), camera.getPerspectiveMatrix(),
		nullptr, nullptr, true);

	world->destoryEntity(debugPlane->m_uid);
}

int main()
{
	std::thread assetThread = asset::assetInit();
	filesystem::registerPlugin("game");
	screen::initializeScreen();

	world = new World();
	renderer = new Renderer(world);
	renderer->initialize(screen::windowHandle, 1280, 720);
	frameBuffer = renderer->createFramebuffer();

	//The debugplane model should always remain in memeory
	renderer->getModel("debugplane");

	gizmoXArrow = world->createEntity("gizmoXArrow", Transform(glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::vec3(0.01, 0.01, 0.01)));
	RenderComponent* gizmoXArrowRc = new RenderComponent();
	gizmoXArrowRc->setModel(renderer->getModel("xarrow"));
	//gizmoXArrowRc->enabled = false;
	gizmoXArrowRc->m_onTop = true;
	gizmoXArrow->addComponent(gizmoXArrowRc);
	gizmoYArrow = world->createEntity("gizmoYArrow", Transform(glm::vec3(0, 0, 0), glm::vec3(-90, 0, 0), glm::vec3(0.01, 0.01, 0.01)));
	RenderComponent* gizmoYArrowRc = new RenderComponent();
	gizmoYArrowRc->setModel(renderer->getModel("yarrow"));
	//gizmoYArrowRc->enabled = false;
	gizmoYArrowRc->m_onTop = true;
	gizmoYArrow->addComponent(gizmoYArrowRc);
	gizmoZArrow = world->createEntity("gizmoZArrow", Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.01, 0.01, 0.01)));
	RenderComponent* gizmoZArrowRc = new RenderComponent();
	gizmoZArrowRc->setModel(renderer->getModel("zarrow"));
	//gizmoZArrowRc->enabled = false;
	gizmoZArrowRc->m_onTop = true;
	gizmoZArrow->addComponent(gizmoZArrowRc);

	Entity* e = world->createEntity("MyEntity", Transform(glm::vec3(0, 0, 15), glm::vec3(45, 45, 0), glm::vec3(1, 1, 1)));
	RenderComponent* rc = new RenderComponent();
	rc->setModel(renderer->getModel("Cube"));
	e->addComponent(rc);

	Entity* plane = world->createEntity("plane", Transform(glm::vec3(0.0f, 1.0f, 10.0f), glm::vec3(0, 180, 0), glm::vec3(1.0, 1.0, 1.0)));
	RenderComponent* planerc = new RenderComponent();
	planerc->setModel(renderer->getModel("debugplane"));
	plane->addComponent(planerc);


	Entity* ball = world->createEntity("ball", Transform(glm::vec3(0.0f, 1.0f, 10.0f), glm::vec3(0, 0, 0), glm::vec3(1.0, 1.0, 1.0)));
	RenderComponent* ballrc = new RenderComponent();
	ballrc->setModel(renderer->getModel("woodball"));
	ball->addComponent(ballrc);

	camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::radians(40.0f), 0.1f, 1000.0f, (float)renderer->m_width / (float)renderer->m_height);

	while (!screen::terminated)
	{
		screen::updateScreen();
		cameraMovement();
		selectEntity();
		setupGizmo();
		gizmoSelect();
		gizmoDrag();

		//plane->transform.rotation.y =  -camera.transform.rotation.y - 90;


		renderer->setCamera(camera);
		renderer->render();
		//planeDebug();
		//LOGDEBUG(renderer->screenToWorldPosition(input::mousePosition, nullptr).x);

		renderer->present();
	}


	assetThread.join();
}