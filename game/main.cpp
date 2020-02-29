#include "framework/framework.h"
#include <thread>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

int main()
{
	std::thread assetThread = asset::assetInit();
	filesystem::registerPlugin("game");

	//init
	screen::initializeScreen();
	World* world = new World();
	Renderer* renderer = new Renderer(world);
	renderer->initialize(screen::windowHandle, 1280, 720);

	Entity* e = world->createEntity("MyEntity", Transform(glm::vec3(-15, 0, 0)));
	RenderComponent* rc = new RenderComponent();
	rc->setModel(renderer->getModel("Woodball"));
	e->addComponent(rc);

	Model* m = renderer->getModel("Woodball");

	while (!screen::terminated)
	{
		screen::updateScreen();
		renderer->setCamera(Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 40.0f, 0.1f, 1000.0f, (float)renderer->m_width / (float)renderer->m_height));
		renderer->render();
		renderer->present();

		e->transform.position.z -= 0.0001f;
	}


	assetThread.join();
}