#include "c_propertywindow.h"
#include <framework/imgui/imgui.h>
#include <framework/imgui/imgui_impl_opengl3.h>
#include <framework/imgui/imgui_impl_win32.h>

PropertyWindow::PropertyWindow()
{

}

PropertyWindow::~PropertyWindow()
{

}

void PropertyWindow::render()
{
	ImGui::Begin("Properties");
	ImGui::End();
}