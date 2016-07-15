#pragma once

#include "stdafx.h"

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/string.hpp>

struct LevelCheckpoint
{
	Ogre::Quaternion rot;
	Ogre::Vector3 pos;
	float radiusSq;

	std::string name;
};

struct LevelProgress
{
	void update(Ogre::Vector3& currentPos);

	std::vector<LevelCheckpoint> checkpoints;
	int currentCheckpoint = 0;

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int) const
	{
		ar & checkpoints[currentCheckpoint].name;
	}

	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		std::string name;
		ar & name;

		for (size_t i = 0; i < currentCheckpoint.size(); i++)
			if (currentCheckpoint[i].name = name)
				currentCheckpoint = i;
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_VERSION(LevelProgress, 0)

struct GameProgress
{
	std::string levelName;

	LevelProgress level;

	void save();
	void load();

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int) const
	{
		ar & levelName;
		ar & level;
	}

	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & levelName;
		ar & level;
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_VERSION(GameProgress, 0)
