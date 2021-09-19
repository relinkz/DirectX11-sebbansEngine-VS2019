#include "RenderWindow.h"

bool RenderWindow::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(
		&msg,
		m_handle,
		0,
		0,
		PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(m_handle))
		{
			m_handle = nullptr;
			UnregisterClass(m_windowClassNameWide->c_str(), m_instance);

			return false;
		}
	}

	return true;
}

RenderWindow::RenderWindow(HINSTANCE hInstance, const std::string& wTitle, const std::string& wClassName, const uint32_t width, const uint32_t height)
	: m_handle(nullptr),
	m_instance(hInstance),
	m_windowTitle(std::make_unique<std::string>(wTitle)),
	m_windowTitleWide(std::make_unique<std::wstring>(helpers::strings::StringToWide(wTitle))),
	m_windowClassName(std::make_unique<std::string>(wClassName)),
	m_windowClassNameWide(std::make_unique<std::wstring>(helpers::strings::StringToWide(wTitle))),
	m_windowWidth(width),
	m_windowHeight(height)
{

	m_RegisterWindowClass();
	m_CreateWindowHandle();
}

RenderWindow::~RenderWindow()
{
	m_windowTitle.release();
	m_windowClassName.release();

	if (m_handle != nullptr)
	{
		UnregisterClass(m_windowClassNameWide->c_str(), m_instance);
		DestroyWindow(m_handle);
	}
}

void RenderWindow::m_CreateWindowHandle()
{
	m_handle = CreateWindowEx(
		0,
		m_windowClassNameWide->c_str(),
		m_windowTitleWide->c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0,
		0,
		m_windowWidth,
		m_windowHeight,
		nullptr,
		nullptr,
		m_instance,
		nullptr);

	if (m_handle != nullptr)
	{
		ShowWindow(m_handle, SW_SHOW);
		SetForegroundWindow(m_handle);
		SetFocus(m_handle);
	}
	else
	{
		errorlogger::Log(GetLastError(), "CreateWindowEx failed for window: " + *m_windowTitle);
	}
}

void RenderWindow::m_RegisterWindowClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_instance;
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_windowClassNameWide->c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}
