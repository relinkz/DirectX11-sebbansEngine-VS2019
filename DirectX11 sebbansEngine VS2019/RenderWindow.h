#pragma once
#include "ErrorLogger.h"
#include <memory>

class RenderWindow
{
public:
	bool ProcessMessages();

	RenderWindow(HINSTANCE hInstance, const std::string& wTitle, const std::string& wClassName, const uint32_t width, const uint32_t height);
	~RenderWindow();

private:
	void m_CreateWindowHandle();
	void m_RegisterWindowClass();
	HWND m_handle;
	HINSTANCE m_instance;
	std::unique_ptr<std::string> m_windowTitle;
	std::unique_ptr<std::wstring> m_windowTitleWide;
	std::unique_ptr<std::string> m_windowClassName;
	std::unique_ptr<std::wstring> m_windowClassNameWide;
	std::uint32_t m_windowWidth;
	std::uint32_t m_windowHeight;
};

