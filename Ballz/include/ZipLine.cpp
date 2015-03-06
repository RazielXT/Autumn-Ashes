#include "stdafx.h"
#include "ZipLine.h"
#include "Player.h"

using namespace Ogre;

void ZipLine::pressedKey(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_SPACE)
    {

    }
}

ZipLine::ZipLine(const std::vector<Ogre::Vector3>& points)
{
    initZipLine(points);
}

void ZipLine::initZipLine(const std::vector<Ogre::Vector3>& points)
{
    zipLine.clear();
    zipLine.resize(points.size());

    for (int i = 0; i < points.size(); i++)
    {
        ZipLinePoint& point = zipLine[i];
        point.pos = points[i];

        //first 2
        if (i == 0 || i == 1)
            point.dir = (points[1] - points[0]);
        //last 2
        else if (i >= points.size() - 2)
            point.dir = (points[points.size() - 1] - points[points.size() - 2]);
        //else inside
        else
        {
            auto dirB = (points[i] - points[i - 1]);
            dirB.normalise();
            auto dirF = (points[i + 1] - points[i]);
            dirF.normalise();

            point.dir = (dirB + dirF);
        }

        point.dir.normalise();
    }

	zipLine[0].startOffset=0;
	float timer = 0;
	for (int i = 1; i < points.size(); i++)
	{
		timer += points[i - 1].distance(points[i]) / avgSpeed;
		zipLine[i].startOffset = timer;
	}

	Animation* anim = Global::mSceneMgr->createAnimation(name, timer);
	anim->setInterpolationMode(Animation::IM_SPLINE);

	NodeAnimationTrack* track = anim->createNodeTrack(0, tracker);

	for (int i = 0; i < points.size(); i++)
	{
		Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(zipLine[i].startOffset);
		kf->setTranslate(points[i]);
	}
}

ZipLine::LineProjState ZipLine::getProjectedState(Ogre::Vector3& point, Ogre::Vector3& start, Ogre::Vector3& end)
{
    Vector3 dir = end - start;
    Vector3 pDir = point - start;
    dir.normalise();
    auto dp = pDir.dotProduct(dir);
    dir *= dp;

    //auto lToP = dir.length();
   // auto lleft = (end - dir).length();

    LineProjState state;
    state.projPos = dir;
    state.sqMinDistance = std::min(point.squaredDistance(end), std::min(point.squaredDistance(start), point.squaredDistance(state.projPos)));

    return state;
}

#define MAX_PLAYER_DISTANCE 5

bool ZipLine::placePointOnLine(Vector3& point)
{
    auto zipPos = zipLine[0];
    float minDist = MAX_PLAYER_DISTANCE;

    for (int id = 1; id < zipLine.size(); id++)
    {
        auto state = getProjectedState(point, zipLine[id - 1].pos, zipLine[id].pos);

        if (state.sqMinDistance < minDist)
        {
			auto timePos = zipLine[id - 1].startOffset;
			timePos += state.projPos.distance(zipLine[id-1].pos)/avgSpeed;

			mTrackerState->setTimePosition(timePos);

            minDist = state.sqMinDistance;
        }
    }

    return (minDist != MAX_PLAYER_DISTANCE);
}

bool ZipLine::start()
{
    auto pos = Global::player->body->getPosition();

    if (placePointOnLine(pos))
    {
        //TODO attach player
		if (mTrackerState==nullptr)
			mTrackerState = Global::mSceneMgr->createAnimationState(name);

		mTrackerState->setEnabled(true);
		mTrackerState->setLoop(loop);

		//TODO figure out start speed, based on body speed
		currentSpeed = 1;
        active = true;

        return true;
    }

    return false;
}

void ZipLine::updateSlidingSpeed(float time)
{
	currentSpeed = 1;
}

void ZipLine::release()
{
    active = false;
}

void ZipLine::updateSlidingState(float time)
{
    updateSlidingSpeed(time);

	mTrackerState->addTime(time*currentSpeed);

    //past/near end
    if (!loop && mTrackerState->hasEnded())
        release();
}


bool ZipLine::update(Ogre::Real tslf)
{
    updateSlidingState(tslf);

    return active;
}