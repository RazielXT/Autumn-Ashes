#pragma once
#include "stdafx.h"
#include "Slide.h"

class WalkableSlide : public Slide
{

public:

    WalkableSlide(const std::string& zipName, bool looped, bool isWalkable, float speed, bool slidingAllowed = true) : Slide(zipName, looped, isWalkable, speed), allowSliding(slidingAllowed) {};
    virtual ~WalkableSlide();

protected:

    bool allowSliding;
};