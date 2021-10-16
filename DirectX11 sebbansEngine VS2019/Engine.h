#pragma once
#include "WindowContainer.h"
#include "Timer.h"

class Engine : public WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, const std::string& wTitle, const std::string& wClassName, const uint32_t width, const uint32_t height);
	bool ProcessMessages();
	void Update();
	void RenderFrame();

private:
	std::unique_ptr<Timer> m_timer;
};

