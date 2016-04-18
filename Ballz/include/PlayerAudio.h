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

	void surfaceSliding(int groundId = -1);

	void playWalkingSound(float x, float y, float z, int groundID, float volume = 1.0f);
	void playFallSound(float x, float y, float z, int groundID);
	void playHurtSound(float x, float y, float z);
	void playClimbSound(float x, float y, float z);

	std::map<Ogre::String, ObjectAudio*> dynamicsAudio;

private:

	struct LocalSound
	{
		irrklang::ISound* s;
		float x, y, z;

		bool velocityVolume = false;
		float velocityMul;
		float velocityMax;
	};
	std::vector<LocalSound> localSounds;

	int lastSlidingId = -1;

	static const int MaterialsTypesMax = 8;

	void loadMaterialAudio();
	void loadMoveAudio();

	std::vector< std::vector<irrklang::ISoundSource*>* > movementAudio;
	std::vector<irrklang::ISoundSource* > fallAudio;
	irrklang::ISoundSource* hurtAudio;
	irrklang::ISoundSource* climbAudio;

};