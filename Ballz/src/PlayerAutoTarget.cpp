#include "stdafx.h"
#include "PlayerAutoTarget.h"
#include "MUtils.h"
#include "Slide.h"
#include "JumpBox.h"

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
	if (targetInfo.targetSlide && targetInfo.targetSlidePosOffset != -1)
	{
		return targetInfo.targetSlide->start(targetInfo.targetSlidePosOffset, true);
	}
	else
	{
		objects.jumpBoxes[0].jumpToBox();
		return true;
	}

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

bool PlayerAutoTarget::getTargetSlideRay(Vector3 pos, Vector3 dir, float rayDistance, Slide* ignoredSlide)
{
	const float rayRadiusSq = 6 * 6;

	//radius 6 at distance 30 (3 at 15 etc)
	const float minRayRadiusW = rayRadiusSq * rayDistance / 30.0f;

	Vector3 rayStart = pos + dir * 2;
	Vector3 rayTarget = rayStart + dir*rayDistance;

	float closest = rayRadiusSq;

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

			if (r.sqMinDistance < minCompDist && r.sqMinDistance < closest)
			{
				closest = r.sqMinDistance;

				foundSegmentPos = r.s2Pos;
				foundSegmentId = i;
			}
		}

		//if new was found
		if (foundSegmentId > 0)
		{
			preparedSlide = s;

			auto s0 = s->trackPoints[foundSegmentId - 1].startOffset;
			auto s1 = s->trackPoints[foundSegmentId].startOffset;
			preparedSlideOffset = s0 + (s1 - s0)*foundSegmentPos;
		}

	}

	return closest<rayRadiusSq;
}

bool PlayerAutoTarget::getTargetSlideTouch(Vector3 pos, Vector3 dir, Slide* ignoredSlide)
{
	pos.y -= 2;
	float maxDistSq = 5;
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
				preparedSlidePos = r.projPos;
				preparedSlide = s;
				preparedSlideOffset = -1;
			}
		}
	}

	return closest < maxDistSq;
}

bool PlayerAutoTarget::getTargetFunc(Vector3 pos, Vector3 dir, float rayDistance)
{
	if (!slidingState.autoAttach)
		return getTargetSlideRay(pos, dir, rayDistance, slidingState.currentSlide);
	else
		return getTargetSlideTouch(pos, dir, slidingState.currentSlide);
}

void PlayerAutoTarget::updateAutoTarget(Vector3 pos, Vector3 dir, float tslf, float rayDistance)
{
	auto found = targetResult.valid() ? targetResult.get() : false;

	if (found)
	{
		targetInfo.targetSlide = preparedSlide;
		targetInfo.targetSlidePosOffset = preparedSlideOffset;
		targetInfo.targetSlidePos = (preparedSlideOffset==-1) ? preparedSlidePos : targetInfo.targetSlide->getTrackPosition(targetInfo.targetSlidePosOffset);

		if (preparedSlideOffset != -1)
		{
			//Global::gameMgr->myMenu->showUseGui(Ui_Target);
		}
		else   //auto attach on touch
		{
			if (lastUnavailableSlide != targetInfo.targetSlide)
				targetInfo.targetSlide->start(targetInfo.targetSlidePos);
		}
	}
	else
	{
		targetInfo.targetSlide = nullptr;
	}

	if (slidingState.autoAttach && lastUnavailableSlide != targetInfo.targetSlide)
		lastUnavailableSlide = nullptr;

	preparedSlide = nullptr;
	targetResult = std::async(std::launch::async, &PlayerAutoTarget::getTargetFunc, this, pos, dir, rayDistance);
}