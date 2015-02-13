#include "stdafx.h"

class Baloon
{
public:

    Baloon(Ogre::SceneManager * sceneMgr,OgreNewt::World* W, Ogre::Vector3 position);
    ~Baloon ();
    int getID()
    {
        return id;
    }
    void setTracker(Ogre::SceneNode* t)
    {
        tracker=t;
    }
    void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex );

private:
    static int counter;
    int id;
    Ogre::Entity* ropeEnt;
    Ogre::Entity* baloonEnt;
    Ogre::SceneNode* baloonNode;
    Ogre::SceneNode* node;
    OgreNewt::Body* baloonB;
    OgreNewt::Body* ropeB;
    OgreNewt::World* mWorld;
    Ogre::SceneManager * mSceneMgr;
    Ogre::SceneNode* tracker;
};