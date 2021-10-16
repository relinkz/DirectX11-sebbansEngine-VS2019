#pragma once

#include <chrono>

class Timer
{
public:
	Timer();
	double GetMilisecondsElapsed();
	void Restart();
	bool Stop();
	bool Start();

private:
	bool m_isRunning;
#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	std::chrono::time_point<std::chrono::steady_clock> m_stop;
#else
	std::chrono::time_point<std::chrono::system_clock> m_start;
	std::chrono::time_point<std::chrono::system_clock> m_stop;
#endif
};
