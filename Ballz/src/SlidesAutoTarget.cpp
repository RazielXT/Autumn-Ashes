#include "stdafx.h"
#include "SlidesAutoTarget.h"
#include "MathUtils.h"


SlidesAutoTargetAsync::SlidesAutoTargetAsync()
{

    targetBillboardSet = Global::mSceneMgr->createBillboardSet("autoTargetBillSet");
    targetBillboardSet->setMaterialName("chimneySmoke");
    auto targetBillboard = targetBillboardSet->createBillboard(Vector3(0, 0, 0));
    targetBillboard->setDimensions(5, 5);

    billboardNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    billboardNode->attachObject(targetBillboardSet);

    targetBillboardSet->setVisible(false);
    targetBillboardSet->setCastShadows(false);
    targetBillboardSet->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);

    targetTimer = 0;
}

bool SlidesAutoTargetAsync::pressedAction()
{
    if (targetInfo.targetSlide)
    {
        return targetInfo.targetSlide->start(targetInfo.targetSlidePosOffset, true);
    }

    return false;
}

SlidesAutoTargetAsync::~SlidesAutoTargetAsync()
{
    if (targetResult.valid())
        targetResult.get();
}

bool SlidesAutoTargetAsync::getTargetSlideFunc(Vector3 pos, Vector3 dir, float rayDistance, Slide* ignoredSlide)
{
    const float rayRadiusSq = 6*6;

    //radius 6 at distance 30 (3 at 15 etc)
    const float minRayRadiusW = rayRadiusSq * rayDistance / 30.0f;

    pos = pos + dir * 2;
    auto target = pos + dir*rayDistance;

    float closest = rayRadiusSq;

    for (auto s : loadedSlides)
    {
        if (s == ignoredSlide)
            continue;

        int foundSegmentId = -1;
        float foundSegmentPos = 1;

        for (size_t i = 1; i < s->slidePoints.size(); i++)
        {
            auto s0 = s->slidePoints[i - 1].pos;
            auto s1 = s->slidePoints[i].pos;

            auto r = MathUtils::getSegmentsDistanceInfo(pos, target, s0, s1);
            float minCompDist = minRayRadiusW*r.s1Pos;

            if (r.sqMinDistance<minCompDist && r.sqMinDistance<closest)
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

            auto s0 = s->slidePoints[foundSegmentId - 1].startOffset;
            auto s1 = s->slidePoints[foundSegmentId].startOffset;
            preparedSlideOffset = s0 + (s1-s0)*foundSegmentPos;
        }

    }

    return closest<rayRadiusSq;
}

void SlidesAutoTargetAsync::updateAutoTarget(Vector3 pos, Vector3 dir, float tslf, float rayDistance, Slide* ignoredSlide)
{
    auto found = targetResult.valid() ? targetResult.get() : false;

    if (found)
    {
        targetInfo.targetSlide = preparedSlide;
        targetInfo.targetSlidePosOffset = preparedSlideOffset;
        targetInfo.targetSlidePos = targetInfo.targetSlide->getTrackPosition(targetInfo.targetSlidePosOffset);

        //Global::gameMgr->myMenu->showUseGui(Ui_Target);
        targetBillboardSet->setVisible(true);
        billboardNode->setPosition(targetInfo.targetSlidePos);
    }
    else
    {
        targetTimer -= tslf;

        if (targetTimer < 0)
        {
            targetBillboardSet->setVisible(false);
            targetInfo.targetSlide = nullptr;
        }
    }

    preparedSlide = nullptr;
    targetResult = std::async(std::launch::async, &SlidesAutoTargetAsync::getTargetSlideFunc, this, pos, dir, rayDistance, ignoredSlide);
}