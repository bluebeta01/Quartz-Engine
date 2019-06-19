#include "framework/framework.h"
#include <thread>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>
#include <DirectXMath.h>
#include <d3dx9math.h>

Camera camera = Camera(glm::vec3(0,0,0), glm::vec3(0,0,0), 40.0f, 0.1f, 1000.0f, 16.0f/9.0f);

int main()
{
	std::thread assetThread = asset::assetInit();
	filesystem::registerPlugin("game");

	//init
	screen::initializeScreen();
	Renderer* renderer = new Renderer(1280, 720);

	Model* m = renderer->getModel("Woodball");

	while (!screen::terminated)
	{
		screen::updateScreen();
		renderer->processRenderJobs();
		renderer->clearFrame();

		struct mats
		{
			glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
		};
		mats mat;
		mat.modelMatrix = renderer->matrixFromTransform(Transform(glm::vec3(0, 0, -15)), true);
		mat.viewMatrix = glm::mat4(1.0f);
		mat.projectionMatrix = glm::perspective(glm::radians(40.0f), 16.0f / 9.0f, 0.1f, 1000.0f);

		//mat.modelMatrix = glm::transpose(mat.modelMatrix);
		//mat.viewMatrix = glm::transpose(mat.viewMatrix);
		//mat.projectionMatrix = glm::transpose(mat.projectionMatrix);

		SHADER_CONSTANT_BUFFER cb;
		cb.bufferDataSize = sizeof(mat);
		cb.slotValue = 0;
		cb.data = &mat;

		renderer->renderModel(m, renderer->shaderManager.getShaderByName("standard"), 1, &cb);

		screen::swapBuffers();
	}


	assetThread.join();
}