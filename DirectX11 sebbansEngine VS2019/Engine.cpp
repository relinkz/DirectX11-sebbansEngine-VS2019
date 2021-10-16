#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, const std::string& wTitle, const std::string& wClassName, const uint32_t width, const uint32_t height)
{
	m_timer = std::make_unique<Timer>();
	m_timer->Start();

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
	float dt = m_timer->GetMilisecondsElapsed();
	m_timer->Restart();

	const float cameraSpeed = 0.02f * dt;
	const float cameraTurnSpeed = 0.01f *dt;

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

		if (m_mouse.IsRightDown())
		{
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
				m_gfx.gameCamera->AdjustRotation(me.GetPos().y * cameraTurnSpeed, me.GetPos().x * cameraTurnSpeed, 0);
			}
		}
	}

	if (m_keyboard.KeyIsPressed('W'))
	{
		auto forwardVec = m_gfx.gameCamera->GetForwardVector();
		m_gfx.gameCamera->AdjustPosition(forwardVec * cameraSpeed);
	}

	if (m_keyboard.KeyIsPressed('S'))
	{
		auto backVec = m_gfx.gameCamera->GetBackwardVector();
		m_gfx.gameCamera->AdjustPosition(backVec * cameraSpeed);
	}

	if (m_keyboard.KeyIsPressed('A'))
	{
		auto leftVec = m_gfx.gameCamera->GetLeftVector();
		m_gfx.gameCamera->AdjustPosition(leftVec * cameraSpeed);
	}

	if (m_keyboard.KeyIsPressed('D'))
	{
		auto rightVec = m_gfx.gameCamera->GetRightVector();
		m_gfx.gameCamera->AdjustPosition(rightVec * cameraSpeed);
	}

	if (m_keyboard.KeyIsPressed(VK_SPACE))
	{
		m_gfx.gameCamera->AdjustPosition(0.0f, cameraSpeed, 0.0f);
	}

	if (m_keyboard.KeyIsPressed('Z'))
	{
		m_gfx.gameCamera->AdjustPosition(0.0f, -cameraSpeed, 0.0f);
	}
}

void Engine::RenderFrame()
{
	m_gfx.RenderFrame();
}
