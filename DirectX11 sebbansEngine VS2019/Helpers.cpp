#include "Helpers.h"

std::wstring helpers::strings::StringToWide(const std::string& str)
{
	std::wstring wide_str(str.begin(), str.end());
	return wide_str;
}
