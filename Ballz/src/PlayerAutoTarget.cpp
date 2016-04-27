#include "stdafx.h"
#include "PlayerAutoTarget.h"
#include "MUtils.h"
#include "Slide.h"
#include "JumpBox.h"
#include "Pole.h"
#include "Player.h"
#include "GameUi.h"
#include "GameStateManager.h"

const float TargetSlideMaxRayDistance = 20;
const float TargetSlideMaxProbeDistance = 6;
const float TouchSlideMaxDistance = 2;

const float TargetPoleMaxRayDistance = 20;
const float TargetPoleMaxProbeDistance = 2.5f;
const float TouchPoleMaxDistance = 2;

const float TargetJumpMaxRayDistance = 10;
const float TargetJumpMaxProbeDistance = 2.5f;

PlayerAutoTarget::PlayerAutoTarget()
{
	targetTimer = 0;
}

PlayerAutoTarget::~PlayerAutoTarget()
{
	if (targetResult.valid())
		targetResult.get();
}

bool PlayerAutoTarget::spacePressed()
{
	if (targetInfo.targetSlide)
	{
		return targetInfo.targetSlide->start(targetInfo.targetSlideOffset, true);
	}
	else if (targetInfo.targetJump)
	{
		targetInfo.targetJump->jumpToBox();
		return true;
	}
	/*else if (targetInfo.targetJump)
	{
		Global::player->pHanging->jumpTo(targetPole);

		return true;
	}*/

	return false;
}

void PlayerAutoTarget::setSlidingInfo(bool allowAutoAttach, Slide* currentSlide)
{
	slidingState.autoAttach = allowAutoAttach;
	slidingState.currentSlide = currentSlide;
}

void TargetableObjects::addLoadedSlide(Slide* slide)
{
	slides.push_back(slide);
}

void TargetableObjects::addLoadedJumpBox(JumpBox box)
{
	jumpBoxes.push_back(box);
}

void TargetableObjects::addLoadedPole(Pole pole)
{
	poles.push_back(pole);
}

bool PlayerAutoTarget::getTargetSlideRay(Vector3 pos, Vector3 dir, float rayDistance, Slide* ignoredSlide)
{
	rayDistance = std::min(rayDistance, TargetSlideMaxRayDistance);
	const float rayRadiusSq = TargetSlideMaxProbeDistance * TargetSlideMaxProbeDistance;

	//radius 6 at distance 30 (3 at 15 etc)
	const float minRayRadiusW = rayRadiusSq * rayDistance / 30.0f;

	Vector3 rayStart = pos + dir * 2;
	Vector3 rayTarget = rayStart + dir*rayDistance;

	for (auto s : objects.slides)
	{
		if (s == ignoredSlide)
			continue;

		int foundSegmentId = -1;
		float foundSegmentPos = 1;

		for (size_t i = 1; i < s->trackPoints.size(); i++)
		{
			auto s0 = s->trackPoints[i - 1].pos;
			auto s1 = s->trackPoints[i].pos;

			auto r = MUtils::getLinesDistanceInfo(rayStart, rayTarget, s0, s1);
			float minCompDist = minRayRadiusW*r.s1Pos;

			if (r.sqMinDistance < minCompDist && r.sqMinDistance < 	preparedInfo.closestTargetDistanceSq)
			{
				preparedInfo.closestTargetDistanceSq = r.sqMinDistance;

				foundSegmentPos = r.s2Pos;
				foundSegmentId = i;
			}
		}

		//if new was found
		if (foundSegmentId > 0)
		{
			preparedInfo.targetSlide = s;

			auto s0 = s->trackPoints[foundSegmentId - 1].startOffset;
			auto s1 = s->trackPoints[foundSegmentId].startOffset;
			preparedInfo.targetSlideOffset = s0 + (s1 - s0)*foundSegmentPos;
		}

	}

	return preparedInfo.targetSlide;
}

bool PlayerAutoTarget::getTargetSlideTouch(Vector3 pos, Vector3 dir, Slide* ignoredSlide)
{
	pos.y -= 2;
	const float maxDistSq = TouchSlideMaxDistance*TouchSlideMaxDistance;
	preparedInfo.closeSlide = nullptr;
	float closest = maxDistSq;

	for (auto s : objects.slides)
	{
		if (s == ignoredSlide)
			continue;

		for (size_t i = 1; i < s->trackPoints.size(); i++)
		{
			auto s0 = s->trackPoints[i - 1].pos;
			auto s1 = s->trackPoints[i].pos;

			auto r = MUtils::getProjectedPointOnLine(pos, s0, s1);

			if (r.sqMinDistance < closest)
			{
				closest = r.sqMinDistance;
				preparedInfo.closeSlidePos = r.projPos;
				preparedInfo.closeSlide = s;
			}
		}
	}

	return preparedInfo.closeSlide;
}

