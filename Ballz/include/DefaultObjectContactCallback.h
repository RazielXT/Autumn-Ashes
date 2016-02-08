#include "stdafx.h"
#include <irrKlang.h>
#include "AudioLibrary.h"

class DefaultObjectContactCallback :
    public OgreNewt::ContactCallback
{
public:
    DefaultObjectContactCallback();
    ~DefaultObjectContactCallback(void);

    int onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex );

    void contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex );

private:

	irrklang::ISoundEngine* soundEngine;
	PhysicsAudio audio;
};
