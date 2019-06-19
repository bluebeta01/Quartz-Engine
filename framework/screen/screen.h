#pragma once
#include "framework/pch.h"
#include <d3d11.h>
#include <D3DX11.h>

namespace screen
{
	extern IDXGISwapChain* dxSwapChain;
	extern ID3D11Device* dxDevice;
	extern ID3D11DeviceContext* dxDeviceContext;
	extern ID3D11RenderTargetView* dxBackBuffer;
	extern bool terminated;
	extern glm::vec2 clientAreaSize;
	extern HWND windowHandle;
	void initializeScreen();
	void updateScreen();
	void swapBuffers();
	void hideCursor();
	void showCursor();
}