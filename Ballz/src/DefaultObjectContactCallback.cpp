#include "stdafx.h"
#include "DefaultObjectContactCallback.h"
#include "bodyUserData.h"

DefaultObjectContactCallback::DefaultObjectContactCallback( irrklang::ISoundEngine* soundEngine  ) : OgreNewt::ContactCallback()
{
    sEngine=soundEngine;
}

DefaultObjectContactCallback::~DefaultObjectContactCallback(void)
{
}

int DefaultObjectContactCallback::onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex )
{
    return 1;
}

void DefaultObjectContactCallback::contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex )
{
    float speed = abs(contactJoint.getFirstContact().getNormalSpeed());

    if(speed>11 && Global::fallSoundOffsetH<=0)
    {
        Ogre::String sound;
        bool notFound = true;
        OgreNewt::Body* body=contactJoint.getBody0();
        Ogre::Any any = body->getUserData();

        if(!any.isEmpty())
        {
            bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(any);
            if(bUD->sounds)
            {
                int rand = (int) Ogre::Math::RangeRandom(0,bUD->sounds->highHitAudio.size()-0.01f);
                sound = bUD->sounds->highHitAudio.at(rand);
                notFound = false;
            }
        }
        else
        {
            body=contactJoint.getBody1();
            any = body->getUserData();

            if(!any.isEmpty())
            {
                bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(any);
                if(bUD->sounds)
                {
                    int rand = (int) Ogre::Math::RangeRandom(0,bUD->sounds->highHitAudio.size()-0.01f);
                    sound = bUD->sounds->highHitAudio.at(rand);
                    notFound = false;
                }
            }
        }

        if(notFound)
            return;

        Ogre::Vector3 ppos,norm;
        contactJoint.getFirstContact().getPositionAndNormal(ppos,norm);
        irrklang::ISound* s = sEngine->play3D(sound.c_str(),irrklang::vec3df(ppos.x,ppos.y,ppos.z), false, false, true, irrklang::ESM_AUTO_DETECT, false);
        s->setMinDistance(5);
        s->drop();

        Global::fallSoundOffsetH = 0.25f;
        Global::fallSoundOffsetL = 0.1f;
    }
    else if(speed>6 && Global::fallSoundOffsetL<=0)
    {

        Ogre::String sound;
        bool notFound = true;
        OgreNewt::Body* body=contactJoint.getBody0();
        Ogre::Any any = body->getUserData();

        if(!any.isEmpty())
        {
            bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(any);
            if(bUD->sounds)
            {
                int rand = (int) Ogre::Math::RangeRandom(0,bUD->sounds->lowHitAudio.size()-0.01f);
                sound = bUD->sounds->lowHitAudio.at(rand);
                notFound = false;
            }
        }
        else
        {
            body=contactJoint.getBody1();
            any = body->getUserData();

            if(!any.isEmpty())
            {
                bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(any);
                if(bUD->sounds)
                {
                    int rand = (int) Ogre::Math::RangeRandom(0,bUD->sounds->lowHitAudio.size()-0.01f);
                    sound = bUD->sounds->lowHitAudio.at(rand);
                    notFound = false;
                }
            }
        }

        if(notFound)
            return;

        Ogre::Vector3 ppos,norm;
        contactJoint.getFirstContact().getPositionAndNormal(ppos,norm);
        irrklang::ISound* s = sEngine->play3D(sound.c_str(),irrklang::vec3df(ppos.x,ppos.y,ppos.z), false, false, false, irrklang::ESM_AUTO_DETECT, false);

        Global::fallSoundOffsetL = 0.15f;
    }

}

