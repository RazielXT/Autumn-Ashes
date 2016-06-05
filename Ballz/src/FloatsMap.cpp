#include "stdafx.h"
#include "FloatsMap.h"
#include <fstream>

bool emptyChar(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

std::string ripWord(std::string& in)
{
	size_t idBegin = 0;
	while (idBegin<in.length() && emptyChar(in[idBegin]))
		idBegin++;

	size_t count = 0;
	while ((count + idBegin)<in.length() && !emptyChar(in[idBegin + count]))
		count++;

	if (count)
	{
		auto word = in.substr(idBegin, count);
		in.erase(in.begin(), in.begin() + idBegin + count);
		return word;
	}
	else
		return "";
}

void FloatsMap::loadFile(std::string filename)
{
	values.clear();
	std::ifstream file(filename);

	if (file.good())
	{
		std::string line;

		while (std::getline(file,line))
		{
			auto name = ripWord(line);
			auto value = ripWord(line);

			if (!value.empty())
				values[name] = std::stof(value);
		}
	}
}

float FloatsMap::get(std::string& name, float defaultValue /*= 0.1f*/)
{
	if (values.find(name) != values.end())
		return values[name];
	else
		return defaultValue;
}

