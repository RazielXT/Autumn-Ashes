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

	PhysicsAudio* physicsAudio = nullptr;

	AudioLibrary(Ogre::Camera* cam);
    ~AudioLibrary();

	void update(float time);

    void play3D(const char* name, Ogre::Vector3& pos, float maxDistance = 10000000.f, float volume = 1.0f);
    void playRandom3D(std::vector<std::string>& sounds, Ogre::Vector3& pos, float maxDistance = 10000000.f, float volume = 1.0f);

    void playWalkingSound(float x, float y, float z, int groundID, float volume = 1.0f);
    void playFallSound(float x, float y, float z, int groundID);

    std::map<Ogre::String, ObjectAudio*> dynamicsAudio;

    static std::string getPath(std::string file, SoundType type = SoundEffect);

private:

    void fillMaterialAudio();
    void fillMoveAudio();

	Ogre::Camera* camera;
    irrklang::ISoundEngine* soundEngine;

    std::map<int, std::vector<Ogre::String>* > movementAudio;
};