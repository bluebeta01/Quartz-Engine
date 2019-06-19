#include "c_renderwindow.h"
#include <framework/imgui/imgui.h>
#include <framework/imgui/imgui_impl_opengl3.h>
#include <framework/imgui/imgui_impl_win32.h>

RenderWindow::RenderWindow()
{
	sceneFrameBuffer = new FrameBuffer(1280,720);
}

RenderWindow::~RenderWindow()
{
	delete sceneFrameBuffer;
}

void RenderWindow::render()
{
	ImGui::Begin("Render Window", 0, ImGuiWindowFlags_NoScrollbar);

	if (lastWindowSizeX != ImGui::GetWindowSize().x || lastWindowSizeY != ImGui::GetWindowSize().y)
	{
		lastWindowSizeX = ImGui::GetWindowSize().x;
		lastWindowSizeY = ImGui::GetWindowSize().y;
		delete sceneFrameBuffer;
		sceneFrameBuffer = new FrameBuffer(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		glViewport(0, 0, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		camera->aspect = ImGui::GetWindowSize().x / (ImGui::GetWindowSize().y);
	}

	windowSize.x = ImGui::GetWindowSize().x;
	windowSize.y = ImGui::GetWindowSize().y;

	localCursorPosition.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - 8;
	localCursorPosition.y = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - 8;
	

	ImGui::Image((ImTextureID)sceneFrameBuffer->glFrameBufferColor, ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 1));
	
	amLooking = ImGui::IsItemHovered() && ImGui::IsMouseDown(1);
	leftClicked = ImGui::IsItemHovered() && ImGui::IsMouseClicked(0);
	ImGui::End();
}