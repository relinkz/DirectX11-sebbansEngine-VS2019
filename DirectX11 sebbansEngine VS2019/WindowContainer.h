#pragma once
#include "RenderWindow.h"

class WindowContainer
{
public:
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	std::unique_ptr<RenderWindow> m_renderWindow;
};

