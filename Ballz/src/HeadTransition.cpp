#include "stdafx.h"
#include "HeadTransition.h"
#include "MUtils.h"
#include "CameraShaker.h"

using namespace Ogre;

void HeadTransition::initializeJump(Ogre::Camera* cam, Ogre::Vector3 target)
{
    pos = cam->getDerivedPosition();
    dir = cam->getDerivedOrientation();
    cam->detachFromParent();

    if (transitionNode == nullptr)
    {
        transitionNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    }

    transitionNode->attachObject(cam);
    transitionNode->setPosition(pos);
    transitionNode->setOrientation(dir);


    dist = std::max(0.5f, pos.distance(target) / 10.0f);
    dirTarget.FromAngleAxis(dir.getYaw(), Vector3(0, 1, 0));
    dirTarget = dirTarget*Quaternion(Ogre::Degree(-30), Vector3(1, 0, 0));

    posTarget = target;
    timer = 0;
}

//w^n, n = -1-1 to 3-1/3, n 0->1
float HeadTransition::transformHeightFuncTimeLow(float time, float hd)
{
    auto t = abs(hd);

    t = t * 2 + 1;

    if (hd > 0)
        t = 1 / t;

    return pow(time, t);
}

float HeadTransition::heightFuncLow(float time, float hd)
{
	float addH = MUtils::smoothjump(time, 2);

    addH *= 1 - abs(hd);

    return addH;
}

//w^n, n = -1-1 to 3-1/3, n 0->1
float HeadTransition::transformHeightFuncTime(float time, float hd)
{
    auto t = abs(hd);

    t = t * 2 + 1;

    if (hd > 0)
        t = 1 / t;

    return pow(time, t);
}

float HeadTransition::heightFunc(float time, float hd)
{
    auto x = transformHeightFuncTime(time, hd);

    float off = 1;
    float ex = 2; //2,4,6
    float addH = -pow(x*off * 2 - off, ex) + pow(off, ex);
    addH /= off * 2;

    addH *= 1 - abs(hd);

    return addH;
}

bool HeadTransition::updateJump(float time)
{
    float w = 0;
    bool beforeJump = false;

    if (timer < 0)
    {
        auto t = timer + time*2.0f;

        if (t < 0)
        {
            timer = t;
            beforeJump = true;
        }
    }

    if (!beforeJump)
    {
        timer = std::min(timer + time*2.75f, dist);
        w = timer / dist;

        //w = pow(w, 0.58f);
        //w = quickstep(w, 0.75f);
    }

    auto hDiff = pos.y - posTarget.y;
    auto tDist = pos.distance(posTarget);
    hDiff = hDiff / tDist;

    auto cpos = MUtils::lerp(pos, posTarget, w);
    auto cdir = Quaternion::nlerp(w, dir, dirTarget, true);

    auto maxH = tDist / 2.0f;
    auto hAdd = heightFuncLow(w, hDiff)*maxH;
    cpos.y += hAdd;

    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Jumping: hadd " + Ogre::StringConverter::toString(hAdd) + ", hd " + Ogre::StringConverter::toString(hd), Ogre::LML_NORMAL);

    transitionNode->setPosition(cpos);
    transitionNode->setOrientation(cdir*Global::shaker->current);

    return (timer == dist);
}

void HeadTransition::initializeTransition(Ogre::Camera* cam, Ogre::Vector3 target, float transitionTime)
{
    timer = transitionTime;
    posTarget = target;
    pos = cam->getDerivedPosition();
    dir = cam->getDerivedOrientation();

    if (transitionNode == nullptr)
    {
        transitionNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    }

    cam->detachFromParent();
    transitionNode->attachObject(cam);

    transitionNode->setPosition(pos);
    transitionNode->setOrientation(dir);
}

bool HeadTransition::updateTransition(float tslf)
{
    timer -= tslf;

    if (timer < 0)
    {
        transitionNode->detachAllObjects();

        return true;
    }

    return false;
}

void HeadTransition::refreshTransition(Ogre::Quaternion actualOr, Ogre::Vector3 actualPos)
{
    auto w = timer;

    Quaternion q = Quaternion::nlerp(1 - w, dir, actualOr, true);
    transitionNode->setOrientation(q);

    Vector3 moveOffset = actualPos - posTarget;
    Vector3 p = w*pos + (1 - w)*posTarget;
    transitionNode->setPosition(p + moveOffset);
}
