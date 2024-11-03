// =======================================================================
//
// Extreme Snail Manager
//
// (c) Paul Alan Freshney 1995-2024
// paul@freshney.org
//
// https://github.com/MaximumOctopus/ExtremeSnailManager
//
// =======================================================================

#pragma once


#include <string>


namespace Formatting
{
	[[nodiscard]] std::wstring AddLeadingSpace(std::wstring, int);
    [[nodiscard]] std::wstring AddTrailingSpace(std::wstring, int);

    std::wstring LDToStr(long double);

	std::string to_utf8(const std::wstring& str);
	std::string to_utf8(const wchar_t* buffer, int len);
}