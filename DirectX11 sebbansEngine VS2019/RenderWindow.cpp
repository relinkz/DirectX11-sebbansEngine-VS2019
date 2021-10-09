#include "WindowContainer.h"

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

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, const std::string& wTitle, const std::string& wClassName, const uint32_t width, const uint32_t height)
{
	m_handle = nullptr;
	m_instance = hInstance;
	m_windowTitle = std::make_unique<std::string>(wTitle);
	m_windowTitleWide = std::make_unique<std::wstring>(helpers::strings::StringToWide(wTitle));
	m_windowClassName = std::make_unique<std::string>(wClassName);
	m_windowClassNameWide = std::make_unique<std::wstring>(helpers::strings::StringToWide(wTitle));
	m_windowWidth = width;
	m_windowHeight = height;

	m_RegisterWindowClass();
	if (!m_CreateWindowHandle(pWindowContainer))
	{
		return false;
	}

	return true;
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

LRESULT CALLBACK handleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		return 0;
	}
	default:
	{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);

		if (!pWindow)
		{
			errorlogger::Log("Critical error: Pointer to window container is null during WM_NCCREATE");
			exit(-1);
		}

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(handleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

bool RenderWindow::m_CreateWindowHandle(WindowContainer* pWindowContainer)
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
		pWindowContainer);

	if (m_handle == nullptr)
	{
		errorlogger::Log(GetLastError(), "CreateWindowEx failed for window: " + *m_windowTitle);
		return false;
	}

	ShowWindow(m_handle, SW_SHOW);
	SetForegroundWindow(m_handle);
	SetFocus(m_handle);
	return false;
}

void RenderWindow::m_RegisterWindowClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
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
