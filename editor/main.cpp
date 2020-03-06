#include "framework/framework.h"
#include "c_gizmo.h"

Camera camera;
Renderer* renderer;
Framebuffer* frameBuffer;
World* world;

Gizmo* gizmo;

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

	gizmo = new Gizmo();


	//Entity* e = world->createEntity("MyEntity", Transform(glm::vec3(0, 0, 15), glm::vec3(45, 45, 0), glm::vec3(1, 1, 1)));
	//RenderComponent* rc = new RenderComponent();
	//rc->setModel(renderer->getModel("Cube"));
	//e->addComponent(rc);

	//Entity* plane = world->createEntity("plane", Transform(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0, 180, 0), glm::vec3(1.0, 1.0, 1.0)));
	//RenderComponent* planerc = new RenderComponent();
	//planerc->setModel(renderer->getModel("debugplane"));
	//plane->addComponent(planerc);


	//Entity* ball = world->createEntity("ball", Transform(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(1.0, 1.0, 1.0)));
	//RenderComponent* ballrc = new RenderComponent();
	//ballrc->setModel(renderer->getModel("woodball"));
	//ball->addComponent(ballrc);

	//plane->addChild(ball);

	

	camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::radians(40.0f), 0.1f, 1000.0f, (float)renderer->m_width / (float)renderer->m_height);

	while (!screen::terminated)
	{
		screen::updateScreen();
		cameraMovement();
		gizmo->update();

		renderer->setCamera(camera);
		renderer->render();
		renderer->present();


		if (input::isKeyPressed('P'))
		{
			for (int x = 0; x < 20; x++)
			{
				for (int y = 0; y < 20; y++)
				{
					for (int z = 0; z < 20; z++)
					{
						Entity* ball = world->createEntity("ball", Transform(glm::vec3(x * 3, y * 3, z * 3), glm::vec3(0, 0, 0), glm::vec3(1.0, 1.0, 1.0)));
						RenderComponent* ballrc = new RenderComponent();
						ballrc->setModel(renderer->getModel("woodball"));
						ball->addComponent(ballrc);
					}
				}
			}
		}
	}


	assetThread.join();
}