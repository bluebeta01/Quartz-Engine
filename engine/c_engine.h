#pragma once
#include <thread>
#include "c_world.h"
#include "render/c_renderer.h"

class Engine
{
public:
	World m_world;
	Renderer m_renderer;
	Engine(HWND windowHandle, int windowWidth, int windowHeight);
	void update();
	void terminate();

private:
	std::thread m_assetThread;
};