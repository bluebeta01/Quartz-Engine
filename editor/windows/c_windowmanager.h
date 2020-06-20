#pragma once
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_win32.h>
#include <Imgui/imgui_impl_dx11.h>
#include "c_window.h"
#include "c_renderwindow.h"

static class WindowManager
{
public:
	static RenderWindow* s_renderWindow;
	static ID3D11Device* s_device;

	static void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	static void renderWindows();

private:
	static void setStyle();
};