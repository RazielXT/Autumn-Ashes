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
	
	for (auto g : loaded.groups)
	{
		loaded.clearGroup(g.second);
		delete g.second;
	}
}

void AudioLibrary::LoadedSounds::clearGroup(LoadedGroup* group)
{
	for (auto g : group->subGroups)
	{
		clearGroup(g.second);	
	}

	delete group;
}

void AudioLibrary::playSoundRandom(std::vector<irrklang::ISoundSource*>& sounds, float x, float y, float z, float maxDistance, float volume)
{
	int rand = (int)Ogre::Math::RangeRandom(0, sounds.size() - 0.01f);
	auto sound = sounds[rand];

	playSound(sound, x, y, z, true, maxDistance, volume);
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

irrklang::ISoundSource* AudioLibrary::getSound(std::string name)
{
	auto s = loaded.namedSounds.find(name);
	auto sound = s->second;

	if (s == loaded.namedSounds.end())
		sound = preloadNamedSound(name);

	return sound;
}

std::vector<irrklang::ISoundSource*> AudioLibrary::getSoundGroup(std::vector<std::string> groupPath)
{
	auto group = &library.groups;

	for (auto& subGroup : groupPath)
	{
		auto it = group->subGroups.find(subGroup);
		if (it == group->subGroups.end())
			return{};

		group = it->second;
	}

	std::vector<irrklang::ISoundSource*> out;
	for (auto& s : group->sounds)
	{
		auto source = preloadSound(group->path + s.file);

		if (source)
			out.push_back(source);
	}

	return out;
}

irrklang::ISoundSource* AudioLibrary::preloadSound(std::string path)
{
	return soundEngine->addSoundSourceFromFile(path.c_str(), irrklang::ESM_AUTO_DETECT, true);
}

irrklang::ISoundSource* AudioLibrary::preloadNamedSound(std::string name)
{
	auto s = library.namedSounds.find(name);

	if (s == library.namedSounds.end())
		return nullptr;

	auto source = soundEngine->addSoundSourceFromFile(s->second.file.c_str(), irrklang::ESM_AUTO_DETECT, true);
	loaded.namedSounds[name] = source;

	return source;
}

/*
void AudioLibrary::addPossibleSounds(std::vector<irrklang::ISoundSource*>* sounds, std::vector<std::string> soundFiles)
{
	for (auto& soundFile : soundFiles)
	{
		auto sound = getSoundSource(soundFile);

		if (sound)
			sounds->push_back(sound);
	}
}*/
