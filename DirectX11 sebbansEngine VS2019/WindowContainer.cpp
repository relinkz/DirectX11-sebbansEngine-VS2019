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
		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}
