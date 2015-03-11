#include "stdafx.h"
#include "TopSlide.h"

TopSlide::TopSlide(SceneNode* node, const std::string& zipAnimName, bool looped, float speed) : Slide(zipAnimName, looped, speed)
{
    tracker = node;
    base = tracker->createChildSceneNode();
    head = base->createChildSceneNode();

    resetHead();

    initSlide();
}

TopSlide::TopSlide(const std::vector<Ogre::Vector3>& points, const std::string& zipName, bool looped, float speed) : Slide(zipName, looped, speed)
{
    tracker = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    base = tracker->createChildSceneNode();
    head = base->createChildSceneNode();

    resetHead();

    initSlide(points);
}