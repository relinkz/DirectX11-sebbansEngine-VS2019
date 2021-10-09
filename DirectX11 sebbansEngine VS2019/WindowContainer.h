#pragma once
#include "RenderWindow.h"
#include "Keyboard/Keyboard.h"

class WindowContainer
{
public:
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	std::unique_ptr<RenderWindow> m_renderWindow;
	Keyboard m_keyboard;
};

