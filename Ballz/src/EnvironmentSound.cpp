#include "stdafx.h"
#include <vector>
#include "EnvironmentSound.h"

EnvironmentSounds::EnvironmentSounds(irrklang::ISoundEngine* eng )
{
    engine=eng;
}

void EnvironmentSounds::addSound(	Ogre::Matrix4 transform,Ogre::Vector3 size,float decay,float volume,Ogre::String sound,bool continual)
{
    EnvSound* s=new EnvSound();
    s->halfSize=size/2;

    if(decay>s->halfSize.x) decay=s->halfSize.x;
    if(decay>s->halfSize.z) decay=s->halfSize.z;
    if(decay>s->halfSize.y) decay=s->halfSize.y;
    s->decay=decay;
    s->insideSize=s->halfSize-decay;
    s->transform=transform;
    s->volume=volume;
    s->sound = engine->play3D(sound.c_str(),irrklang::vec3df(0,0,0), true, false, true, irrklang::ESM_AUTO_DETECT, false);
    s->sound->setVolume(volume);
    s->continual=continual;

    if(continual)
        s->sound->setVolume(0);
    else
        s->sound->setIsPaused(true);

    s->playing=false;

    sounds.push_back(s);
}

void EnvironmentSounds::reset()
{
    for(unsigned int i=0; i<sounds.size(); i++)
    {
        EnvSound* s=sounds.at(i);
        s->sound->drop();
        delete s;
    }
    sounds.clear();
}

void EnvironmentSounds::update(Ogre::Vector3 pos)
{
    for(unsigned int i=0; i<sounds.size(); i++)
    {
        EnvSound* s=sounds.at(i);
        Ogre::Vector3 sPos=s->transform*pos;
        //checking code
        if(abs(sPos.x)<s->halfSize.x && abs(sPos.z)<s->halfSize.z && abs(sPos.y)<s->halfSize.y)
        {
            if(!s->playing)
            {
                if(s->continual)
                    s->sound->setVolume(s->volume);
                else
                    s->sound->setIsPaused(false);

                s->playing=true;
            }

            Ogre::Vector3 oPos=sPos;

            if(sPos.x<-s->insideSize.x)
                sPos.x=-s->insideSize.x;
            else if(sPos.x>s->insideSize.x)
                sPos.x=s->insideSize.x;

            if(sPos.y<-s->insideSize.y)
                sPos.y=-s->insideSize.y;
            else if(sPos.y>s->insideSize.y)
                sPos.y=s->insideSize.y;

            if(sPos.z<-s->insideSize.z)
                sPos.z=-s->insideSize.z;
            else if(sPos.z>s->insideSize.z)
                sPos.z=s->insideSize.z;

            s->sound->setVolume(s->volume*(s->decay-(oPos-sPos).length())/s->decay);

            sPos=s->transform.inverse()*sPos;

            s->sound->setPosition(irrklang::vec3df(sPos.x,sPos.y,sPos.z));
        }
        else if(s->playing)
        {
            if(s->continual)
                s->sound->setVolume(0);
            else
                s->sound->setIsPaused(true);
            s->playing=false;
        }
    }
}