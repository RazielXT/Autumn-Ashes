#pragma once

#include "stdafx.h"

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

	AudioLibrary(Ogre::Camera* camera);
	~AudioLibrary();

	void update(float time);
	void reset();

	void playSoundRandom(std::vector<irrklang::ISoundSource*>& sounds, float x, float y, float z, float maxDistance = 10000000.f, float volume = 1.0f);
	irrklang::ISound* playSound(irrklang::ISoundSource* sound, float x, float y, float z, bool drop = true, float maxDistance = 10000000.f, float volume = 1.0f);

	irrklang::ISoundSource* getSound(std::string name);
	std::vector<irrklang::ISoundSource*> getSoundGroup(std::vector<std::string> groupPath);

private:

	irrklang::ISoundSource* preloadNamedSound(std::string name);
	irrklang::ISoundSource* preloadSound(std::string path);

	Ogre::Camera* camera;

	struct SoundLibrary
	{
		struct Sound
		{
			std::string file;
		};

		struct SoundGroup
		{
			std::string path;
			std::map<std::string, SoundGroup*> subGroups;
			std::vector<Sound> sounds;
		};

		SoundGroup groups;
		std::map<std::string, Sound> namedSounds;
	}
	library;

	struct LoadedSounds
	{
		struct LoadedGroup
		{
			std::map<std::string, LoadedGroup*> subGroups;
			std::vector<irrklang::ISoundSource*> sounds;

		};

		std::map<std::string, LoadedGroup*> groups;
		std::map<std::string, irrklang::ISoundSource*> namedSounds;

		void clearGroup(LoadedGroup* group);
	}
	loaded;
};

