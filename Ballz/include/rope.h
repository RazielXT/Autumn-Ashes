#include "stdafx.h"

class Rope
{
public:

    Rope(Ogre::SceneManager * sceneMgr,OgreNewt::World* W, Ogre::Vector3 position, int parts,Ogre::Real scale);
    ~Rope();

private:

    static int counter;
    OgreNewt::World* mWorld;
    Ogre::SceneManager * mSceneMgr;

};