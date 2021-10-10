#pragma once
#include "Helpers.h"
#include <Windows.h>

namespace errorlogger
{
	void Log(const std::string& message);
	void Log(HRESULT hr, const std::string& message);
	void Log(HRESULT hr, const std::wstring& message);
}


