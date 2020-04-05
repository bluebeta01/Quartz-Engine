#include "pch.h"
#include "c_engine.h"
#include "asset/asset.h"
#include "filesystem/filesystem.h"
#include "screen/screen.h"

Engine::Engine(HWND windowHandle, int windowWidth, int windowHeight) : m_world(), m_renderer(&m_world)
{
	m_assetThread = asset::assetInit();
	filesystem::registerPlugin("game");
	if (windowHandle == NULL)
	{
		screen::initializeScreen();
		m_renderer.initialize(screen::windowHandle, windowWidth, windowHeight);
	}
	else
	{
		m_renderer.initialize(windowHandle, windowWidth, windowHeight);
	}


}

void Engine::update()
{
	screen::updateScreen();

	for (auto p : m_world.m_entityMap)
	{
		for (auto i : p.second->m_components)
		{
			i.second->update();
		}
	}
}

void Engine::terminate()
{
	m_assetThread.join();
}