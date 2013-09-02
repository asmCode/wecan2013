#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

class StringUtils
{
public:
	static std::string UpperCase(const std::string &str);
	static std::string LowerCase(const std::string &str);

	static std::wstring ToWide(const std::string &str);
	static std::string ToNarrow(const std::wstring &str);

	static void Split(const std::string src, const std::string &separator, std::vector<std::string> &result);
};

#endif // STRING_UTILS_H

