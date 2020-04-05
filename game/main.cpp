#include "framework/framework.h"
#include <thread>
#include <iostream>

World* world;
Renderer* renderer;

int main()
{
	std::thread assetThread = asset::assetInit();
	filesystem::registerPlugin("game");
	screen::initializeScreen();

	world = new World();
	renderer = new Renderer(world);
	renderer->initialize(screen::windowHandle, 1280, 720);

	

	while (!screen::terminated)
	{
		screen::updateScreen();
		renderer->setCamera(Camera(glm::vec3(0, 0, -10), glm::vec3(0, 90, 0), glm::radians(40.0f), 0.1f, 1000.0f, (float)renderer->m_width / (float)renderer->m_height));
		renderer->render();
		renderer->present();
	}


	assetThread.join();
}