#include "stdafx.h"
#include "ZipLine.h"
#include "Player.h"
#include "MathUtils.h"

using namespace Ogre;

void ZipLine::pressedKey(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_SPACE)
    {
        auto jumpSpeed = Global::player->getFacingDirection() * 12;
        jumpSpeed.y = std::max(jumpSpeed.y, 5.0f);

        release();
        Global::player->body->setVelocity(jumpSpeed);
    }

    if (arg.key == OIS::KC_C)
    {
        release();
    }
}

void ZipLine::movedMouse(const OIS::MouseEvent &e)
{
	float mouseX = (-1 * e.state.X.rel*Global::timestep) / 10.0f;
	float mouseY = (-1 * e.state.Y.rel*Global::timestep) / 10.0f;

	float maxAngle = 70;

	headState.pitch += mouseY;
	if (headState.pitch > -maxAngle && headState.pitch < maxAngle)
		head->pitch(Degree(mouseY), Node::TS_LOCAL);
	else
	{
		headState.pitch = headState.pitch - mouseY;
		if (headState.pitch < 0)
		{
			head->pitch(Degree(-maxAngle - headState.pitch), Node::TS_LOCAL);
			headState.pitch = -maxAngle;
		}
		if (headState.pitch > 0)
		{
			head->pitch(Degree(maxAngle - headState.pitch), Node::TS_LOCAL);
			headState.pitch = maxAngle;
		}
	}

	headState.yaw += mouseX;
	if (headState.yaw > -maxAngle && headState.yaw < maxAngle)
		head->yaw(Degree(mouseX), Node::TS_LOCAL);
	else
	{
		headState.yaw = headState.yaw - mouseX;
		if (headState.yaw < 0)
		{
			head->yaw(Degree(-maxAngle - headState.yaw), Node::TS_LOCAL);
			headState.yaw = -maxAngle;
		}
		if (headState.yaw > 0)
		{
			head->yaw(Degree(maxAngle - headState.yaw), Node::TS_LOCAL);
			headState.yaw = maxAngle;
		}
	}
}

ZipLine::ZipLine(SceneNode* node, const std::string& zipAnimName, bool looped, float speed) : name(zipAnimName), loop(looped), avgSpeed(speed)
{
	tracker = node;
	base = tracker->createChildSceneNode();
	head = base->createChildSceneNode();
	head->setPosition(0, 2.5f, 0);
}

ZipLine::ZipLine(const std::vector<Ogre::Vector3>& points, const std::string& zipName, bool looped, float speed) : name(zipName), loop(looped), avgSpeed(speed)
{
    tracker = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    base = tracker->createChildSceneNode();
    head = base->createChildSceneNode();
    head->setPosition(0, -1.5f, 0);

    initZipLine(points);
}

void ZipLine::initZipLine()
{
	Animation* anim = Global::mSceneMgr->getAnimation(name);
	auto track = anim->getNodeTrack(0);

	zipLine.clear();
	zipLine.resize(track->getNumKeyFrames());

	Quaternion previous;

	for (size_t i = 0; i < track->getNumKeyFrames(); i++)
	{
		ZipLinePoint& point = zipLine[i];
		auto keyFrame = track->getNodeKeyFrame(i);
		point.pos = keyFrame->getTranslate();
		
		//TODO if needed
		//point.dir = keyFrame->get ??

		point.startOffset = keyFrame->getTime();

		//slerp hotfix
		auto rotation = keyFrame->getRotation();
		if (i > 0)
		{
			float fCos = previous.Dot(rotation);
			if (fCos < 0.0f)
				rotation = -rotation;

			keyFrame->setRotation(rotation);
		}
		previous = rotation;
	}
}

void ZipLine::initZipLine(const std::vector<Ogre::Vector3>& points)
{
    zipLine.clear();
    zipLine.resize(points.size());

    for (size_t i = 0; i < points.size(); i++)
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
    for (size_t i = 1; i < points.size(); i++)
    {
        timer += points[i - 1].distance(points[i]) / avgSpeed;
        zipLine[i].startOffset = timer;
    }

    Animation* anim = Global::mSceneMgr->createAnimation(name, timer);
    anim->setInterpolationMode(Animation::IM_SPLINE);

    NodeAnimationTrack* track = anim->createNodeTrack(0, tracker);

    track->setUseShortestRotationPath(true);

    Quaternion previous;
	int loopEnd = loop ? 1 : 0;

	for (size_t i = 0; i < points.size() + loopEnd; i++)
    {
		if (i == points.size())
			i = 0;

        Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(zipLine[i].startOffset);
        kf->setTranslate(points[i]);

        auto dir = zipLine[i].dir;
        auto q = Vector3(0, 0, -1).getRotationTo(Vector3(dir.x, 0, dir.z));
        auto q2 = Vector3(0, 0, -1).getRotationTo(Vector3(0, dir.y, -1));

        auto rotation = q*q2;

        //slerp hotfix by http://ogre3d.org/forums/viewtopic.php?f=2&t=47636
        if (i > 0)
        {
            float fCos = previous.Dot(rotation);
            if (fCos < 0.0f)
                rotation = -rotation;
        }

        kf->setRotation(rotation);

        previous = rotation;
    }
}

#define MAX_PLAYER_DISTANCE_SQ 5*5

