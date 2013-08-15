#include "StringUtils.h"
#include <algorithm>
#include <ctype.h>

std::string StringUtils::UpperCase(const std::string &str)
{
	std::string result;
	result.resize(str.length());
	std::transform(str.begin(), str.end(), result.begin(), toupper);
	return result;
}

std::string StringUtils::LowerCase(const std::string &str)
{
	std::string result;
	result.resize(str.length());
	std::transform(str.begin(), str.end(), result.begin(), tolower);
	return result;
}

std::wstring StringUtils::ToWide(const std::string &str)
{
	return std::wstring(str.begin(), str.end());
}

std::string StringUtils::ToNarrow(const std::wstring &str)
{
	return std::string(str.begin(), str.end());
}

