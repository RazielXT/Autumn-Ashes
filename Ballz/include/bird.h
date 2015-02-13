#include "stdafx.h"

class Bird
{
public:

    Bird(Ogre::SceneManager * sceneMgr,OgreNewt::World* W, Ogre::Vector3 position,Ogre::SceneNode* tr,OgreNewt::MaterialID* m);
    void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex );

private:

    static int counter;
    Ogre::SceneNode* tracker;
    Ogre::SceneNode* bN;
};