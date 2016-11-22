#include "stdafx.h"
#include "AnimatedTrack.h"
#include "MUtils.h"
#include "Player.h"

using namespace Ogre;

AnimatedTrack::AnimatedTrack(const std::string& animTrackName)
{
	animName = animTrackName;
}

AnimatedTrack::~AnimatedTrack()
{

}

void AnimatedTrack::updateTrack(Ogre::Real time)
{
	mTrackerState->addTime(time);
}

inline void fixSpline(Quaternion& rotation, Quaternion previous)
{
	float fCos = previous.Dot(rotation);
	if (fCos < 0.0f)
		rotation = -rotation;
}

void AnimatedTrack::initTrack(const std::string& initAnimName)
{
	Animation* anim = Global::sceneMgr->getAnimation(initAnimName);
	auto o_track = anim->getNodeTrack(0);
	float realLength = 0;

	for (unsigned short i = 1; i < o_track->getNumKeyFrames(); i++)
	{
		auto pKeyFrame = o_track->getNodeKeyFrame(i - 1);
		auto keyFrame = o_track->getNodeKeyFrame(i);
		realLength += pKeyFrame->getTranslate().distance(keyFrame->getTranslate());
	}

	auto mod = realLength / anim->getLength();

	trackPoints.clear();
	trackPoints.resize(o_track->getNumKeyFrames());

	animName = "gen_" + initAnimName;
	Animation* newAnim = Global::sceneMgr->createAnimation(animName, realLength);
	newAnim->setInterpolationMode(Animation::IM_SPLINE);

	track = newAnim->createNodeTrack(0, tracker);
	track->setUseShortestRotationPath(true);

	Quaternion previous;
	Quaternion yawMe(Degree(-90), Vector3(0, 1, 0));

	for (unsigned short i = 0; i < o_track->getNumKeyFrames(); i++)
	{
		SlidePoint& point = trackPoints[i];
		auto keyFrame = o_track->getNodeKeyFrame(i);

		Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(keyFrame->getTime()*mod);
		kf->setTranslate(keyFrame->getTranslate());

		point.pos = kf->getTranslate();
		point.startOffset = kf->getTime();

		//slerp hotfix
		auto rotation = keyFrame->getRotation();
		rotation = rotation*yawMe;

		if (i > 0)
		{
			fixSpline(rotation, previous);

			keyFrame->setRotation(rotation);
		}

		kf->setRotation(keyFrame->getRotation());

		previous = rotation;
	}
}

void AnimatedTrack::initTrack(const std::vector<Ogre::Vector3>& points)
{
	trackPoints.clear();
	trackPoints.resize(points.size());

	std::vector<Vector3> pointsDir;

	for (size_t i = 0; i < points.size(); i++)
	{
		SlidePoint& point = trackPoints[i];
		point.pos = points[i];

		Vector3 dir;

		//first 2
		if (i == 0 || i == 1)
			dir = (points[1] - points[0]);
		//last 2
		else if (i >= points.size() - 2)
			dir = (points[points.size() - 1] - points[points.size() - 2]);
		//else inside
		else
		{
			auto dirB = (points[i] - points[i - 1]);
			dirB.normalise();
			auto dirF = (points[i + 1] - points[i]);
			dirF.normalise();

			dir = (dirB + dirF);
		}

		dir.normalise();

		pointsDir.push_back(dir);
	}

	trackPoints[0].startOffset = 0;
	float timer = 0;
	for (size_t i = 1; i < points.size(); i++)
	{
		timer += points[i - 1].distance(points[i]);
		trackPoints[i].startOffset = timer;
	}

	Animation* anim;

	if (Global::sceneMgr->hasAnimation(animName))
	{
		anim = Global::sceneMgr->getAnimation(animName);
		track->removeAllKeyFrames();

		mTrackerState = nullptr;
		Global::sceneMgr->destroyAnimationState(animName);
	}
	else
	{
		anim = Global::sceneMgr->createAnimation(animName, timer);
		anim->setInterpolationMode(Animation::IM_LINEAR);

		track = anim->createNodeTrack(0, tracker);
		track->setUseShortestRotationPath(true);
	}


	Quaternion previous;

	for (size_t i = 0; i < points.size(); i++)
	{
		Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(trackPoints[i].startOffset);
		kf->setTranslate(points[i]);

		auto& dir = pointsDir[i];
		auto q = Vector3(0, 0, -1).getRotationTo(Vector3(dir.x, 0, dir.z));
		auto q2 = Vector3(0, 0, -1).getRotationTo(Vector3(0, dir.y, -1));

		auto rotation = q*q2;

		//slerp hotfix by http://ogre3d.org/forums/viewtopic.php?f=2&t=47636
		if (i > 0)
		{
			fixSpline(rotation, previous);
		}

		kf->setRotation(rotation);

		previous = rotation;
	}
}