bool PlayerAutoTarget::getTargetPole(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayDistance)
{
	rayDistance = std::min(rayDistance, TargetPoleMaxRayDistance);

	Pole* ignored = Global::player->pHanging->currentPole;
	preparedInfo.targetPole = preparedInfo.closePole = nullptr;

	const float maxDistSq = TargetPoleMaxProbeDistance * TargetPoleMaxProbeDistance;
	const float maxTouchDistSq = TouchPoleMaxDistance * TouchPoleMaxDistance;

	for (auto& pole : objects.poles)
	{
		float pDist = pole.position.squaredDistance(pos);

		if (&pole != ignored && pDist < preparedInfo.closestTargetDistanceSq && abs(pole.pinDirection.dotProduct(dir)) < 0.8f)
		{
			auto p = MUtils::getProjectedPointOnLine(pole.position, pos, pos + dir*rayDistance);

			if (p.sqMinDistance < maxDistSq)
			{
				preparedInfo.closestTargetDistanceSq = pDist;
				preparedInfo.targetPole = &pole;
			}

			if (pDist < maxTouchDistSq)
			{
				preparedInfo.closePole = &pole;
			}
		}
	}

	return preparedInfo.targetPole != nullptr;
}

bool PlayerAutoTarget::getTargetJump(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayDistance)
{
	rayDistance = std::min(rayDistance, TargetJumpMaxRayDistance);
	preparedInfo.targetJump = nullptr;

	const float maxDistSq = TargetJumpMaxProbeDistance * TargetJumpMaxProbeDistance;

	for (auto& jump : objects.jumpBoxes)
	{
		float pDist = jump.position.squaredDistance(pos);

		if (pDist < preparedInfo.closestTargetDistanceSq && jump.faceDirection.dotProduct(dir) > 0.5f)
		{
			auto p = MUtils::getProjectedPointOnLine(jump.position, pos, pos + dir*rayDistance);

			if (p.sqMinDistance < maxDistSq)
			{
				preparedInfo.closestTargetDistanceSq = pDist;
				preparedInfo.targetJump = &jump;
			}
		}
	}

	return preparedInfo.targetJump != nullptr;
}

bool PlayerAutoTarget::getTargetFunc(Vector3 pos, Vector3 dir, float rayDistance)
{
	preparedInfo.closestTargetDistanceSq = 9999;

	getTargetPole(pos, dir, rayDistance);

	getTargetJump(pos, dir, rayDistance);

	getTargetSlideTouch(pos, dir, slidingState.currentSlide);

	return getTargetSlideRay(pos, dir, rayDistance, slidingState.currentSlide);
}

void PlayerAutoTarget::updateAutoTarget(Vector3 pos, Vector3 dir, float tslf, float rayDistance)
{
	auto found = targetResult.valid() ? targetResult.get() : false;

	targetInfo = preparedInfo;

	if (found)
	{
		//if((targetInfo.targetSlide))
		//	targetInfo.slidePos = targetInfo.targetSlide->getTrackPosition(targetInfo.slidePosOffset);

		if (targetInfo.targetSlide)
		{
			//Global::gameMgr->myMenu->showUseGui(Ui_Target);
		}
		else  //auto attach on touch
		{
			if (targetInfo.closeSlide)
				if (lastUnavailableSlide != targetInfo.closeSlide)
					targetInfo.closeSlide->start(targetInfo.closeSlidePos);
		}
	}
	else
	{
		targetInfo.targetSlide = nullptr;
	}

	if (slidingState.autoAttach && lastUnavailableSlide != targetInfo.targetSlide)
		lastUnavailableSlide = nullptr;

	if (targetInfo.targetPole)
	{
		Global::gameMgr->myMenu->gameUi->showUseGui(Ui_UseEnergy);
	}

	targetResult = std::async(std::launch::async, &PlayerAutoTarget::getTargetFunc, this, pos, dir, rayDistance);
}

