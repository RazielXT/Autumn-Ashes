#include "stdafx.h"

using namespace Ogre;


AudioLibrary::AudioLibrary(Ogre::Camera* cam)
{
    Global::soundEngine = soundEngine = irrklang::createIrrKlangDevice();
    soundEngine->setListenerPosition(irrklang::vec3df(0, 0, 0), irrklang::vec3df(0, 0, 1));
    camera = cam;

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
        if (std::find(deleted.begin(), deleted.end(), audio) == deleted.end())
        {
            deleted.push_back(audio);
            delete audio;
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

void AudioLibrary::reset()
{
    soundEngine->removeAllSoundSources();

    preloadedSounds.clear();
    fillMoveAudio();
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
    if (groundID > MaterialsTypesMax)
        return;

    auto sounds = *movementAudio[groundID];

    int randID = (int)Ogre::Math::RangeRandom(0, sounds.size() - 0.01f);

    auto sound = sounds[randID];
    auto music = soundEngine->play3D(sound, irrklang::vec3df(x, y, z), false, false, true, false);
    //music = soundEngine->play2D(sound.c_str(), false, false, true, irrklang::ESM_AUTO_DETECT, false);

    music->setVolume(volume);
    /*music->setMinDistance(0.5f);
    music->setPlaybackSpeed(Global::timestep);

    if (Global::timestep < 1)
        music->getSoundEffectControl()->enableWavesReverbSoundEffect(0, -10 * Global::timestep, 2600, 0.5);
    	*/
    music->drop();
}

irrklang::ISound* AudioLibrary::playSound(irrklang::ISoundSource* sound, float x, float y, float z, bool drop, float maxDistance, float volume)
{
    irrklang::ISound* music = soundEngine->play3D(sound, irrklang::vec3df(x, y, z), false, false, true, true);
    music->setMaxDistance(maxDistance);
    music->setVolume(volume);
    music->setPlaybackSpeed(Global::timestep);

    if (Global::timestep < 1)
        music->getSoundEffectControl()->enableWavesReverbSoundEffect(0, -10 * Global::timestep, 2600, 0.5);

    if (drop)
        music->drop();

    return music;
}

void AudioLibrary::playHurtSound(float x, float y, float z)
{
    playSound(hurtAudio, x, y, z, true, 10);
}

void AudioLibrary::playClimbSound(float x, float y, float z)
{
    playSound(climbAudio, x, y, z, true, 5, 0.3f);
}

void AudioLibrary::playFallSound(float x, float y, float z, int groundID)
{
    if (groundID > MaterialsTypesMax)
        return;

    irrklang::ISound* music  = soundEngine->play3D(fallAudio[groundID], irrklang::vec3df(x, y, z), false, false, true, true);
    music->setMaxDistance(10);
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

irrklang::ISoundSource* AudioLibrary::getSoundSource(std::string path)
{
    auto s = preloadedSounds.find(path);
    auto sound = s->second;

    if (s == preloadedSounds.end())
        sound = preloadSound(path);

    return sound;
}

irrklang::ISoundSource* AudioLibrary::preloadSound(std::string path)
{
    auto sound = soundEngine->addSoundSourceFromFile(path.c_str(), irrklang::ESM_AUTO_DETECT, true);

    if (sound)
        preloadedSounds[path] = sound;

    return sound;
}

void AudioLibrary::addPossibleSounds(std::vector<irrklang::ISoundSource*>* sounds, std::vector<std::string> soundFiles)
{
    for (auto& soundFile : soundFiles)
    {
        auto sound = getSoundSource(soundFile);

        if (sound)
            sounds->push_back(sound);
    }
}

void AudioLibrary::fillMoveAudio()
{
    movementAudio.resize(MaterialsTypesMax + 1);
    std::vector<irrklang::ISoundSource*>* sounds = new std::vector<irrklang::ISoundSource*>();
    addPossibleSounds(sounds, { getPath("Stone_Hard_Walk_01.wav"), getPath("Stone_Hard_Walk_02.wav"), getPath("Stone_Hard_Walk_03.wav") });
    movementAudio[1] = sounds;

    sounds = new std::vector<irrklang::ISoundSource*>();
    addPossibleSounds(sounds, { getPath("grass_walk_01.wav"), getPath("grass_walk_02.wav") , getPath("grass_walk_03.wav"), getPath("dirt_walk_02.wav") ,getPath("Dirt_Walk_03.wav") });
    movementAudio[2] = movementAudio[0] = movementAudio[8] = sounds;

    sounds = new std::vector<irrklang::ISoundSource*>();
    addPossibleSounds(sounds, { getPath("metalgrate1.wav"),getPath("metalgrate2.wav"),getPath("metalgrate3.wav"),getPath("metalgrate4.wav") });
    movementAudio[3] = movementAudio[7] = sounds;

    sounds = new std::vector<irrklang::ISoundSource*>();
    addPossibleSounds(sounds, { getPath("wood_walk_01.wav"),getPath("wood_walk_02.wav"),getPath("wood_walk_03.wav") });
    movementAudio[4] = movementAudio[5] = movementAudio[6] = sounds;

    //-------------------------------------

    fallAudio.resize(MaterialsTypesMax + 1);
    fallAudio[1] = getSoundSource(getPath("Stone_Hard_Walk_01.wav"));
    fallAudio[0] = fallAudio[2] = fallAudio[8] = getSoundSource(getPath("grass_run_02.wav"));
    fallAudio[4] = fallAudio[5] = fallAudio[6] = getSoundSource(getPath("wood_run_02.wav"));
    fallAudio[3] = fallAudio[7] = getSoundSource(getPath("metalgrate1.wav"));

    hurtAudio = getSoundSource(getPath("pad.wav"));
    climbAudio = getSoundSource(getPath("pad.wav"));
}
