#include "c_gui.h"
#include <framework/framework.h>



GUI::GUI()
{
	dockerWindow = new DockerWindow();
	assetWindow = new AssetWindow();
	propertyWindow = new PropertyWindow();
	renderWindow = new RenderWindow();

	ImGui_ImplWin32_EnableDpiAwareness();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	ImGui_ImplWin32_Init(screen::windowHandle);
	ImGui_ImplOpenGL3_Init("#version 400");
}

GUI::~GUI()
{
	delete dockerWindow;
}

void GUI::drawWindow()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1));
	dockerWindow->render();
	assetWindow->render();
	propertyWindow->render();
	renderWindow->render();
	ImGui::PopStyleColor();



	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}