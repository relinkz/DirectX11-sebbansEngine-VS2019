#pragma once

#include <comdef.h>
#include "Helpers.h"

#define COM_ERROR_IF_FAILED( hr, msg ) if( FAILED( hr ) ) throw COMException( hr, msg, __FILE__, __FUNCTION__, __LINE__ )

class COMException
{
public:
	COMException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line)
	{
		_com_error error(hr);
		m_whatMsg = L"Msg: " + helpers::strings::StringToWide(msg + "\n");
		m_whatMsg += error.ErrorMessage();
		m_whatMsg += L"\nFile: " + helpers::strings::StringToWide(file);
		m_whatMsg += L"\nFunction: " + helpers::strings::StringToWide(function);
		m_whatMsg += L"\nLine: " + helpers::strings::StringToWide(std::to_string(line));
	}

	const wchar_t* what() const
	{
		return m_whatMsg.c_str();
	}

private:
	std::wstring m_whatMsg;
};