void AnimatedTrack::invertTrack()
{
	std::vector<Ogre::Vector3> newPoints;

	for (size_t i = trackPoints.size(); i > 0; i--)
	{
		newPoints.push_back(trackPoints[i - 1].pos);
	}

	//todo slide tempNode = nullptr;
	initTrack(newPoints);
}

void AnimatedTrack::setCorrectDirection(bool bidirectional, float startOffset)
{
	if (startOffset < 0)
		startOffset = mTrackerState->getTimePosition();

	if (bidirectional)
	{
		auto pdir = Global::camera->direction;
		auto slDir = getDirectionState(startOffset)*Vector3(0, 0, -1);

		if (pdir.dotProduct(slDir) < 0)
		{
			Animation* anim = Global::sceneMgr->getAnimation(animName);
			startOffset = anim->getLength() - startOffset;
			invertTrack();
		}
	}

	if (mTrackerState == nullptr)
		mTrackerState = Global::sceneMgr->createAnimationState(animName);

	mTrackerState->setTimePosition(startOffset);
}

Ogre::Vector3 AnimatedTrack::getTrackPosition(float timeOffset)
{
	Ogre::TransformKeyFrame key(0, 0);
	track->getInterpolatedKeyFrame(timeOffset, &key);

	return key.getTranslate();
}

Ogre::Quaternion AnimatedTrack::getDirectionState(float offset)
{
	Ogre::TransformKeyFrame key(0, 0);
	track->getInterpolatedKeyFrame(offset, &key);

	return key.getRotation();
}

Ogre::Quaternion AnimatedTrack::getDirectionState()
{
	Ogre::TransformKeyFrame key(0, 0);
	track->getInterpolatedKeyFrame(mTrackerState->getTimePosition(), &key);

	return key.getRotation();
}

Ogre::TransformKeyFrame AnimatedTrack::getCurrentState()
{
	Ogre::TransformKeyFrame key(0, 0);
	track->getInterpolatedKeyFrame(mTrackerState->getTimePosition(), &key);

	return key;
}

#define MAX_PLAYER_DISTANCE_SQ 4*4

bool AnimatedTrack::startFromPosition(Ogre::Vector3& point)
{
	//auto log = Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log");

	auto zipPos = trackPoints[0];
	float minDist = MAX_PLAYER_DISTANCE_SQ;

	//log->logMessage("START------TRY TO PLACE POINT ON LINE", Ogre::LML_NORMAL);

	for (size_t id = 1; id < trackPoints.size(); id++)
	{
		auto state = MUtils::getProjectedPointOnLine(point, trackPoints[id - 1].pos, trackPoints[id].pos);

		//log->logMessage(std::to_string(id) + ". state: " + Ogre::StringConverter::toString(state.projPos) + " distance " + Ogre::StringConverter::toString(state.sqMinDistance), Ogre::LML_NORMAL);
		//log->logMessage(std::to_string(id) + ". line: " + Ogre::StringConverter::toString(slidePoints[id - 1].pos) + " to " + Ogre::StringConverter::toString(slidePoints[id].pos), Ogre::LML_NORMAL);

		if (state.sqMinDistance < minDist)
		{
			//log->logMessage("ACCEPTED");

			auto timePos = trackPoints[id - 1].startOffset;
			timePos += state.projPos.distance(trackPoints[id - 1].pos);

			mTrackerState->setTimePosition(timePos);

			minDist = state.sqMinDistance;
		}
	}

	//log->logMessage("END------TRY TO PLACE POINT ON LINE", Ogre::LML_NORMAL);

	return (minDist != MAX_PLAYER_DISTANCE_SQ);
}