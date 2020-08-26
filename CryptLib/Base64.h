#pragma once
#include <codecvt>
#include <string>


class CBase64
{
public:
	std::string Base64_Encode(unsigned char const*, unsigned int len);
	std::string Base64_Decode(std::string const& s);
	std::wstring Utf8_To_Wstring(const std::string& str);
	std::string Wstring_To_Utf8(const std::wstring& str);
	const unsigned char* Convert(const std::string& s);
};

