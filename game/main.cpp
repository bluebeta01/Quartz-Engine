#include "framework/framework.h"
#include <thread>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

int main()
{
	std::thread assetThread = asset::assetInit();
	filesystem::registerPlugin("game");
	screen::initializeScreen();

	World* world = new World();
	Renderer* renderer = new Renderer(world);
	renderer->initialize(screen::windowHandle, 1280, 720);

	Entity* e = world->createEntity("MyEntity", Transform(glm::vec3(0, 0, 15), glm::vec3(45,45,0), glm::vec3(1,1,1)));
	RenderComponent* rc = new RenderComponent();
	rc->setModel(renderer->getModel("Cube"));
	e->addComponent(rc);

	/*Entity* ball = world->createEntity("ball", Transform(glm::vec3(0.0f, 0.0f, 10.0f)));
	RenderComponent* ballrc = new RenderComponent();
	ballrc->setModel(renderer->getModel("Woodball"));
	ball->addComponent(ballrc);*/

	Entity* tinyBall = world->createEntity("tinyball", Transform(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0,0,0), glm::vec3(0.2,0.2,0.2)));
	RenderComponent* tinyBallrc = new RenderComponent();
	tinyBallrc->setModel(renderer->getModel("Woodball"));
	tinyBall->addComponent(tinyBallrc);

	Entity* plane = world->createEntity("plane", Transform(glm::vec3(0.0f, -1.0f, 10.0f), glm::vec3(-90, 0, 0), glm::vec3(100.0, 100.0, 100.0)));
	RenderComponent* planerc = new RenderComponent();
	planerc->setModel(renderer->getModel("debugplane"));
	plane->addComponent(planerc);


	while (!screen::terminated)
	{
		screen::updateScreen();

		

		renderer->setCamera(Camera(glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::radians(40.0f), 0.1f, 1000.0f, (float)renderer->m_width / (float)renderer->m_height));
		renderer->render();
		
		tinyBall->transform.position = renderer->screenToWorldPosition(input::mousePosition);

		renderer->render();

		

		renderer->present();

		tinyBall->transform.position = glm::vec3(0, 0, 0);

		//e->transform.position.z -= 0.0001f;
	}


	assetThread.join();
}