#include "stdafx.h"
#include "Progress.h"

#include <iostream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

const char* savePath = ".\\gstats";

void GameProgress::save()
{
	std::ofstream ofs(savePath, std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << *this;
}

void GameProgress::load()
{
	std::ifstream ifs(savePath, std::ios::binary);

	if (ifs.good())
	{
		boost::archive::binary_iarchive ia(ifs);
		ia >> *this;
	}
}

void LevelProgress::update(Ogre::Vector3& currentPos)
{
	auto c = checkpoints.size();
	for (size_t i = 0; i < c; i++)
	{
		if (checkpoints[i].pos.squaredDistance(currentPos) < c.radiusSq)
			currentCheckpoint = i;
	}
}
