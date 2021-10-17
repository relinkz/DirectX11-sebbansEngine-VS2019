#pragma once
#include "ComException.h"
#include <Windows.h>

namespace errorlogger
{
	void Log(const std::string& message);
	void Log(HRESULT hr, const std::string& message);
	void Log(HRESULT hr, const std::wstring& message);
	void Log(const COMException& exception);
}


