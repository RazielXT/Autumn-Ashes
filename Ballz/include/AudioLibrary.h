#pragma once

#include "stdafx.h"

enum SoundType
{
    SoundEffect,
    Music,
    Ui
};

struct ObjectAudio;

struct PhysicsAudio
{
    float fallSoundOffsetH;
    float fallSoundOffsetL;
};

class AudioLibrary
{
public:

    static const int MaterialsTypesMax = 8;

    PhysicsAudio* physicsAudio = nullptr;

    AudioLibrary(Ogre::Camera* cam);
    ~AudioLibrary();

    void update(float time);
    void reset();

    void play3D(const char* name, Ogre::Vector3& pos, float maxDistance = 10000000.f, float volume = 1.0f);
    void playRandom3D(std::vector<std::string>& sounds, Ogre::Vector3& pos, float maxDistance = 10000000.f, float volume = 1.0f);

    void playWalkingSound(float x, float y, float z, int groundID, float volume = 1.0f);
    void playFallSound(float x, float y, float z, int groundID);
    void playHurtSound(float x, float y, float z);
    void playClimbSound(float x, float y, float z);
    irrklang::ISound* playSound(irrklang::ISoundSource* sound, float x, float y, float z, bool drop = true, float maxDistance = 10000000.f, float volume = 1.0f);

    std::map<Ogre::String, ObjectAudio*> dynamicsAudio;

    irrklang::ISoundSource* getSoundSource(std::string path);

    static std::string getPath(std::string file, SoundType type = SoundEffect)
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

private:

    irrklang::ISoundSource* preloadSound(std::string path);

    void fillMaterialAudio();
    void fillMoveAudio();
    void addPossibleSounds(std::vector<irrklang::ISoundSource*>* sounds, std::vector<std::string> soundFiles);

    Ogre::Camera* camera;
    irrklang::ISoundEngine* soundEngine;

    std::vector< std::vector<irrklang::ISoundSource*>* > movementAudio;
    std::vector<irrklang::ISoundSource* > fallAudio;
    irrklang::ISoundSource* hurtAudio;
    irrklang::ISoundSource* climbAudio;

    std::map<std::string, irrklang::ISoundSource*> preloadedSounds;
};
