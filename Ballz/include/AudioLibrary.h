#pragma once

#include "stdafx.h"

enum SoundType
{
    SoundEffect,
    Music,
    Ui
};

struct ObjectAudio;

class AudioLibrary
{
public:
	AudioLibrary(irrklang::ISoundEngine* engine);
    ~AudioLibrary();

	irrklang::ISound* play3D(const char* name, Ogre::Vector3& pos, float maxDistance = 10000000.f, float volume = 1.0f);
	irrklang::ISound* playRandom3D(std::vector<std::string>& sounds, Ogre::Vector3& pos, float maxDistance = 10000000.f, float volume = 1.0f);

    void playWalkingSound(float x, float y, float z, int groundID);
    void playFallSound(float x, float y, float z, int groundID);

    std::map<Ogre::String, ObjectAudio*> dynamicsAudio;

    static inline std::string getPath(std::string file, SoundType type = SoundEffect);

private:

    void fillMaterialAudio();
    void fillMoveAudio();

	irrklang::ISoundEngine* soundEngine;

    std::map<int, std::vector<Ogre::String>* > movementAudio;
};