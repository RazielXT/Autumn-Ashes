#include "stdafx.h"
#include "DefaultObjectContactCallback.h"
#include "BodyUserData.h"

DefaultObjectContactCallback::DefaultObjectContactCallback() : OgreNewt::ContactCallback()
{
	soundEngine = Global::soundEngine;
	Global::audioLib->physicsAudio = &audio;
}

DefaultObjectContactCallback::~DefaultObjectContactCallback(void)
{
}

int DefaultObjectContactCallback::onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex )
{
    return 1;
}

Ogre::String getRandomSound(std::vector<Ogre::String>& sounds)
{
	int rand = (int)Ogre::Math::RangeRandom(0, sounds.size() - 0.01f);
	return sounds[rand];
}

inline Ogre::String getHitAudio(OgreNewt::Body* body0, OgreNewt::Body* body1, bool highHit)
{
	Ogre::Any any = body0->getUserData();
	bodyUserData* bUD = nullptr;

	if (!any.isEmpty())
	{
		bUD = Ogre::any_cast<bodyUserData*>(any);
	}
	else
	{
		any = body1->getUserData();

		if (!any.isEmpty())
			bUD = Ogre::any_cast<bodyUserData*>(any);
	}

	if (bUD && bUD->sounds)
	{
		return getRandomSound(highHit ? bUD->sounds->highHitAudio : bUD->sounds->lowHitAudio);
	}
	else
		return Ogre::String();
}

void DefaultObjectContactCallback::contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex )
{
    float speed = abs(contactJoint.getFirstContact().getNormalSpeed());
	float distance = 0;
	Ogre::String sound;

	if (speed > 11 && audio.fallSoundOffsetH <= 0)
	{
		distance = 5;
		audio.fallSoundOffsetH = 0.25f;
		audio.fallSoundOffsetL = 0.1f;
	}
	else if (speed > 6 && audio.fallSoundOffsetL <= 0)
	{
		distance = 1;
		audio.fallSoundOffsetL = 0.15f;
	}

	if (distance)
	{
		sound = getHitAudio(contactJoint.getBody0(), contactJoint.getBody1(), distance == 5);
	}

	if (!sound.empty())
	{
		Ogre::Vector3 ppos, norm;
		contactJoint.getFirstContact().getPositionAndNormal(ppos, norm);
		irrklang::ISound* s = soundEngine->play3D(sound.c_str(), irrklang::vec3df(ppos.x, ppos.y, ppos.z), false, false, true, irrklang::ESM_AUTO_DETECT, false);
		s->setMinDistance(distance);
		s->drop();
	}
}

