#include "Timer.h"

Timer::Timer() :
	m_isRunning(false)
{
	m_start = std::chrono::high_resolution_clock::now();
	m_stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMilisecondsElapsed()
{
	auto elapsed = std::chrono::duration<double, std::milli>(m_stop - m_start);
	if (m_isRunning)
	{
		elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_start);
	}

	return elapsed.count();
}

void Timer::Restart()
{
	m_isRunning = true;
	m_start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
	if (!m_isRunning)
	{
		return false;
	}
	m_stop = std::chrono::high_resolution_clock::now();
	m_isRunning = false;

	return true;
}

bool Timer::Start()
{
	if (m_isRunning)
	{
		return false;
	}
	m_start = std::chrono::high_resolution_clock::now();
	m_isRunning = true;

	return true;
}
