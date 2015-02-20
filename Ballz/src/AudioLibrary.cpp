#include "stdafx.h"
#include "globals.h"


std::string AudioLibrary::getPath(std::string file, SoundType type)
{
    switch (type)
    {
    case Music:
        return std::string("../../media/audio/music/" + file);
    case SoundEffect:
    default:
        return std::string("../../media/audio/effects/" + file);
    }
}


AudioLibrary::AudioLibrary()
{
    fillMaterialAudio();
    fillMoveAudio();
}

AudioLibrary::~AudioLibrary()
{
    std::vector<void*> deleted;

    for (auto audio : dynamicsAudio)
    {
        if (std::find(deleted.begin(), deleted.end(), audio.second) == deleted.end())
        {
            deleted.push_back(audio.second);
            delete audio.second;
        }
    }

    deleted.clear();
    for (auto audio : movementAudio)
    {
        if (std::find(deleted.begin(), deleted.end(), audio.second) == deleted.end())
        {
            deleted.push_back(audio.second);
            delete audio.second;
        }
    }
}

void AudioLibrary::playWalkingSound(float x, float y, float z, int groundID)
{
    auto soundEngine = Global::soundEngine;
    irrklang::ISound* music;

    auto sounds = movementAudio.find(groundID);

    if (sounds == movementAudio.end()) return;

    int randID = (int)Ogre::Math::RangeRandom(0, sounds->second->size() - 0.01f);

    auto sound = sounds->second->at(randID);
    music = soundEngine->play3D(sound.c_str(), irrklang::vec3df(x, y, z), false, false, true, irrklang::ESM_AUTO_DETECT, true);

    music->setMaxDistance(10);
    music->setPlaybackSpeed(Global::timestep);

    if (Global::timestep < 1)
        music->getSoundEffectControl()->enableWavesReverbSoundEffect(0, -10 * Global::timestep, 2600, 0.5);

	music->drop();
}

void AudioLibrary::playFallSound(float x, float y, float z, int groundID)
{
    auto soundEngine = Global::soundEngine;
    irrklang::ISound* music;

    switch (groundID)
    {
    case 1:
        music = soundEngine->play3D(getPath("Stone_Hard_Walk_02.wav").c_str(), irrklang::vec3df(x, y, z), false, false, true, irrklang::ESM_AUTO_DETECT, true);
        music->setMaxDistance(10);
        break;
    case 0:
    case 2:
    case 8:
        music = soundEngine->play3D(getPath("grass_run_02.wav").c_str(), irrklang::vec3df(x, y, z), false, false, true, irrklang::ESM_AUTO_DETECT, true);
        music->setMaxDistance(10);
        break;
    case 4:
    case 5:
    case 6:
        music = soundEngine->play3D(getPath("wood_run_02.wav").c_str(), irrklang::vec3df(x, y, z), false, false, true, irrklang::ESM_AUTO_DETECT, true);
        music->setMaxDistance(10);
        break;
    case 3:
    default:
        music = soundEngine->play3D(getPath("metalgrate1.wav").c_str(), irrklang::vec3df(x, y, z), false, false, true, irrklang::ESM_AUTO_DETECT, true);
        music->setMaxDistance(10);
        break;

    }

    music->setPlaybackSpeed(Global::timestep);

    if (Global::timestep < 1)
        music->getSoundEffectControl()->enableWavesReverbSoundEffect(0, -10 * Global::timestep, 2600, 0.5);

	music->drop();
}

void AudioLibrary::fillMaterialAudio()
{
    ObjectAudio* audio = new ObjectAudio;
    audio->highHitAudio.push_back(getPath("woodFall.wav"));
    audio->lowHitAudio.push_back(getPath("wood_walk_01.wav"));
    audio->lowHitAudio.push_back(getPath("wood_walk_02.wav"));
    audio->lowHitAudio.push_back(getPath("wood_walk_03.wav"));
    audio->lowHitAudio.push_back(getPath("wooden-plank-snap.wav"));
    audio->climbDoubleFreq = true;
    dynamicsAudio["Wood"] = audio;

    audio = new ObjectAudio;
    audio->highHitAudio.push_back(getPath("metalgrate2.wav"));
    audio->lowHitAudio.push_back(getPath("metalgrate4.wav"));
    audio->lowHitAudio.push_back(getPath("metalgrate1.wav"));
    audio->lowHitAudio.push_back(getPath("metalgrate3.wav"));
    audio->climbDoubleFreq = true;
    dynamicsAudio["Metal"] = audio;

    audio = new ObjectAudio;
    audio->highHitAudio.push_back(getPath("bush3.wav"));
    audio->lowHitAudio.push_back(getPath("bush2.wav"));
    audio->lowHitAudio.push_back(getPath("bush1.wav"));
    audio->lowHitAudio.push_back(getPath("bush3.wav"));
    audio->climbDoubleFreq = false;
    dynamicsAudio["Bush"] = audio;
}

void AudioLibrary::fillMoveAudio()
{
    std::vector<Ogre::String>* sounds = new std::vector<Ogre::String>();
    sounds->push_back(getPath("Stone_Hard_Walk_01.wav"));
    sounds->push_back(getPath("Stone_Hard_Walk_02.wav"));
    sounds->push_back(getPath("Stone_Hard_Walk_03.wav"));
    movementAudio[1] = sounds;

    sounds = new std::vector<Ogre::String>();
    sounds->push_back(getPath("grass_walk_01.wav"));
    sounds->push_back(getPath("grass_walk_02.wav"));
    sounds->push_back(getPath("grass_walk_03.wav"));
    sounds->push_back(getPath("dirt_walk_02.wav"));
    sounds->push_back(getPath("Dirt_Walk_03.wav"));
    movementAudio[2] = sounds;
    movementAudio[0] = sounds;
    movementAudio[8] = sounds;

    sounds = new std::vector<Ogre::String>();
    sounds->push_back(getPath("metalgrate1.wav"));
    sounds->push_back(getPath("metalgrate2.wav"));
    sounds->push_back(getPath("metalgrate3.wav"));
    sounds->push_back(getPath("metalgrate4.wav"));
    movementAudio[3] = sounds;
    movementAudio[7] = sounds;

    sounds = new std::vector<Ogre::String>();
    sounds->push_back(getPath("wood_walk_01.wav"));
    sounds->push_back(getPath("wood_walk_02.wav"));
    sounds->push_back(getPath("wood_walk_03.wav"));
    movementAudio[4] = sounds;
    movementAudio[5] = sounds;
    movementAudio[6] = sounds;
}
