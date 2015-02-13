#include "stdafx.h"
#include <irrKlang.h>

class DefaultObjectContactCallback :
    public OgreNewt::ContactCallback
{
public:
    DefaultObjectContactCallback( irrklang::ISoundEngine* soundEngine );
    ~DefaultObjectContactCallback(void);

    int onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex );

    void contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex );

private:
    irrklang::ISoundEngine* sEngine;
};
