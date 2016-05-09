#pragma once

#include "stdafx.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

struct StateSave
{
	const int version = 1;

	std::string levelName;
	int levelPlace;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		int ver = version;

		ar & ver;
		ar & levelName;
		ar & levelPlace;
	}

	void save();
	void load();
};