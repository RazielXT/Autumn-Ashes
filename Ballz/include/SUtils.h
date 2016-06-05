#pragma once

#include "stdafx.h"

using namespace Ogre;

namespace SUtils
{
inline bool startsWith(const std::string str, const std::string with)
{
	if (str.length() < with.length())
		return false;

	auto sub = str.substr(0, with.length());

	return with == sub;
}

inline bool endsWith(const std::string str, const std::string with)
{
	if (str.length() < with.length())
		return false;

	auto sub = str.substr(str.length() - with.length(), with.length());

	return with == sub;
}

inline std::string strtok_str(std::string& txt, char delim)
{
	auto dPos = txt.find_first_of(delim);
	std::string ret = txt;

	if (dPos != std::string::npos)
	{
		ret.erase(dPos, std::string::npos);
		txt.erase(0, dPos + 1);
	}
	else
		txt.clear();

	return ret;
}

}