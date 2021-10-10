#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, const std::string& wTitle, const std::string& wClassName, const uint32_t width, const uint32_t height)
{
	m_renderWindow = std::make_unique<RenderWindow>();
	if (!m_renderWindow->Initialize(this, hInstance, wTitle, wClassName, width, height))
	{
		return false;
	}

	if (!m_gfx.Initialize(m_renderWindow->GetHWindow(), width, height))
	{
		return false;
	}

	return true;
}

bool Engine::ProcessMessages()
{
	if (!m_renderWindow->ProcessMessages())
	{
		return false;
	}
	return true;
}

void Engine::Update()
{
	while (!m_keyboard.CharBufferIsEmpty())
	{
		auto ch = m_keyboard.ReadChar();
	}

	while (!m_keyboard.KeyBufferIsEmpty())
	{
		auto keyboardEvent = m_keyboard.ReadKey();
	}

	while (!m_mouse.EventBufferIsEmpty())
	{
		auto me = m_mouse.ReadEvent();
	}
}
