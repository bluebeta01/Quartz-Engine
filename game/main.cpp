#include "framework/framework.h"
#include <thread>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

//Camera camera = Camera(glm::vec3(0,0,0), glm::vec3(0,0,0), 40.0f, 0.1f, 1000.0f, 16.0f/9.0f);

int main()
{
	std::thread assetThread = asset::assetInit();
	filesystem::registerPlugin("game");

	//init
	screen::initializeScreen();
	Renderer* renderer = new Renderer();
	renderer->initialize(screen::windowHandle, 1280, 720);

	Model* m = renderer->getModel("Woodball");

	while (!screen::terminated)
	{
		screen::updateScreen();
		renderer->render(m, Transform(glm::vec3(0, 0, -15), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
		renderer->present();
	}


	assetThread.join();
}