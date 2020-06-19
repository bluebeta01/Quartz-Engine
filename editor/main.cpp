#include "c_editor.h"
#include "engine/engine.h"
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_win32.h>
#include <Imgui/imgui_impl_dx11.h>

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

int main()
{
	Editor editor;
	GameWindow::s_imguiWndProcCallback = wndImguiProc;

	while (!GameWindow::s_isTerminating)
	{
		editor.tick();
		editor.render();
	}

	editor.terminate();
}