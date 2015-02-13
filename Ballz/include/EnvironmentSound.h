#pragma once

#include "stdafx.h"
#include <irrKlang.h>
#include <vector>

struct EnvSound
{
    Ogre::Matrix4 transform;
    Ogre::Vector3 halfSize;
    Ogre::Vector3 insideSize;
    float decay;
    float volume;
    irrklang::ISound* sound;
    bool playing;
    bool continual;
};

class EnvironmentSounds
{
public:

    EnvironmentSounds(irrklang::ISoundEngine* eng);
    //~EnvironmentSounds ();
    void addSound(	Ogre::Matrix4 transform,Ogre::Vector3 size,float decay,float volume,Ogre::String sound, bool continual);
    void update(Ogre::Vector3 pos);
    void reset();

private:

    std::vector<EnvSound*> sounds;
    irrklang::ISoundEngine* engine;

};