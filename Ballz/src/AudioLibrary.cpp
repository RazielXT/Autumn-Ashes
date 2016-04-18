#include "stdafx.h"

using namespace Ogre;


AudioLibrary::AudioLibrary(Ogre::Camera* cam)
{
	Global::soundEngine = soundEngine = irrklang::createIrrKlangDevice();
	soundEngine->setListenerPosition(irrklang::vec3df(0, 0, 0), irrklang::vec3df(0, 0, 1));
	camera = cam;
}

AudioLibrary::~AudioLibrary()
{
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
