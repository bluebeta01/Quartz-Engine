#pragma once
#include <thread>
#include "c_world.h"
#include "render/c_renderer.h"
#include "c_gamewindow.h"

class Engine
{
public:
	World m_world;
	Renderer m_renderer;
	Engine();
	void tick();
	void terminate();

private:
	std::thread m_assetThread;
};