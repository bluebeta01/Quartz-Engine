#include "pch.h"
#include "c_engine.h"
#include "asset/asset.h"
#include "filesystem/filesystem.h"
#include "c_gamewindow.h"

Engine::Engine() : m_world(), m_renderer(&m_world)
{
	m_assetThread = asset::assetInit();
	filesystem::registerPlugin("game");
	if (GameWindow::s_windowHandle == NULL)
	{
		m_renderer.initialize();
	}
	else
	{
		m_renderer.initialize();
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