#include "stdafx.h"
#include "PlayerAudio.h"
#include "Player.h"

PlayerAudio::PlayerAudio(Player* player) : p(player)
{
	audio = Global::audioLib;
	engine = audio->soundEngine;

	loadMaterialAudio();
	loadMoveAudio();
}

PlayerAudio::~PlayerAudio()
{
	std::vector<void*> deleted;

	for (auto audio : dynamicsAudio)
	{
		if (std::find(deleted.begin(), deleted.end(), audio.second) == deleted.end())
		{
			deleted.push_back(audio.second);
			delete audio.second;
		}
	}

	deleted.clear();
	for (auto audio : movementAudio)
	{
		if (std::find(deleted.begin(), deleted.end(), audio) == deleted.end())
		{
			deleted.push_back(audio);
			delete audio;
		}
	}
}

void PlayerAudio::update(float tslf)
{
	auto& pos = p->camPosition;
	auto volumeVel = p->bodyVelocityL / 100.0f;
	for (auto& s : localSounds)
	{
		s.s->setVolume(std::min(volumeVel, 0.2f));
		s.s->setPosition(irrklang::vec3df(pos.x + s.x, pos.y + s.y, pos.z + s.z));
	}
}

void PlayerAudio::surfaceSliding(int groundId)
{
	if (lastSlidingId != groundId)
	{
		if (!localSounds.empty())
		{
			localSounds[0].s->stop();
			localSounds[0].s->drop();
			localSounds.clear();
		}


		switch (groundId)
		{
		case -1:
			break;
		case 0:
		default:
			auto s = audio->getSoundSource(AudioLibrary::getPath("MetalSolidFootstep_Slide_Loop.ogg", Character));
			auto a = audio->playSound(s, 0, 0, 0, false, 10, 0.0f);
			a->setIsLooped(true);
			localSounds.push_back({ a, 0, -2, 0 });
			break;
		}

		lastSlidingId = groundId;
	}
}

void PlayerAudio::playWalkingSound(float x, float y, float z, int groundID, float volume)
{
	if (groundID > MaterialsTypesMax)
		return;

	auto sounds = *movementAudio[groundID];

	int randID = (int)Ogre::Math::RangeRandom(0, sounds.size() - 0.01f);

	auto sound = sounds[randID];
	auto music = engine->play3D(sound, irrklang::vec3df(x, y, z), false, false, true, false);
	//music = soundEngine->play2D(sound.c_str(), false, false, true, irrklang::ESM_AUTO_DETECT, false);

	music->setVolume(volume);
	/*music->setMinDistance(0.5f);
	music->setPlaybackSpeed(Global::timestep);

	if (Global::timestep < 1)
	music->getSoundEffectControl()->enableWavesReverbSoundEffect(0, -10 * Global::timestep, 2600, 0.5);
	*/
	music->drop();
}

void PlayerAudio::playHurtSound(float x, float y, float z)
{
	audio->playSound(hurtAudio, x, y, z, true, 10);
}

void PlayerAudio::playClimbSound(float x, float y, float z)
{
	audio->playSound(climbAudio, x, y, z, true, 5, 0.5f);
}

void PlayerAudio::playFallSound(float x, float y, float z, int groundID)
{
	if (groundID > MaterialsTypesMax)
		return;

	irrklang::ISound* music = engine->play3D(fallAudio[groundID], irrklang::vec3df(x, y, z), false, false, true, true);
	music->setMaxDistance(10);
	music->setPlaybackSpeed(Global::timestep);

	if (Global::timestep < 1)
		music->getSoundEffectControl()->enableWavesReverbSoundEffect(0, -10 * Global::timestep, 2600, 0.5);

	music->drop();
}

