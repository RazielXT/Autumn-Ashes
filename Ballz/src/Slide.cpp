#include "stdafx.h"
#include "Slide.h"
#include "Player.h"
#include "MUtils.h"
#include "GUtils.h"

using namespace Ogre;

Slide::~Slide()
{
}

void Slide::releasedKey(const OIS::KeyEvent &arg)
{
	if (!sliding)
		return;

	if (arg.key == OIS::KC_LSHIFT)
	{
		sprint = false;
	}
}

void Slide::pressedKey(const OIS::KeyEvent &arg)
{
	if (!sliding)
		return;

	if (arg.key == OIS::KC_SPACE && unavailableTimer<0)
	{
		release(true, true);
	}

	if (arg.key == OIS::KC_C)
	{
		release();
	}

	if (arg.key == OIS::KC_LSHIFT)
	{
		sprint = true;
	}
}

void Slide::movedMouse(const OIS::MouseEvent &e)
{
	if (!sliding || sprint)
		return;

	float mod = Global::timestep / -10.0f;
	float mouseX = e.state.X.rel*mod;
	float mouseY = e.state.Y.rel*mod;

	const float maxAngle = 85;

	headState.pitch = Math::Clamp(headState.pitch + mouseX, -maxAngle, maxAngle);
	headState.yaw = Math::Clamp(headState.yaw + mouseY, -maxAngle, maxAngle);
}

void Slide::instantDetach(bool returnControl)
{
	release(returnControl);
}

void Slide::startJumpToSlide()
{
	auto target = getCurrentState().getTranslate();
	target.y += head->getPosition().y;

	headArrival.initializeJump(target);

	jumpingToSlide = true;

	float shakeW = std::min(1.0f, headArrival.dist*0.8f);
	Global::player->pCamera->shaker.startShaking(shakeW*0.8f, shakeW*1.0f, 0.5f, 1, 1, 0.4f, 0.25f, 1, true);
}

void Slide::updateJumpToSlide(float time)
{
	if (headArrival.updateJump(time))
	{
		jumpingToSlide = false;

		attach(false,0.2f);
	}
}

bool Slide::start(bool withJump)
{
	auto pos = Global::player->bodyPosition;
	pos.y += 1.5f;

	return start(pos, withJump);
}

bool Slide::start(Vector3& pos, bool withJump)
{
	if (sliding || unavailableTimer > 0)
		return false;

	if (mTrackerState == nullptr)
		mTrackerState = Global::sceneMgr->createAnimationState(animName);

	if (startFromPosition(pos))
	{
		setCorrectDirection(bidirectional);
		removeControlFromPlayer();

		/*
		auto pdir = Global::player->getFacingDirection();
		auto slDir = getDirectionState()*Vector3(0, 0, -1);
		currentSpeed = Global::player->bodyVelocityL * std::max(0.0f, pdir.dotProduct(slDir));//  Global::player->bodyVelocity / avgSpeed;
		*/
		currentSpeed = avgSpeed;

		if (withJump)
			startJumpToSlide();
		else
			attach(false);

		Global::player->pSliding->setSlide(this);

		return true;
	}

	return false;
}

bool Slide::start(float startOffset, bool withJump, float headArrivalTimer)
{
	if (sliding && unavailableTimer<=0)
		return false;

	setCorrectDirection(bidirectional, startOffset);

	currentSpeed = avgSpeed;
	updateSlidingSpeed(1.0f);

	removeControlFromPlayer();

	Global::player->pSliding->setSlide(this);

	if (withJump)
		startJumpToSlide();
	else
		attach(false, headArrivalTimer);

	return true;
}

void Slide::updateSlidingSpeed(float time)
{
	//auto verticalDir = tracker->getOrientation().getPitch().valueRadians();
	//auto dir = tracker->getOrientation()*Vector3(0, 0, -1);
	//currentSpeed = Math::Clamp(currentSpeed + -dir.y*0.5f*time, 1.0f, 2.5f);

	auto dir = tracker->getOrientation()*Vector3(0, 0, -1);
	float g = dir.y < 0 ? (1 + dir.y) : 0.5f * (1 - dir.y);

	if (sprint)
		g += 1.0f;

	g *= avgSpeed;

	float nextSpeed = currentSpeed + time*g;

	currentSpeed = nextSpeed;

	//if (walkable)
	//    currentSpeed = 0;
}

void Slide::removeControlFromPlayer()
{
	Global::player->enableControl(false);
}

