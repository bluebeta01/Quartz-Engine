#pragma once
#include "windows/c_dockerwindow.h"
#include "windows/c_assetswindow.h"
#include "windows/c_propertywindow.h"
#include "windows/c_renderwindow.h"
#include <framework/imgui/imgui.h>
#include <framework/imgui/imgui_impl_opengl3.h>
#include <framework/imgui/imgui_impl_win32.h>

class GUI
{
public:
	DockerWindow* dockerWindow;
	AssetWindow* assetWindow;
	PropertyWindow* propertyWindow;
	RenderWindow* renderWindow;
	GUI();
	~GUI();
	void drawWindow();
};