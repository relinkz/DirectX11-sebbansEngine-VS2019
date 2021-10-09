#pragma once
#include "RenderWindow.h"
#include "Keyboard/Keyboard.h"
#include "Mouse/Mouse.h"

class WindowContainer
{
public:
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	std::unique_ptr<RenderWindow> m_renderWindow;
	Keyboard m_keyboard;
	Mouse m_mouse;
};

