#include "stdafx.h"
#include "SUtils.h"

using namespace Ogre;

const char* audioDirectory = "../../media/audio/";

AudioLibrary::AudioLibrary(Ogre::Camera* cam)
{
	Global::soundEngine = soundEngine = irrklang::createIrrKlangDevice();
	soundEngine->setListenerPosition(irrklang::vec3df(0, 0, 0), irrklang::vec3df(0, 0, 1));
	camera = cam;

	loadCfg("sounds.xml");
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

void AudioLibrary::loadCfgElement(const void* element, ParseState state)
{
	XMLElement* xmlElement = (XMLElement*)element;
	std::string name = xmlElement->Value();

	if (name == "Group")
	{
		auto g = new SoundLibrary::SoundGroup();
		auto path = xmlElement->Attribute("path");
		if (path)
			state.path += path + std::string("/");

		auto type = xmlElement->Attribute("type");
		if (type)
			state.type = type;

		auto type = xmlElement->Attribute("type");
		if (type)
			state.type = type;

		g->path = state.path;
		auto name = xmlElement->Attribute("name");
		name = name ? name : (path ? path : "");

		state.currentGroup->subGroups[name] = g;
		state.currentGroup = g;

		const XMLElement* childElement = 0;
		while (childElement = IterateChildElements(xmlElement, childElement))
			loadCfgElement(childElement, state);
	}
	if (name == "Sound")
	{
		auto file = xmlElement->Attribute("file");
		auto name = xmlElement->Attribute("name");
		if (!name) name = "";

		state.currentGroup->sounds.push_back({ name, file + state.type });
	}
	if (name == "Sounds")
	{
		auto format = xmlElement->Attribute("format");
		std::string files = xmlElement->Attribute("files");
		
		if (format == "array")
		{
			while (!files.empty())
			{
				auto file = SUtils::strtok_str(files, ',');
				state.currentGroup->sounds.push_back({ "", file + state.type });
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

	const XMLElement* childElement = 0;
	while (childElement = IterateChildElements(root, childElement))
	{
		loadCfgElement(childElement, {audioDirectory, ".wav", &library.groups});
	}
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
