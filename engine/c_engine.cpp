#include "pch.h"
#include "c_engine.h"
#include "asset/asset.h"
#include "filesystem/filesystem.h"
#include "c_gamewindow.h"

Engine::Engine(GameWindow* gameWindow) : m_world(), m_renderer(&m_world)
{
	m_assetThread = asset::assetInit(&gameWindow->s_isTerminating);
	filesystem::registerPlugin("game");
	if (gameWindow->m_windowHandle == NULL)
	{
		m_renderer.initialize(gameWindow);
	}
	else
	{
		m_renderer.initialize(gameWindow);
	}


}

void Engine::tick()
{
	m_renderer.tick();
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