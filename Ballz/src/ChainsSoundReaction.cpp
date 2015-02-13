#include "stdafx.h"
#include "ChainsSoundTask.h"


ChainsSoundTask::ChainsSoundTask()
{
}

void ChainsSoundTask::setUserData(void* data)
{
    mBody = (OgreNewt::Body*) data;
}

bool ChainsSoundTask::start()
{
    if(running)
        return false;

    playing = false;

    Ogre::String sound = "../../media/chains.wav";

    Ogre::Vector3 pos = mBody->getPosition();
    s = Global::soundEngine->play3D(sound.c_str(),irrklang::vec3df(pos.x,pos.y,pos.z), true, true, true, irrklang::ESM_AUTO_DETECT,false);
    s->setMinDistance(2);
    s->setPlayPosition((UINT)Ogre::Math::RangeRandom(0,(float)s->getPlayLength()));

    return true;
}

bool ChainsSoundTask::update(Ogre::Real tslf)
{
    //float val = Ogre::Math::Clamp(mBody->getVelocity().squaredLength(),0.0f,1.0f);
    float val = Ogre::Math::Clamp(mBody->getOmega().squaredLength()/3.0f,0.0f,1.0f);

    if(playing)
    {
        s->setVolume(val);

        if(val<0.01)
        {
            playing = false;
            s->setIsPaused(true);
        }
    }
    else
    {
        if(val>0.01)
        {
            s->setVolume(val);
            playing = true;
            s->setIsPaused(false);
        }
    }

    return true;
}
