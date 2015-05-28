#include "stdafx.h"
#include "CrowsGroup.h"

using namespace Ogre;

CrowFlight::CrowFlight(int num_crows, float randomYaw, float mFlightMinTime, Ogre::SceneNode* node)
{
	randomYawMax = randomYaw;
	flightMinTime = mFlightMinTime;

	//init paths
	auto animNode = Global::mSceneMgr->getSceneNode(node->getName() + "Anim");
	auto anims = Global::mSceneMgr->getAnimations();

	for (auto it = anims.cbegin(); it != anims.cend(); it++)
	{
		if (it->second->getNumNodeTracks()==0)
			continue;

		auto track = it->second->getNodeTrack(0);
		
		if (track->getAssociatedNode() == animNode)
			flightAnims.push_back(it->second);
	}

	//init crows
	for (int i = 0; i < num_crows; i++)
	{
		addCrow(new Crow());
	}
}

CrowFlight::~CrowFlight()
{
	//destroy crows
	for (auto c : crows)
	{
		delete c;
	}
}

bool CrowFlight::update(Ogre::Real tslf)
{
	//update crows
	for (auto c : crows)
	{
		c->update(tslf);
	}

	//change crow state if available (crow->group->land)
	auto c = crows.begin();
	while (c != crows.end())
	{
		if ((*c)->readyToLand())
		{
			CrowLanding* l = group->getPossibleLanding();

			if (l)
			{
				l->addCrow(*c);
				crows.erase(c);
				continue;
			}			
		}

		c++;
	}
}

void CrowFlight::addCrow(Crow* crow)
{
	crow->flyTo(getPossibleFlight());
	crows.push_back(crow);	
}

Ogre::Animation* CrowFlight::getPossibleFlight()
{
	//find best free path
	int rand = (int)Ogre::Math::RangeRandom(0, flightAnims.size() - 0.01f);
	return flightAnims[rand];
}

CrowLanding::CrowLanding(int num_crows, float mRadius, Ogre::SceneNode* node)
{
	radius = mRadius;
	pos = node->_getDerivedPosition();

	//init crows
	for (int i = 0; i < num_crows && acceptsLanding(); i++)
	{
		addCrow(new Crow());
	}
}

CrowLanding::~CrowLanding()
{
	//destroy crows
	for (auto c : crows)
	{
		delete c;
	}
}

bool CrowLanding::update(Ogre::Real tslf)
{
	//update crows
	for (auto c : crows)
	{
		c->update(tslf);
	}

	//change crow state if available (crow->group->flight)
	auto c = crows.begin();
	while (c != crows.end())
	{
		if ((*c)->readyToFly())
		{
			CrowFlight* f = group->getPossibleFlight();

			if (f)
			{
				f->addCrow(*c);
				crows.erase(c);
				continue;
			}
		}

		c++;
	}
}

bool CrowLanding::acceptsLanding() const
{
	//check if free
	return (crows.size <= radius);
}

void CrowLanding::addCrow(Crow* crow)
{
	Vector3 lpos(pos);

	crow->landTo(lpos);
	crows.push_back(crow);
}

CrowsGroup::~CrowsGroup()
{
	//destroy vectors
	for (auto c : landings)
	{
		delete c;
	}

	for (auto c : flights)
	{
		delete c;
	}
}

bool CrowsGroup::update(Ogre::Real tslf)
{
	//update vectors
	for (auto l : landings)
	{
		l->update(tslf);
	}

	for (auto f : flights)
	{
		f->update(tslf);
	}
}

CrowFlight* CrowsGroup::getPossibleFlight()
{
	//try in all
	/*for (auto f : flights)
	{
		return f;
	}*/

	int rand = (int)Ogre::Math::RangeRandom(0, flights.size() - 0.01f);
	return flights[rand];
}

CrowLanding* CrowsGroup::getPossibleLanding()
{
	//try in all
	for (auto l : landings)
	{
		if (l->acceptsLanding()) return l;
	}

	return nullptr;
}