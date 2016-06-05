#include "stdafx.h"
#include "SUtils.h"

using namespace Ogre;

const char* audioDirectory = Path::Audio;

AudioLibrary::AudioLibrary(Ogre::Camera* cam)
{
	Global::soundEngine = soundEngine = irrklang::createIrrKlangDevice();
	soundEngine->setListenerPosition(irrklang::vec3df(0, 0, 0), irrklang::vec3df(0, 0, 1));
	camera = cam;

	loadCfg("character.xml");
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
	loaded.groups.clear();
	loaded.namedSounds.clear();
	loadedSources.clear();
}

void AudioLibrary::playSoundRandom(std::vector<irrklang::ISoundSource*>& sounds, float x, float y, float z, float maxDistance, float volume)
{
	int rand = (int)Ogre::Math::RangeRandom(0, sounds.size() - 0.01f);
	auto sound = sounds[rand];

	playSound(sound, x, y, z, true, maxDistance, volume);
}

irrklang::ISound* AudioLibrary::playSound(irrklang::ISoundSource* sound, float x, float y, float z, bool drop, float maxDistance, float volume)
{
	irrklang::ISound* music = soundEngine->play3D(sound, irrklang::vec3df(x, y, z), false, false, true, false);
	music->setMaxDistance(maxDistance);
	music->setVolume(volume);
	music->setPlaybackSpeed(Global::timestep);

	if (drop)
		music->drop();

	return music;
}

irrklang::ISound* AudioLibrary::playSoundId(AudioId sound, float x, float y, float z, bool drop /*= true*/, float maxDistance /*= 10000000.f*/, float volume /*= 1.0f*/)
{
	return playSound(getSound(sound), x, y, z, drop, maxDistance, volume);
}

irrklang::ISoundSource* AudioLibrary::getSound(AudioId name)
{
	auto s = loaded.namedSounds.find(name);

	if (s != loaded.namedSounds.end())
		return s->second;
	else
		return loadNamedSound(name);
}

std::vector<irrklang::ISoundSource*> AudioLibrary::getSoundGroup(AudioId group)
{
	auto lg = loaded.groups.find(group);

	if (lg != loaded.groups.end())
		return lg->second.sounds;
	else
	{
		std::vector<irrklang::ISoundSource*> out;

		auto g = library.groups.find(group);
		if (g != library.groups.end())
			for (auto s : g->second.sounds)
			{
				out.push_back(loadSoundSource(g->second.path + s.file));
			}

		loaded.groups[group] = { out };

		return out;
	}
}

irrklang::ISoundSource* AudioLibrary::loadNamedSound(AudioId name)
{
	auto s = library.namedSounds.find(name);

	if (s == library.namedSounds.end())
		return nullptr;

	auto source = loadSoundSource(s->second.file);
	loaded.namedSounds[name] = source;

	return source;
}

irrklang::ISoundSource* AudioLibrary::loadSoundSource(std::string path)
{
	auto s = loadedSources.find(path);

	if (s == loadedSources.end())
	{
		auto fullPath = audioDirectory + path;
		auto source = soundEngine->addSoundSourceFromFile(fullPath.c_str(), irrklang::ESM_AUTO_DETECT, true);
		loadedSources[path] = source;
		return source;
	}
	else
		return s->second;
}

#include "tinyxml2.h"
using namespace tinyxml2;

static const XMLElement* IterateChildElements(const XMLElement* xmlElement, const XMLElement* childElement)
{
	if (xmlElement != 0)
	{
		if (childElement == 0)
			childElement = xmlElement->FirstChildElement();
		else
			childElement = childElement->NextSiblingElement();

		return childElement;
	}

	return 0;
}

AudioId getAudioId(const char* str)
{
	if (str)
		for (size_t i = 0; i < Invalid; i++)
		{
			if (strcmp(str, audioIdNames[i]) == 0)
				return AudioId(i);
		}

	return Invalid;
}

void AudioLibrary::loadCfgElement(const void* element, ParseState state)
{
	XMLElement* xmlElement = (XMLElement*)element;
	std::string name = xmlElement->Value();

	auto path = xmlElement->Attribute("path");
	if (path) state.path = path + std::string("/");

	path = xmlElement->Attribute("subpath");
	if (path) state.path += path + std::string("/");

	auto type = xmlElement->Attribute("type");
	if (type) state.type = std::string(".") + type;

	if (name == "info")
	{
		const XMLElement* childElement = 0;
		while (childElement = IterateChildElements(xmlElement, childElement))
			loadCfgElement(childElement, state);
	}
	if (name == "group")
	{
		auto name = xmlElement->Attribute("name");
		auto& g = library.groups[getAudioId(name)];
		g.path = state.path;
		state.group = &g;

		const XMLElement* childElement = 0;
		while (childElement = IterateChildElements(xmlElement, childElement))
			loadCfgElement(childElement, state);
	}
	if (name == "sound")
	{
		auto file = xmlElement->Attribute("file");
		auto name = getAudioId(xmlElement->Attribute("name"));

		if(state.group)
			state.group->sounds.push_back({ name , file + state.type });

		if (name != Invalid)
			library.namedSounds[name] = { name , state.path + file + state.type };
	}
	if (name == "sounds")
	{
		auto format = xmlElement->Attribute("format");
		std::string files = xmlElement->Attribute("files");

		if (format == "array")
		{
			while (!files.empty())
			{
				auto file = SUtils::strtok_str(files, ',');
				state.group->sounds.push_back({ Invalid, file + state.type });
			}
		}
	}
}

void AudioLibrary::loadCfg(std::string file)
{
	file = audioDirectory + file;

	XMLDocument document;
	document.LoadFile(file.c_str());
	auto root = document.RootElement();

	loadCfgElement(root, {"", ".wav", nullptr});
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
