#pragma once

#include "stdafx.h"

enum SoundType
{
	SoundEffect,
	Character,
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

	irrklang::ISoundEngine* soundEngine;
	PhysicsAudio* physicsAudio = nullptr;

	AudioLibrary(Ogre::Camera* cam);
	~AudioLibrary();

	void update(float time);
	void reset();

	void play3D(const char* name, Ogre::Vector3& pos, float maxDistance = 10000000.f, float volume = 1.0f);
	void playRandom3D(std::vector<std::string>& sounds, Ogre::Vector3& pos, float maxDistance = 10000000.f, float volume = 1.0f);

	irrklang::ISound* playSound(irrklang::ISoundSource* sound, float x, float y, float z, bool drop = true, float maxDistance = 10000000.f, float volume = 1.0f);

	irrklang::ISoundSource* getSoundSource(std::string path);

	static std::string getPath(std::string file, SoundType type = SoundEffect)
	{
		switch (type)
		{
		case Music:
			return std::string("../../media/audio/music/" + file);
		case Character:
			return std::string("../../media/audio/character/" + file);
		case SoundEffect:
		default:
			return std::string("../../media/audio/effects/" + file);
		}
	}

	void addPossibleSounds(std::vector<irrklang::ISoundSource*>* sounds, std::vector<std::string> soundFiles);

private:

	irrklang::ISoundSource* preloadSound(std::string path);
	std::map<std::string, irrklang::ISoundSource*> preloadedSounds;

	Ogre::Camera* camera;

};
