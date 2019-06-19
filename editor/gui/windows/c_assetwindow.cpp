#include "c_assetswindow.h"

#include <framework/imgui/imgui.h>
#include <framework/imgui/imgui_impl_opengl3.h>
#include <framework/imgui/imgui_impl_win32.h>

AssetWindow::AssetWindow()
{

}

AssetWindow::~AssetWindow()
{

}

void AssetWindow::render()
{
	ImGui::Begin("Asset Explorer");
	ImGui::End();
}