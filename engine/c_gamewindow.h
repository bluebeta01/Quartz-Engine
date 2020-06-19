#pragma once
#include <Windows.h>
#include <glm.hpp>

class GameWindow
{
public:
	HWND m_windowHandle;
	static bool s_isTerminating;
	static glm::vec2 s_clientSize;
	static bool s_resizeEvent;
	GameWindow();
	void initialize(int sizeX, int sizeY);
	void tick();

private:
	void centerCursor();
};