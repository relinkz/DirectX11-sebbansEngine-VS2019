#include "ErrorLogger.h"
#include <comdef.h>

void errorlogger::Log(const std::string& message)
{
	std::string error_msg = "Error: " + message;
	MessageBoxA(NULL, error_msg.c_str(), "ERROR", MB_ICONERROR);
}

void errorlogger::Log(HRESULT hr, const std::string& message)
{
	_com_error error(hr);
	std::wstring error_msg = L"Error: " + helpers::strings::StringToWide(message) + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_msg.c_str(), L"ERROR", MB_ICONERROR);
}

void errorlogger::Log(HRESULT hr, const std::wstring& message)
{
	_com_error error(hr);
	std::wstring error_msg = L"Error: " + message + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_msg.c_str(), L"ERROR", MB_ICONERROR);
}
