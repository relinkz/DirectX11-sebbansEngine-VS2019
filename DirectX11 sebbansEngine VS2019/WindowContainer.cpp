#include "WindowContainer.h"

static bool wasKeyAlreadyPressed(LPARAM lParam)
{
	return lParam & 0x40000000;
}

WindowContainer::WindowContainer()
{
	static bool raw_input_initialized = false;
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01; // Mouse
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			errorlogger::Log(GetLastError(), "Failed to register raw input devices");
			exit(-1);
		}

		raw_input_initialized = true;
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
	{
		return true;
	}

	// rerouted from RenderWindow
	switch (uMsg)
	{
		/*
		MOUSE HANDLING
		*/
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (m_keyboard.IsKeysAutoRepeat())
		{
			m_keyboard.OnKeyPressed(keycode);
		}
		else
		{
			if (!wasKeyAlreadyPressed(lParam))
			{
				m_keyboard.OnKeyPressed(keycode);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		m_keyboard.OnKeyReleased(keycode);
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		if (m_keyboard.IsCharsAutoRepeat())
		{
			m_keyboard.OnChar(ch);
		}
		else
		{
			if (!wasKeyAlreadyPressed(lParam))
			{
				m_keyboard.OnChar(ch);
			}
		}
		return 0;
	}
	/*
	MOUSE HANDLING
	*/
	case WM_MOUSEMOVE:
	{
		MousePoint mousePos = { LOWORD(lParam), HIWORD(lParam) };
		m_mouse.OnMouseMove(mousePos);

		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		MousePoint mousePos = { LOWORD(lParam), HIWORD(lParam) };
		m_mouse.OnLeftPressed(mousePos);

		return 0;
	}

	case WM_RBUTTONDOWN:
	{
		MousePoint mousePos = { LOWORD(lParam), HIWORD(lParam) };
		m_mouse.OnRightPressed(mousePos);

		return 0;
	}

	case WM_MBUTTONDOWN:
	{
		MousePoint mousePos = { LOWORD(lParam), HIWORD(lParam) };
		m_mouse.OnMidPressed(mousePos);

		return 0;
	}

	case WM_LBUTTONUP:
	{
		MousePoint mousePos = { LOWORD(lParam), HIWORD(lParam) };
		m_mouse.OnLeftReleased(mousePos);

		return 0;
	}

	case WM_RBUTTONUP:
	{
		MousePoint mousePos = { LOWORD(lParam), HIWORD(lParam) };
		m_mouse.OnRightReleased(mousePos);

		return 0;
	}

	case WM_MBUTTONUP:
	{
		MousePoint mousePos = { LOWORD(lParam), HIWORD(lParam) };
		m_mouse.OnMidReleased(mousePos);

		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		MousePoint wheelPoint = { LOWORD(lParam), HIWORD(lParam) };

		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			m_mouse.OnWheelUp(wheelPoint);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			m_mouse.OnWheelDown(wheelPoint);
		}
	}
	case WM_INPUT:
	{
		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					MousePoint pos;
					pos.x = raw->data.mouse.lLastX;
					pos.y = raw->data.mouse.lLastY;

					m_mouse.OnMouseMoveRaw(pos);
				}
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	default:
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}
