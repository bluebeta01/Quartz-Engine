#include <engine/engine.h>
#include "c_windowmanager.h"

RenderWindow* WindowManager::s_renderWindow;

void WindowManager::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	ImGui_ImplWin32_EnableDpiAwareness();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	setStyle();
	ImGui_ImplWin32_Init(GameWindow::s_windowHandle);
	ImGui_ImplDX11_Init(device, deviceContext);
	
	s_renderWindow = new RenderWindow();
	s_renderWindow->show();
}

void WindowManager::renderWindows()
{
	ImGuiContext* i = ImGui::GetCurrentContext();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	s_renderWindow->render();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void WindowManager::setStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.83f, 0.83f, 0.83f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.80f, 0.80f, 0.80f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.77f, 0.77f, 0.77f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.67f, 0.67f, 0.67f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.49f, 0.49f, 0.49f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.37f, 0.37f, 0.37f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(1.00f, 1.00f, 1.00f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.73f, 0.73f, 0.73f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.38f, 0.38f, 0.38f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.39f, 0.39f, 0.39f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.49f, 0.49f, 0.49f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.53f, 0.53f, 0.53f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.80f);
}