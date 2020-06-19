#include "c_editor.h"
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_win32.h>
#include <Imgui/imgui_impl_dx11.h>

Entity* plane;

// Our state
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

bool wndImguiProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::GetCurrentContext() == NULL)
		return false;

	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
	{
		int button = 0;
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
		if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
			::SetCapture(GameWindow::s_windowHandle);
		io.MouseDown[button] = true;
		return 0;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	{
		int button = 0;
		if (msg == WM_LBUTTONUP) { button = 0; }
		if (msg == WM_RBUTTONUP) { button = 1; }
		if (msg == WM_MBUTTONUP) { button = 2; }
		if (msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
		io.MouseDown[button] = false;
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == GameWindow::s_windowHandle)
			::ReleaseCapture();
		return 0;
	}
	case WM_MOUSEWHEEL:
		io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		return 0;
	case WM_MOUSEHWHEEL:
		io.MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return 0;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacterUTF16((unsigned short)wParam);
		return 0;
	/*case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT && ImGui_ImplWin32_UpdateMouseCursor())
			return 1;
		return 0;
	case WM_DEVICECHANGE:
		if ((UINT)wParam == DBT_DEVNODES_CHANGED)
			g_WantUpdateHasGamepad = true;
		return 0;
	case WM_DISPLAYCHANGE:
		g_WantUpdateMonitors = true;
		return 0;*/
	}
	return 0;
}

void setStyle()
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

Editor::Editor()
{
	GameWindow::initialize(1280, 720);
	m_engine = new Engine();

	m_camera = m_engine->m_world.createEntity("Camera");
	m_camera->addComponent(new CameraComponent());

	dynamic_cast<CameraComponent*>(m_camera->getComponent(Component::COMPONENT_TYPE_CAMERA_COMPONENT))->m_camera
		= Camera(glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::radians(40.0f), 0.1f, 1000.0f,
		(float)GameWindow::s_renderAreaSize.x / (float)GameWindow::s_renderAreaSize.y);

	m_gizmo.initialize(&m_engine->m_renderer, &dynamic_cast<CameraComponent*>(m_camera->getComponent(Component::COMPONENT_TYPE_CAMERA_COMPONENT))->m_camera, &m_engine->m_world);

	//This model should remain loaded at ALL TIMES
	m_engine->m_renderer.getModel("debugplane");


	plane = m_engine->m_world.createEntity("plane", Transform(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0, 180, 0), glm::vec3(1.0, 1.0, 1.0)));
	plane->addComponent(new RenderComponent(m_engine->m_renderer.getModel("debugplane")));
	Entity* ball = m_engine->m_world.createEntity("ball", Transform(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(1.0, 1.0, 1.0)));
	ball->addComponent(new RenderComponent(m_engine->m_renderer.getModel("woodball")));
	plane->addChild(ball);


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
	ImGui_ImplDX11_Init(m_engine->m_renderer.m_dx11Renderer.m_device,
		m_engine->m_renderer.m_dx11Renderer.m_deviceContext);
	GameWindow::s_imguiWndProcCallback = wndImguiProc;


}

void Editor::tick()
{
	GameWindow::tick();


	m_engine->tick();
	cameraMovement();
	m_gizmo.update();

	plane->transform.rotation.x += 0.3f;
}

void Editor::render()
{
	m_engine->m_renderer.setCamera(dynamic_cast<CameraComponent*>(m_camera->getComponent(Component::COMPONENT_TYPE_CAMERA_COMPONENT))->m_camera);
	m_engine->m_renderer.render();


	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow(&show_demo_window);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	m_engine->m_renderer.present();
}

void Editor::terminate()
{
	m_engine->terminate();
}

void Editor::cameraMovement()
{
	float cameraSpeed = 0.3f;
	float cameraSensitivity = 0.1f;

	if (input::isMouseButtonDown(input::MOUSEBUTTON_RIGHT))
	{
		input::cursorLock = true;
		m_camera->transform.rotation.x -= input::cursorDelta.y * cameraSensitivity;
		m_camera->transform.rotation.y -= input::cursorDelta.x * cameraSensitivity;

		glm::vec3 forward = dynamic_cast<CameraComponent*>(m_camera->getComponent(Component::COMPONENT_TYPE_CAMERA_COMPONENT))->m_camera.getForward();
		glm::vec3 right = dynamic_cast<CameraComponent*>(m_camera->getComponent(Component::COMPONENT_TYPE_CAMERA_COMPONENT))->m_camera.getRight();

		if (input::isKeyDown('W'))
		{
			m_camera->transform.position += forward * cameraSpeed;
		}
		if (input::isKeyDown('S'))
		{
			m_camera->transform.position -= forward * cameraSpeed;
		}
		if (input::isKeyDown('A'))
		{
			m_camera->transform.position += right * cameraSpeed;
		}
		if (input::isKeyDown('D'))
		{
			m_camera->transform.position -= right * cameraSpeed;
		}
	}
	else
	{
		input::cursorLock = false;
	}
}