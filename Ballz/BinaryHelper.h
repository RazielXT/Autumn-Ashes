#pragma once
#include <vector>

enum class VarType
{
	FLOAT,
	INT,
	STRING,
	ARRAY
};

struct BinaryWriter
{
	std::vector<char> data;

	void put(std::string val);
	void put(int val);
	void put(float val);
};

void BinaryWriter::put(std::string val)
{
	data.push_back((char)VarType::STRING);
	int s = (int)val.size();
	data.insert(data.end(), &s, &s + sizeof (int));
	data.insert(data.end(),val.data(), val.data() + val.size());
}

void BinaryWriter::put(int val)
{
	data.push_back((char)VarType::INT);
	data.insert(data.end(), &val, &val + sizeof(val));
}

void BinaryWriter::put(float val)
{
	data.push_back((char)VarType::FLOAT);
	data.insert(data.end(), &val, &val + sizeof(val));
}

//////////-------------------------------------------------

struct BinaryReader
{
	BinaryReader(std::vector<char> buffer) : buffer(buffer.data()), size(buffer.size())
	{
	}

	bool get(std::string& val);
	bool get(int& val);
	bool get(float& val);

	const char* buffer = nullptr;
	size_t size = 0;
	size_t pos = 0;
};

bool BinaryReader::get(std::string& val)
{
	if (pos >= size || buffer[pos] != (char)VarType::STRING)
		return false;

	pos++;

	int s = *reinterpret_cast<const int*>(buffer + pos);
	pos += sizeof s;

	val = *reinterpret_cast<const char*>(buffer + pos);
	pos += s;

	return true;
}

bool BinaryReader::get(int& val)
{
	if (pos >= size || buffer[pos] != (char)VarType::INT)
		return false;

	val = *reinterpret_cast<const int*>(buffer + pos);
	pos += sizeof val;

	return true;
}

bool BinaryReader::get(float& val)
{
	if (pos >= size || buffer[pos] != (char)VarType::FLOAT)
		return false;

	val = *reinterpret_cast<const float*>(buffer + pos);
	pos += sizeof val;

	return true;
}
