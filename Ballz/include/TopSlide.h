#pragma once
#include "stdafx.h"
#include "Slide.h"

using namespace Ogre;

class TopSlide : public Slide
{
public:

	TopSlide(Ogre::SceneNode* node, const std::string& zipAnimName, bool looped, float speed);

protected:

};