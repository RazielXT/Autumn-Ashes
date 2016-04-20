#pragma once

#include "stdafx.h"

class PlayerAudio
{
	Player* p;
	AudioLibrary* audio;
	irrklang::ISoundEngine* engine;

public:

	PlayerAudio(Player* player);
	~PlayerAudio();

	void update(float tslf);

	void slide(bool active);
	void surfaceSliding(int groundId = -1);

	void playWalkingSound(float x, float y, float z, int groundID, float volume = 1.0f);
	void playFallSound(float x, float y, float z, int groundID);
	void playHurtSound(float x, float y, float z);
	void playClimbSound(float x, float y, float z);

	std::map<Ogre::String, ObjectAudio*> dynamicsAudio;

	struct LocalSound
	{
		irrklang::ISound* s;
		float x, y, z;

		bool velocityVolume;
		float velocityVolumeMul;
		float velocityVolumeMax;
	};

	void addLocalSound(LocalSound sound);
	void removeLocalSound(irrklang::ISound* audio);

private:

	std::vector<LocalSound> localSounds;

	int lastSlidingId = -1;
	irrklang::ISound* surfaceSlideAudio = nullptr;

	bool wasSliding = false;
	irrklang::ISound* slideAudio = nullptr;

	static const int MaterialsTypesMax = 8;

	void loadMaterialAudio();
	void loadMoveAudio();

	std::vector< std::vector<irrklang::ISoundSource*> > movementAudio;
	std::vector<irrklang::ISoundSource* > fallAudio;
	irrklang::ISoundSource* hurtAudio;
	irrklang::ISoundSource* climbAudio;

};