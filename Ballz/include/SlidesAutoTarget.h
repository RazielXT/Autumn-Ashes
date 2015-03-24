#pragma once

#include "stdafx.h"
#include "Slide.h"
#include <future>
#include <vector>

struct TargetSlideInfo
{
    Slide* targetSlide = nullptr;
	float targetSlidePosOffset;
    Ogre::Vector3 targetSlidePos;
};

class SlidesAutoTargetAsync
{
public:

    SlidesAutoTargetAsync();
    ~SlidesAutoTargetAsync();

    void updateAutoTarget(Vector3 pos, Vector3 dir, float tslf, Slide* ignoredSlide = nullptr);

    TargetSlideInfo targetInfo;

	std::vector<Slide*> loadedSlides;

private:

    float targetTimer;
    OgreNewt::ConvexCollisionPtr conv_col;

    std::future<bool> targetResult;
    bool getTargetSlideFunc(Vector3 pos, Vector3 dir, Slide* ignoredSlide);
};