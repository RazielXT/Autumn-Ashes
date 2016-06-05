#pragma once

#include <string>
#include <map>

struct FloatsMap
{
	void loadFile(std::string filename);
	float get(std::string& name, float defaultValue = 0.1f);

	std::map<std::string, float> values;
};