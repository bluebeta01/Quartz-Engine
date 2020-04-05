#include "pch.h"
#include "screen.h"
#include "input/input.h"

namespace screen
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	MSG msg;
	HWND windowHandle;
	HDC windowDeviceContext;
	bool terminated = false;
	glm::vec2 clientAreaSize;

	IDXGISwapChain* dxSwapChain;
	ID3D11Device* dxDevice;
	ID3D11DeviceContext* dxDeviceContext;
	ID3D11RenderTargetView* dxBackBuffer;

	void initD3D()
	{
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
		scd.BufferCount = 1;                                    // one back buffer
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
		scd.OutputWindow = windowHandle;                                // the window to be used
		scd.SampleDesc.Count = 4;                               // how many multisamples
		scd.Windowed = TRUE;                                    // windowed/full-screen mode

		// create a device, device context and swap chain using the information in the scd struct
		D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			NULL,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&scd,
			&dxSwapChain,
			&dxDevice,
			NULL,
			&dxDeviceContext);

		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer;
		dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);

		// use the back buffer address to create the render target
		dxDevice->CreateRenderTargetView(pBackBuffer, NULL, &dxBackBuffer);
		pBackBuffer->Release();

		// set the render target as the back buffer
		dxDeviceContext->OMSetRenderTargets(1, &dxBackBuffer, NULL);

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = 1280;
		viewport.Height = 720;

		dxDeviceContext->RSSetViewports(1, &viewport);
	}

	void initializeScreen()
	{
		WNDCLASSEX wc;

		// clear out the window class for use
		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		// fill in the struct with the needed information
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = "quartzwindow";

		RegisterClassEx(&wc);

		clientAreaSize = glm::vec2(1280, 720);

		RECT wr = { 0, 0, 1280, 720 };    // set the size, but not the position
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
		windowHandle = CreateWindowEx(
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

		if (!windowHandle)
		{
			LOGERROR("Failed to create the window handle!");
			return;
		}

		ShowWindow(windowHandle, 1);
		//initD3D();
	}



	void centerCursor()
	{
		RECT windowRect = {};
		GetWindowRect(windowHandle, &windowRect);
		int x = ((windowRect.right - windowRect.left) / 2) + windowRect.left;
		int y = ((windowRect.bottom - windowRect.top) / 2) + windowRect.top;
		SetCursorPos(x, y);
	}
	void updateMouseInput()
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(windowHandle, &p);
		input::mouseDelta.x = p.x - input::mousePosition.x;
		input::mouseDelta.y = p.y - input::mousePosition.y;
		if (input::lockMouse)
		{
			centerCursor();
			GetCursorPos(&p);
			ScreenToClient(windowHandle, &p);
			
			
		}
		else
		{
			
		}
		input::mousePosition = glm::vec2((int)p.x, (int)p.y);
	}

	void setCursorPosition(glm::vec2 position)
	{
		POINT p;
		p.x = position.x;
		p.y = position.y;
		ClientToScreen(windowHandle, &p);
		SetCursorPos(p.x, p.y);
	}

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
			GetClientRect(windowHandle, &clientSize);
			clientAreaSize = glm::vec2((int)clientSize.right, (int)clientSize.bottom);
			break;
		}
		case WM_CLOSE:
		{
			terminated = true;
			break;
		}

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	void swapBuffers()
	{
		dxSwapChain->Present(0, 0);
	}
	void updateScreen()
	{
		input::clearMouseButtonsClicked();
		input::clearKeysPressed();

		while (PeekMessage(&msg, NULL, 0, 0, 1))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		updateMouseInput();
	}
	void hideCursor()
	{
		ShowCursor(false);
	}
	void showCursor()
	{
		ShowCursor(true);
	}
}