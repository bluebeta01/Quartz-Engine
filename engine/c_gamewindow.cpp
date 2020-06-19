#include "pch.h"
#include "c_gamewindow.h"
#include "input/input.h"

namespace Window
{
	MSG msg;

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			input::registerMouseButtonDown(input::MOUSEBUTTON_LEFT);
			input::registerMouseButtonClick(input::MOUSEBUTTON_LEFT);
			break;
		}
		case WM_LBUTTONUP:
		{
			input::registerMouseButtonUp(input::MOUSEBUTTON_LEFT);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			input::registerMouseButtonDown(input::MOUSEBUTTON_RIGHT);
			input::registerMouseButtonClick(input::MOUSEBUTTON_RIGHT);
			break;
		}
		case WM_RBUTTONUP:
		{
			input::registerMouseButtonUp(input::MOUSEBUTTON_RIGHT);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			input::registerMouseButtonDown(input::MOUSEBUTTON_MIDDLE);
			input::registerMouseButtonClick(input::MOUSEBUTTON_MIDDLE);
			break;
		}
		case WM_MBUTTONUP:
		{
			input::registerMouseButtonUp(input::MOUSEBUTTON_MIDDLE);
			break;
		}
		case WM_KEYDOWN:
		{
			input::registerKeyDown(wParam);
			input::registerKeyPressed(wParam);
			break;
		}
		case WM_KEYUP:
		{
			input::registerKeyUp(wParam);
			break;
		}
		case WM_SIZE:
		{
			RECT clientSize = {};
			GetClientRect(hwnd, &clientSize);
			GameWindow::s_clientSize = glm::vec2((int)clientSize.right, (int)clientSize.bottom);
			GameWindow::s_resizeEvent = true;
			break;
		}
		case WM_CLOSE:
		{
			GameWindow::s_isTerminating = true;
			break;
		}

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

bool GameWindow::s_isTerminating = false;
bool GameWindow::s_resizeEvent = false;
bool GameWindow::s_renderAreaLockedToClientSize = true;
glm::vec2 GameWindow::s_clientSize = glm::vec2();
glm::vec2 GameWindow::s_renderAreaSize = glm::vec2();
glm::vec2 GameWindow::s_renderAreaTopLeft = glm::vec2();
HWND GameWindow::s_windowHandle;

void GameWindow::initialize(int sizeX, int sizeY)
{
	s_clientSize.x = sizeX;
	s_clientSize.y = sizeY;
	s_renderAreaSize.x = sizeX;
	s_renderAreaSize.y = sizeY;
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = Window::WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "quartzwindow";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, s_clientSize.x, s_clientSize.y};    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
	s_windowHandle = CreateWindowEx(
		0,                              // Optional window styles.
		"quartzwindow",                     // Window class
		"Quartz RPG Engine",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,

		NULL,       // Parent window    
		NULL,       // Menu
		GetModuleHandle(NULL),  // Instance handle
		NULL        // Additional application data
	);

	if (!s_windowHandle)
	{
		LOGERROR("Failed to create the window handle!");
		return;
	}

	ShowWindow(s_windowHandle, 1);
}

void GameWindow::tick()
{
	if (s_renderAreaLockedToClientSize)
		s_renderAreaSize = s_clientSize;

	input::clearKeysPressed();
	input::clearMouseButtonsClicked();
	s_resizeEvent = false;

	while (PeekMessage(&Window::msg, NULL, 0, 0, 1))
	{
		TranslateMessage(&Window::msg);
		DispatchMessage(&Window::msg);

		POINT p;
		GetCursorPos(&p);
		ScreenToClient(s_windowHandle, &p);
		input::cursorDelta.x = p.x - input::cursorPosition.x;
		input::cursorDelta.y = p.y - input::cursorPosition.y;

		if (input::cursorLock)
		{
			centerCursor();
			GetCursorPos(&p);
			ScreenToClient(s_windowHandle, &p);
		}
		input::cursorPosition = glm::vec2(p.x, p.y);

		
	}

	//centerCursor();
}

void GameWindow::centerCursor()
{
	RECT windowRect = {};
	GetWindowRect(s_windowHandle, &windowRect);
	int x = ((windowRect.right - windowRect.left) / 2) + windowRect.left;
	int y = ((windowRect.bottom - windowRect.top) / 2) + windowRect.top;
	SetCursorPos(x, y);
}