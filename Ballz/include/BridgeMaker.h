#ifndef BRIDGEMKR_H
#define BRIDGEMKR_H

#include "stdafx.h"

class BridgeMaker
{
public:

	BridgeMaker(Ogre::SceneManager * sceneMgr,OgreNewt::World* W);
	~BridgeMaker ();

	/*looseness- 1=very loose, 0=very tight */
	void makeBridge(Ogre::Vector3 position, Ogre::Vector3 target, Ogre::Real scale, Ogre::Real looseness, int matType, unsigned int visibilityFlag = 1, Ogre::String material = defaultMat);
	void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex);

private:

	static Ogre::String defaultMat;
	OgreNewt::World* mWorld;
	Ogre::SceneManager * mSceneMgr;

};

#endif