#include "c_dockerwindow.h"
#include <framework/imgui/imgui.h>
#include <framework/framework.h>

DockerWindow::DockerWindow()
{

}

DockerWindow::~DockerWindow()
{

}

void DockerWindow::render()
{
	ImGui::Begin("DOCKSPACE", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	ImGui::SetWindowSize(ImVec2(screen::clientAreaSize.x, screen::clientAreaSize.y), 0);
	ImGui::SetWindowPos(ImVec2(0, 0), 0);
	ImGui::End();
}