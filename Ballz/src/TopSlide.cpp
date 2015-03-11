#include "stdafx.h"
#include "TopSlide.h"

TopSlide::TopSlide(SceneNode* node, const std::string& zipAnimName, bool looped, float speed) : Slide(zipAnimName, looped, speed)
{
	tracker = node;
	base = tracker->createChildSceneNode();
	head = base->createChildSceneNode();
	head->setPosition(0, 2.5f, 0);
	avgSpeed *= 5;

	initSlide();
}