void Slide::attach(bool retainDirection, float headArrivalTime)
{
	headArrivalTime = 0.5f;
	resetHead();

	{
		Ogre::Camera* cam = Global::camera->cam;

		auto state = getCurrentState();
		auto pDir = cam->getDerivedOrientation();
		auto slDir = getDirectionState();

		if (retainDirection)
		{
			headState.pitch = MUtils::getYawBetween(pDir, slDir);
			headState.yaw = MUtils::getPitchBetween(pDir, slDir);
		}
		else
		{
			headState.pitch = headState.yaw = 0;
		}

		headArrival.initializeTransition(state.getTranslate() + head->getPosition(), headArrivalTime);
	}

	unavailableTimer = headArrivalTime;

	Global::player->pCamera->shaker.nodHead(1);
	Global::player->pSliding->slidingStarted();

	mTrackerState->setEnabled(true);
	mTrackerState->setLoop(loop);
	sliding = true;
	sprint = false;
}

void Slide::release(bool returnControl, bool manualJump)
{
	if (returnControl)
	{
		unavailableTimer = 0.5f;

		//GUtils::DebugPrint(Ogre::StringConverter::toString(head->_getDerivedPosition()));
		Global::player->body->setPositionOrientation(head->_getDerivedPosition(), Ogre::Quaternion::IDENTITY);
		//Global::camera->attachCamera();
		Global::player->pCamera->attachCameraWithTransition(0.2f);

		float releaseSpeed = pow(std::max(1.0f, currentSpeed * 0.5f), 0.75f);

		if (!manualJump)
			Global::player->body->setVelocity(getDirectionState()*Vector3(0, 0, -1 * releaseSpeed) + Vector3(0, 3, 0));
		else
			Global::player->body->setVelocity(10 * Global::camera->direction + Vector3(0, 10, 0));

		Global::player->pCamera->shaker.nodHead(1);
		//Global::camera->shaker.startShaking(0.8f, 1.0f, 0.5f, 1, 1, 0.4f, 0.25f, 1, true);

		enablePlayerControl = true;
	}

	mTrackerState->setEnabled(false);

	sliding = false;
	Global::player->pSliding->slidingEnd();
}

void Slide::updateHeadArrival(float time)
{
	bool finished = headArrival.updateTransition(time);

	Quaternion qpitch = Quaternion(Degree(headState.pitch), Vector3(0, 1, 0));
	Quaternion qyaw = Quaternion(Degree(headState.yaw), Vector3(1, 0, 0));
	Quaternion qCam = qpitch*qyaw*Global::player->pCamera->shaker.current;

	if (finished)
	{
		Ogre::Camera* cam = Global::camera->cam;
		head->attachObject(cam);
		head->setOrientation(qCam);
	}
	else
	{
		Quaternion qCur = head->_getDerivedOrientation()*qCam;
		headArrival.refreshTransition(qCur, head->_getDerivedPosition());
	}
}

void Slide::updateSlidingCamera(float time)
{
	if (headArrival.timer > 0)
		updateHeadArrival(time);
	else
	{
		float headDif = abs(headState.pitch) + abs(headState.yaw);

		if (sprint && headDif>0)
		{
			float pW = time * 250 * headState.pitch / headDif;
			if (headState.pitch>0)
				headState.pitch = std::max(0.0f, headState.pitch - pW);
			else
				headState.pitch = std::min(0.0f, headState.pitch - pW);

			float yW = time * 90 * headState.yaw / headDif;
			if (headState.yaw > 0)
				headState.yaw = std::max(0.0f, headState.yaw - yW);
			else
				headState.yaw = std::min(0.0f, headState.yaw - yW);
		}

		Quaternion qpitch = Quaternion(Degree(headState.pitch), Vector3(0, 1, 0));
		Quaternion qyaw = Quaternion(Degree(headState.yaw), Vector3(1, 0, 0));

		head->setOrientation(qpitch*qyaw*Global::player->pCamera->shaker.current);
	}
}

void Slide::updateSlidingState(float time)
{
	auto thisPos = tracker->getPosition();
	realSpeed = lastPos.distance(thisPos) / time;

	//auto log = Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log");
	//log->logMessage("LINE SPEED " + std::to_string(realSpeed), Ogre::LML_NORMAL);

	updateSlidingSpeed(time);

	updateTrack(time*currentSpeed);

	updateSlidingCamera(time);

	//past/near end
	if (!loop && mTrackerState->hasEnded())
	{
		release();
	}

	lastPos = tracker->getPosition();
}


bool Slide::update(Ogre::Real tslf)
{
	tslf *= Global::timestep;

	if (sliding)
		updateSlidingState(tslf);
	else if (jumpingToSlide)
		updateJumpToSlide(tslf);

	if (unavailableTimer>0)
		unavailableTimer -= tslf;

	if (enablePlayerControl)
	{
		Global::player->enableControl(true);
		enablePlayerControl = false;
	}

	return jumpingToSlide || sliding || unavailableTimer>0;
}