void PlayerAudio::loadMaterialAudio()
{
	ObjectAudio* audio = new ObjectAudio;
	audio->highHitAudio.push_back(AudioLibrary::getPath("woodFall.wav"));
	audio->lowHitAudio.push_back(AudioLibrary::getPath("wood_walk_01.wav"));
	audio->lowHitAudio.push_back(AudioLibrary::getPath("wood_walk_02.wav"));
	audio->lowHitAudio.push_back(AudioLibrary::getPath("wood_walk_03.wav"));
	audio->lowHitAudio.push_back(AudioLibrary::getPath("wooden-plank-snap.wav"));
	audio->climbDoubleFreq = true;
	dynamicsAudio["Wood"] = audio;

	audio = new ObjectAudio;
	audio->highHitAudio.push_back(AudioLibrary::getPath("metalgrate2.wav"));
	audio->lowHitAudio.push_back(AudioLibrary::getPath("metalgrate4.wav"));
	audio->lowHitAudio.push_back(AudioLibrary::getPath("metalgrate1.wav"));
	audio->lowHitAudio.push_back(AudioLibrary::getPath("metalgrate3.wav"));
	audio->climbDoubleFreq = true;
	dynamicsAudio["Metal"] = audio;

	audio = new ObjectAudio;
	audio->highHitAudio.push_back(AudioLibrary::getPath("bush3.wav"));
	audio->lowHitAudio.push_back(AudioLibrary::getPath("bush2.wav"));
	audio->lowHitAudio.push_back(AudioLibrary::getPath("bush1.wav"));
	audio->lowHitAudio.push_back(AudioLibrary::getPath("bush3.wav"));
	audio->climbDoubleFreq = false;
	dynamicsAudio["Bush"] = audio;
}

void PlayerAudio::loadMoveAudio()
{
	movementAudio.resize(MaterialsTypesMax + 1);
	std::vector<irrklang::ISoundSource*>* sounds = new std::vector<irrklang::ISoundSource*>();
	audio->addPossibleSounds(sounds, { AudioLibrary::getPath("Stone_Hard_Walk_01.wav"), AudioLibrary::getPath("Stone_Hard_Walk_02.wav"), AudioLibrary::getPath("Stone_Hard_Walk_03.wav") });
	movementAudio[1] = sounds;

	sounds = new std::vector<irrklang::ISoundSource*>();
	audio->addPossibleSounds(sounds, { AudioLibrary::getPath("grass_walk_01.wav"), AudioLibrary::getPath("grass_walk_02.wav") , AudioLibrary::getPath("grass_walk_03.wav"), AudioLibrary::getPath("dirt_walk_02.wav") ,AudioLibrary::getPath("Dirt_Walk_03.wav") });
	movementAudio[2] = movementAudio[0] = movementAudio[8] = sounds;

	sounds = new std::vector<irrklang::ISoundSource*>();
	audio->addPossibleSounds(sounds, { AudioLibrary::getPath("metalgrate1.wav"),AudioLibrary::getPath("metalgrate2.wav"),AudioLibrary::getPath("metalgrate3.wav"),AudioLibrary::getPath("metalgrate4.wav") });
	movementAudio[3] = movementAudio[7] = sounds;

	sounds = new std::vector<irrklang::ISoundSource*>();
	audio->addPossibleSounds(sounds, { AudioLibrary::getPath("wood_walk_01.wav"),AudioLibrary::getPath("wood_walk_02.wav"),AudioLibrary::getPath("wood_walk_03.wav") });
	movementAudio[4] = movementAudio[5] = movementAudio[6] = sounds;

	//-------------------------------------

	fallAudio.resize(MaterialsTypesMax + 1);
	fallAudio[1] = audio->getSoundSource(AudioLibrary::getPath("Stone_Hard_Walk_01.wav"));
	fallAudio[0] = fallAudio[2] = fallAudio[8] = audio->getSoundSource(AudioLibrary::getPath("grass_run_02.wav"));
	fallAudio[4] = fallAudio[5] = fallAudio[6] = audio->getSoundSource(AudioLibrary::getPath("wood_run_02.wav"));
	fallAudio[3] = fallAudio[7] = audio->getSoundSource(AudioLibrary::getPath("metalgrate1.wav"));

	hurtAudio = audio->getSoundSource(AudioLibrary::getPath("pad.wav"));
	climbAudio = audio->getSoundSource(AudioLibrary::getPath("pullup.wav"));
}