bool ZipLine::placePointOnLine(Vector3& point)
{
    auto zipPos = zipLine[0];
    float minDist = MAX_PLAYER_DISTANCE_SQ;

    for (size_t id = 1; id < zipLine.size(); id++)
    {
        auto state = MathUtils::getProjectedState(point, zipLine[id - 1].pos, zipLine[id].pos);

        if (state.sqMinDistance < minDist)
        {
            auto timePos = zipLine[id - 1].startOffset;
            timePos += state.projPos.distance(zipLine[id-1].pos)/avgSpeed;

            mTrackerState->setTimePosition(timePos);

            minDist = state.sqMinDistance;
        }
    }

    return (minDist != MAX_PLAYER_DISTANCE_SQ);
}

bool ZipLine::start()
{
    if (active || unavailableTimer>0)
        return false;

    //auto pos = Global::mSceneMgr->getSceneNode("Test")->getPosition();// zipLine[0].pos;//
    auto pos = Global::player->body->getPosition();
    pos.y += 1.5f;

    if (mTrackerState == nullptr)
        mTrackerState = Global::mSceneMgr->createAnimationState(name);

    if (placePointOnLine(pos))
    {
        attach();

		turnRollState.first = true;
        mTrackerState->setEnabled(true);
        mTrackerState->setLoop(true);

        //TODO figure out start speed, based on body speed
        currentSpeed = 0.5f;
        active = true;

        return true;
    }

    return false;
}

void ZipLine::updateSlidingSpeed(float time)
{
    //auto verticalDir = tracker->getOrientation().getPitch().valueRadians();
    auto dir = tracker->getOrientation()*Vector3(0, 0, -1);

    currentSpeed = Math::Clamp(currentSpeed + -dir.y*0.5f*time, 1.0f, 2.5f);
}

void ZipLine::attach()
{
    Ogre::Camera* cam = Global::mSceneMgr->getCamera("Camera");

    headArrival.timer = 0.5f;
    headArrival.pos = cam->getDerivedPosition();
    headArrival.dir = cam->getDerivedOrientation();

	headState.pitch = 0;
	headState.yaw = 0;

    registerInputListening();

    cam->detachFromParent();

    Global::player->enableControl(false);

    tracker->removeAllChildren();
    Global::mSceneMgr->getRootSceneNode()->addChild(base);
    head->attachObject(cam);

    Global::player->body->freeze();
}

void ZipLine::release()
{
    Global::player->attachCamera();

    unregisterInputListening();

    enablePlayerControl = true;

    Global::player->body->setPositionOrientation(tracker->getPosition(), Ogre::Quaternion::IDENTITY);
    Global::player->body->unFreeze();

    unavailableTimer = 0.5f;
    active = false;
}

void ZipLine::updateTurningRoll(float time)
{
	const float turnForce = 20;
	const float stabilityForce = 5.0f;
	const float rollLimit = 1.0f;

	auto q = tracker->getOrientation();

	if (turnRollState.first)
	{
		turnRollState.first = false;
		turnRollState.lastOr = q;
		turnRollState.headRoll = 0;
		turnRollState.torque = 0;
	}

    //force to side, faster speed means more turn
	auto force = MathUtils::getYawBetween(q, turnRollState.lastOr);
	force *= time*currentSpeed*turnForce;
   
	auto& headRoll = turnRollState.headRoll;

	turnRollState.torque -= time*headRoll*stabilityForce;

	headRoll += force + turnRollState.torque;

	/*
    //force to center
	float centerForce = time*stabilityForce;
    if (headRoll > 0)
        headRoll = std::max(0.0f, headRoll - centerForce);
    else
        headRoll = std::min(0.0f, headRoll + centerForce);
	*/

	headRoll = Math::Clamp(headRoll, -rollLimit, rollLimit);
    base->setOrientation(Quaternion(Radian(headRoll), Vector3(0, 0, 1)));

	turnRollState.lastOr = q;
    Global::debug = headRoll;
}

void ZipLine::updateHeadArrival(float time)
{
    headArrival.timer -= time;

    if (headArrival.timer <= 0)
    {
        base->resetToInitialState();

        base->getParentSceneNode()->removeChild(base);
        tracker->addChild(base);
    }
    else
    {
        auto w = headArrival.timer / 0.5f;
        Quaternion q = w*headArrival.dir + (1-w)*tracker->getOrientation();
        Vector3 p = w*headArrival.pos + (1 - w)*tracker->getPosition();

        base->setPosition(p);
        base->setOrientation(q);
    }

}

void ZipLine::updateSlidingCamera(float time)
{
    if (headArrival.timer > 0)
        updateHeadArrival(time);
    else
        updateTurningRoll(time);
}

void ZipLine::updateSlidingState(float time)
{
    updateSlidingSpeed(time);

    mTrackerState->addTime(time*currentSpeed);

    updateSlidingCamera(time);

    //past/near end
    if (!loop && mTrackerState->hasEnded())
        release();
}


bool ZipLine::update(Ogre::Real tslf)
{
    tslf *= Global::timestep;

    if (enablePlayerControl)
    {
        Global::player->enableControl(true);
        enablePlayerControl = false;
    }

    if (unavailableTimer <= 0)
        updateSlidingState(tslf);
    else
        unavailableTimer -= tslf;

    return active || unavailableTimer>0;
}