#include <framework/framework.h>
#include <vector>
#include <thread>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>
#include "gui/c_gui.h"

std::vector<PointLight> lights;
std::vector<Entity*> entities;
Camera camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 40.0f, 0.1f, 1000.0f, 16.0f / 9.0f);

GUI* gui;

Entity* selectedEntity;

int main()
{
	std::thread assetThread = asset::assetInit();
	filesystem::registerPlugin("game");

	//init
	screen::initializeScreen();
	Renderer* renderer = new Renderer(1280, 720);
	PointLight pl;
	pl.color = glm::vec3(1, 0, 0);
	lights.push_back(pl);
	pl.color = glm::vec3(0, 0, 1);
	pl.position = glm::vec3(0, 0, -30);
	lights.push_back(pl);

	StaticMeshEntity* g = new StaticMeshEntity("yarrow", Transform(glm::vec3(0, 0, -15)));
	g->getComponent<RenderComponent*>()->setModel(renderer->getModel("yarrow"));
	//entities.push_back(g);

	StaticMeshEntity * ball = new StaticMeshEntity("woodenball", Transform(glm::vec3(0, 0, -15)));
	ball->getComponent<RenderComponent*>()->setModel(renderer->getModel("Woodball"));
	entities.push_back(ball);
	StaticMeshEntity * debugPlane = new StaticMeshEntity("debugplane", Transform(glm::vec3(0, -1, 0), glm::vec3(), glm::vec3(100, 100, 100)));
	debugPlane->getComponent<RenderComponent*>()->setModel(renderer->getModel("debugplane"));
	entities.push_back(debugPlane);

	

	gui = new GUI();
	gui->renderWindow->camera = &camera;

	

	while (!screen::terminated)
	{
		screen::updateScreen();
		renderer->prepFrame();

		if (gui->renderWindow->amLooking)
		{
			float sensitivity = 0.1;
			camera.transform.rotation.y += input::mouseDelta.x * sensitivity;
			camera.transform.rotation.x += input::mouseDelta.y * sensitivity;

			if (input::isKeyDown('W'))
			{
				camera.transform.position -= camera.getForward();
			}
			if (input::isKeyDown('S'))
			{
				camera.transform.position += camera.getForward();
			}
			if (input::isKeyDown('A'))
			{
				camera.transform.position += glm::cross(camera.getForward(), glm::vec3(0, 1, 0));
			}
			if (input::isKeyDown('D'))
			{
				camera.transform.position -= glm::cross(camera.getForward(), glm::vec3(0, 1, 0));
			}
		}


		renderer->setFrameBuffer(gui->renderWindow->sceneFrameBuffer);
		renderer->setViewMatrix(camera.getViewMatrix());
		renderer->setProjectionMatrix(glm::perspective(glm::radians(camera.fov), camera.aspect, camera.nearPlane, camera.farPlane));
		renderer->setLightsVector(lights);
		renderer->prepFrame();
		renderer->drawEntities(entities);

		Transform t = Transform(glm::vec3(0, 1, -15));
		glm::mat4 modelMatrix = renderer->matrixFromTransform(t, true);
		glm::mat4 projMatrix = glm::perspective(glm::radians(camera.fov), camera.aspect, camera.nearPlane, camera.farPlane);
		glm::mat4 viewMatrix = camera.getViewMatrix();

		SHADER_PROPERTY props[3];
		props[0].name = "projMatrix";
		props[0].type = SHADER_PROPERTY_TYPE_MAT4;
		props[0].value = glm::value_ptr(projMatrix);

		props[1].name = "transMatrix";
		props[1].type = SHADER_PROPERTY_TYPE_MAT4;
		props[1].value = glm::value_ptr(modelMatrix);

		props[2].name = "viewMatrix";
		props[2].type = SHADER_PROPERTY_TYPE_MAT4;
		props[2].value = glm::value_ptr(viewMatrix);


		renderer->renderModel(renderer->getModel("Woodball"), renderer->shaderManager.getShaderByName("standard"), 3, props);

		renderer->setFrameBuffer(NULL);

		gui->drawWindow();


		/*if (gui->renderWindow->leftClicked)
		{
			selectedEntity = renderer->entityColorPick(entities, gui->renderWindow->localCursorPosition, glm::vec2(gui->renderWindow->windowSize.x, gui->renderWindow->windowSize.y));
			if(selectedEntity)
				LOGDEBUG(selectedEntity->name);
		}*/

		//LOGDEBUG(gui->renderWindow->localCursorPosition.y);

		screen::swapBuffers();
	}


	assetThread.join();
}