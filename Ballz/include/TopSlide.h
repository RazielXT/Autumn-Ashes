#pragma once
#include "stdafx.h"
#include "Slide.h"

using namespace Ogre;

class TopSlide : public Slide
{
public:

    TopSlide(const std::vector<Ogre::Vector3>& points, const std::string& zipName, bool looped, float speed);

    TopSlide(Ogre::SceneNode* node, const std::string& zipName, const std::string& zipAnimName, bool looped, float speed);

    virtual bool start();

protected:

    virtual void updateSlidingSpeed(float time);

    virtual void resetHead()
    {
        head->setPosition(0, 2.5f, 0);
        head->resetOrientation();
    }
};