#include "stdafx.h"

using namespace Ogre;

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


AudioLibrary::AudioLibrary(Ogre::Camera* cam)
{
    fillMaterialAudio();
    fillMoveAudio();

    camera = cam;
    soundEngine = irrklang::createIrrKlangDevice();
    soundEngine->setListenerPosition(irrklang::vec3df(0, 0, 0), irrklang::vec3df(0, 0, 1));
    Global::soundEngine = soundEngine;
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

    soundEngine->drop();
}

void AudioLibrary::update(float time)
{
    Vector3 pos = camera->getDerivedPosition();
    Vector3 or = camera->getDerivedOrientation()*Vector3(0, 0, 1);
    soundEngine->setListenerPosition(irrklang::vec3df(pos.x, pos.y, pos.z), irrklang::vec3df(or.x, or.y, or.z));

    if (physicsAudio)
    {
        if (physicsAudio->fallSoundOffsetH > 0)
        {
            physicsAudio->fallSoundOffsetH -= time;
        }
        if (physicsAudio->fallSoundOffsetL > 0)
        {
            physicsAudio->fallSoundOffsetL -= time;
        }
    }
}

void AudioLibrary::playRandom3D(std::vector<std::string>& sounds, Ogre::Vector3& pos, float maxDistance, float volume)
{
    int rand = (int)Ogre::Math::RangeRandom(0, sounds.size() - 0.01f);
    String& sound = sounds[rand];

    play3D(sound.c_str(), pos, maxDistance, volume);
}

void AudioLibrary::play3D(const char* name, Vector3& pos, float maxDistance, float volume)
{
    irrklang::ISound* s = soundEngine->play3D(AudioLibrary::getPath(name).c_str(), irrklang::vec3df(pos.x, pos.y + 2, pos.z), false, false, true, irrklang::ESM_AUTO_DETECT, true);
    s->setMaxDistance(maxDistance);
    s->setVolume(volume);

    if (Global::timestep < 1)
    {
        s->setPlaybackSpeed(Global::timestep);
        s->getSoundEffectControl()->enableWavesReverbSoundEffect(0, -10 * Global::timestep, 2600, 0.5);
    }

    s->drop();
}
void AudioLibrary::playWalkingSound(float x, float y, float z, int groundID, float volume)
{
    auto sounds = movementAudio.find(groundID);
    if (sounds == movementAudio.end()) return;

    int randID = (int)Ogre::Math::RangeRandom(0, sounds->second->size() - 0.01f);

    auto sound = sounds->second->at(randID);
    auto music = soundEngine->play3D(sound.c_str(), irrklang::vec3df(x, y, z), false, false, true, irrklang::ESM_AUTO_DETECT, false);
    //music = soundEngine->play2D(sound.c_str(), false, false, true, irrklang::ESM_AUTO_DETECT, false);

    music->setVolume(volume);
    /*music->setMinDistance(0.5f);
    music->setPlaybackSpeed(Global::timestep);

    if (Global::timestep < 1)
        music->getSoundEffectControl()->enableWavesReverbSoundEffect(0, -10 * Global::timestep, 2600, 0.5);
    	*/
    music->drop();
}

void AudioLibrary::playFallSound(float x, float y, float z, int groundID)
{
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
