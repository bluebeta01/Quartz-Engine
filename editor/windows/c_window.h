#pragma once
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_win32.h>
#include <Imgui/imgui_impl_dx11.h>

class Window
{
public:
	virtual ~Window() {};
	virtual void show() { m_enabled = true; };
	virtual void hide() { m_enabled = false; };
	virtual void render() {};

protected:
	bool m_enabled = false;
};