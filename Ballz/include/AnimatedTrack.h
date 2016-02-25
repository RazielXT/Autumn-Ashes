#pragma once
#include "stdafx.h"

class SlidesAutoTargetAsync;

class AnimatedTrack
{
	friend SlidesAutoTargetAsync;

public:

	AnimatedTrack(const std::string& animTrackName);
	virtual ~AnimatedTrack();

	void updateTrack(Ogre::Real tslf);

protected:

	struct SlidePoint
	{
		Ogre::Vector3 pos;
		float startOffset;
	};
	std::vector<SlidePoint> trackPoints;

	void setCorrectDirection(bool bidirectional = true, float startOffset = -1);
	void initTrack(const std::string& animName);
	void initTrack(const std::vector<Ogre::Vector3>& points);
	void invertTrack();

	Ogre::Vector3 getTrackPosition(float timeOffset);
	Ogre::Quaternion getDirectionState(float offset);
	Ogre::Quaternion getDirectionState();
	Ogre::TransformKeyFrame getCurrentState();

	bool startFromPosition(Ogre::Vector3& point);


	Ogre::NodeAnimationTrack* track;
	Ogre::AnimationState * mTrackerState = nullptr;
	Ogre::SceneNode* tracker;
	std::string animName;
};