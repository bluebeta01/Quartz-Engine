#pragma once
#include <Windows.h>
#include <glm.hpp>

static class GameWindow
{
public:
	static glm::vec2 s_clientSize;
	static glm::vec2 s_renderAreaSize;
	static glm::vec2 s_renderAreaTopLeft;
	static glm::vec2 s_cursorPosInRenderArea;
	static bool s_isTerminating;
	static bool s_resizeEvent;
	static bool s_renderAreaLockedToClientSize;
	static bool (*s_imguiWndProcCallback)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HWND s_windowHandle;

	static void initialize(int sizeX, int sizeY);
	static void tick();

private:
	static void centerCursor();
};