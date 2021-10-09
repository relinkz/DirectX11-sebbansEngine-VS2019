#include "WindowContainer.h"

static bool wasKeyAlreadyPressed(LPARAM lParam)
{
	return lParam & 0x40000000;
}

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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

	default:
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}
