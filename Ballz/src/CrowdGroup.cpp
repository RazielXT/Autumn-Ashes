#include "stdafx.h"
#include "CrowsGroup.h"

using namespace Ogre;

CrowFlight::CrowFlight(int crows, float randomYaw, float mFlightMinTime, Ogre::SceneNode* node)
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
			flightNames.push_back(it->second);
	}

	//init crows
}

CrowFlight::~CrowFlight()
{
	//destroy crows
}

bool CrowFlight::update(Ogre::Real tslf)
{
	//update crows
	//change crow state if available (crow->group->land)
}

void CrowFlight::createPossibleFlight()
{
	//find best free path
}

CrowLanding::CrowLanding(int crows, float radius)
{
	//init crows
}

CrowLanding::~CrowLanding()
{
	//destroy crows
}

bool CrowLanding::update(Ogre::Real tslf)
{
	//update crows
	//change crow state if available (crow->group->flight)
}

bool CrowLanding::acceptsLanding() const
{
	//check if free
}


CrowsGroup::~CrowsGroup()
{
	//destroy vectors
}

bool CrowsGroup::update(Ogre::Real tslf)
{
	//update vectors
}

CrowFlight* CrowsGroup::getPossibleFlight()
{
	//try in all
}

CrowLanding* CrowsGroup::getPossibleLanding()
{
	//try in all
}