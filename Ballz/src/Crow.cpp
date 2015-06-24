#include "stdafx.h"
#include "Crow.h"
#include "MathUtils.h"
#include "Player.h"

using namespace Ogre;

Crow::Crow(bool onGround)
{
    //init entity+node
    mEntity = Global::mSceneMgr->createEntity("AnimatedCrow.mesh");
    mEntity->setMaterialName("crow1");
    mNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mNode->attachObject(mEntity);
    mNode->setScale(0.4f, 0.4f, 0.4f);

    animation.init(mEntity);

    animation.fadeTo("flightFast", 0);
    curModelAnimType = Flying;

    stateChangeTimer = 0;
    stateVariationTimer = 0;

    modelOr = Quaternion(Degree(-90), Vector3(0, 1, 0)) * Quaternion(Degree(90), Vector3(0, 0, 1));
    modelOffset = Vector3(0, 0.75f, 0);

    minFlightTime = 10;

    playerPos = &Global::player->bodyPosition;
}

Crow::~Crow()
{
}

void Crow::updateAnimationState()
{
    //update model anim
    if (curModelAnimType != Landing && path.state == Landing && path.getTempTimeLeft() < 0.85f)
    {
        animation.fadeTo("landing", 0.15f, false);
        curModelAnimType = Landing;
    }
    else if (curModelAnimType != Flying && (path.state == Flying || (path.state == Lifting && path.getTempTime() > 0.6f)))
    {
        animation.fadeTo("flightFast", 0.25f);
        curModelAnimType = Flying;
    }
    else if (curModelAnimType != Lifting && path.state == Lifting && path.getTempTime() < 0.6f)
    {
        animation.fadeTo("landing", 0.05f, false);
        curModelAnimType = Lifting;
    }
    else if (curModelAnimType != Standing && path.state == Standing)
    {
        animation.fadeTo("idle", 0.25f);
        curModelAnimType = Standing;
    }
    else if (curModelAnimType != Walking && path.state == Walking && stateVariationTimer < 0)
    {
        animation.fadeTo("walking", 0.1f);
        curModelAnimType = Walking;
    }
    //Global::DebugPrint(std::to_string(curAnimType), true);
}

void Crow::update(Ogre::Real tslf)
{
    //tslf /= 10.0f;
    updateAnimationState();
    animation.update(tslf);

    //update node pos
    auto lastState = path.state;

    if (path.update(tslf, mNode, modelOr, modelOffset))
    {
        if (path.state == Flying)
        {
            stateVariationTimer = switchFlightTime;

            if (lastState != SwitchFlying)
                stateChangeTimer = minFlightTime;
            else
                stateChangeTimer = std::max(stateChangeTimer, switchFlightTime / 2.0f);
        }
        else
        {
            if (lastState != Walking)
                stateChangeTimer = minGroundTime;

            if (path.state == Standing)
                scheduleWalking();
        }

    }
    else
    {
        //update state timer
        stateChangeTimer -= tslf;
        stateVariationTimer -= tslf;

        if (stateVariationTimer < 0 && path.state == Standing && allowedWalking)
            startWalking();
    }

}

bool Crow::readyToChangeFlyPath() const
{
    return (path.state == Flying && stateVariationTimer <= 0);
}

bool Crow::readyToFly() const
{
    if (!validateGroundPos(path.getPosition()))
        return true;

    return (path.state == Standing && stateChangeTimer <= 0);
}

bool Crow::readyToLand() const
{
    return (path.state == Flying && stateChangeTimer <= 0);
}

void Crow::switchFlyTo(Ogre::Animation* flightAnim)
{
    for (int i = 0; i < 3; i++)
    {
        float pos = Math::RangeRandom(0, flightAnim->getLength());

        if (validateFlightChange(flightAnim, pos))
        {
            path.setSwitchFlightAnim(flightAnim, pos);
            animation.fadeTo("flightFast", 0.2f);
            curModelAnimType = Flying;

            return;
        }
    }

    stateVariationTimer += 2;
}

bool Crow::validateFlightChange(Ogre::Animation* flightAnim, float pos)
{
    auto track = flightAnim->getNodeTrack(0);

    Ogre::TransformKeyFrame key0(0, 0);
    track->getInterpolatedKeyFrame(pos, &key0);

    //too close
    if (key0.getTranslate().squaredDistance(path.getPosition()) < 100)
        return false;

    Vector3 dir(key0.getTranslate() - path.getPosition());

    //fly away +-90 deg
    if (dir.dotProduct(key0.getRotation()*Vector3(0,0,1)) < 0)
        return false;

    return true;
}

void Crow::flyTo(Ogre::Animation* flightAnim)
{
    float pos = Math::RangeRandom(0, flightAnim->getLength());

    //start lifting
    path.setLiftingAnim(flightAnim, pos);
}

bool Crow::validateGroundPos(Ogre::Vector3 pos) const
{
    return pos.squaredDistance(*playerPos) > 25;
}

void Crow::landTo(Ogre::Vector3 pos, float maxOffset)
{
    Vector3 offset(Math::RangeRandom(-maxOffset, maxOffset), 0, Math::RangeRandom(-maxOffset, maxOffset));
    Vector3 lpos(pos + offset);
    bool passRay = MathUtils::getVerticalRayPos(lpos, 5);

    //try 3 positions, get last one if nothing passes
    for (size_t i = 0; i < 2; i++)
    {
        if (!passRay || !validateGroundPos(lpos))
        {
            offset = Vector3(Math::RangeRandom(-maxOffset, maxOffset), 0, Math::RangeRandom(-maxOffset, maxOffset));
            lpos = pos + offset;
            passRay = MathUtils::getVerticalRayPos(lpos, 5);
        }
        else
            break;
    }

    //start landing
    path.setLandingAnim(lpos);
}

void Crow::scheduleWalking()
{
    stateVariationTimer = Math::RangeRandom(3, 5);
}

void Crow::startWalking()
{
    bool found = false;
    Vector3 lpos;
    Vector3 spos = path.getPosition();

    //try 3 positions
    for (size_t i = 0; i < 3 && !found; i++)
    {
        float walkDistMax = 3;
        Vector3 offset(Math::RangeRandom(-walkDistMax, walkDistMax), 0, Math::RangeRandom(-walkDistMax, walkDistMax));
        lpos = (spos + offset);

        found = MathUtils::getVerticalRayPos(lpos, 5) && MathUtils::isPathFree(spos, lpos) && validateGroundPos(lpos);
    }

    if (found)
    {
        path.setWalkingAnim(lpos);
        animation.fadeTo("sideWalk", 0.05f);
        stateVariationTimer = 0.5f;
    }
    else
        scheduleWalking();
}