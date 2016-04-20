#pragma once

#include "stdafx.h"
#include "AudioId.h"

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
	irrklang::ISound* playSoundId(AudioId sound, float x, float y, float z, bool drop = true, float maxDistance = 10000000.f, float volume = 1.0f);

	irrklang::ISoundSource* getSound(AudioId sound);
	std::vector<irrklang::ISoundSource*> getSoundGroup(AudioId group);

	irrklang::ISoundSource* loadSoundSource(std::string file);

private:

	irrklang::ISoundSource* loadNamedSound(AudioId name);
	std::map<std::string, irrklang::ISoundSource*> loadedSources;

	Ogre::Camera* camera;

	struct SoundLibrary
	{
		struct Sound
		{
			AudioId name;
			std::string file;
		};

		struct SoundGroup
		{
			std::string path;
			std::vector<Sound> sounds;
		};

		std::map<AudioId, SoundGroup> groups;
		std::map<AudioId, Sound> namedSounds;
	}
	library;

	struct ParseState
	{
		std::string path;
		std::string type;
		SoundLibrary::SoundGroup* group;
	};

	void loadCfg(std::string file);
	void loadCfgElement(const void* xmlElement, ParseState state);

	struct LoadedSounds
	{
		struct LoadedGroup
		{
			std::vector<irrklang::ISoundSource*> sounds;
		};

		std::map<AudioId, LoadedGroup> groups;
		std::map<AudioId, irrklang::ISoundSource*> namedSounds;
	}
	loaded;

};

