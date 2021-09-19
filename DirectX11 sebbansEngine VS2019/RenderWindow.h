#pragma once
#include "ErrorLogger.h"
#include <memory>

class WindowContainer;

class RenderWindow
{
public:
	bool ProcessMessages();

	bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, const std::string& wTitle, const std::string& wClassName, const uint32_t width, const uint32_t height);
	~RenderWindow();

private:
	bool m_CreateWindowHandle(WindowContainer* pWindowContainer);
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

