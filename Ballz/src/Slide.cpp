#include "stdafx.h"
#include "Slide.h"
#include "Player.h"
#include "PlayerSliding.h"
#include "MUtils.h"
#include "SlidesAutoTarget.h"

using namespace Ogre;

Slide::~Slide()
{
}

void Slide::releasedKey(const OIS::KeyEvent &arg)
{
    if (!sliding)
        return;

    if (arg.key == OIS::KC_LSHIFT)
    {
        sprint = false;
    }
}

void Slide::pressedKey(const OIS::KeyEvent &arg)
{
    if (!sliding)
        return;

    if (arg.key == OIS::KC_SPACE && unavailableTimer<0)
    {
        release(true, true);
    }

    if (arg.key == OIS::KC_C)
    {
        release();
    }

    if (arg.key == OIS::KC_LSHIFT)
    {
        sprint = true;
    }
}

void Slide::movedMouse(const OIS::MouseEvent &e)
{
    if (!sliding || sprint)
        return;

    float mod = Global::timestep / -10.0f;
    float mouseX = e.state.X.rel*mod;
    float mouseY = e.state.Y.rel*mod;

    const float maxAngle = 85;

    headState.pitch = headState.pitch + mouseX; // Math::Clamp(headState.pitch + mouseX, -maxAngle, maxAngle);
    headState.yaw = Math::Clamp(headState.yaw + mouseY, -maxAngle, maxAngle);
}

void Slide::instantDetach(bool returnControl)
{
    release(returnControl);
}

void Slide::startJumpToSlide()
{
    auto target = getCurrentState().getTranslate();
    target.y += head->getPosition().y;

    Ogre::Camera* cam = Global::mSceneMgr->getCamera("Camera");

    auto pos = cam->getDerivedPosition();
    auto or = cam->getDerivedOrientation();

    cam->detachFromParent();
    headArrival.tempNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headArrival.tempNode->attachObject(cam);
    headArrival.tempNode->setPosition(pos);
    headArrival.tempNode->setOrientation(or);

    headArrival.pos = pos;
    headArrival.posTarget = target;
    headArrival.timer = 0;
    headArrival.dist = std::max(0.5f, pos.distance(target) / 10.0f);
    headArrival.dir = or;

    headArrival.dirTarget.FromAngleAxis(or.getYaw(), Vector3(0,1,0));
    headArrival.dirTarget = headArrival.dirTarget*Quaternion(Ogre::Degree(-30), Vector3(1, 0, 0));

    jumpingToSlide = true;

    float shakeW = std::min(1.0f, headArrival.dist*0.8f);
    Global::shaker->startShaking(shakeW*0.8f, shakeW*1.0f, 0.5f, 1, 1, 0.4f, 0.25f, 1, true);
}

//w^n, n = -1-1 to 3-1/3, n 0->1
float transformHeightFuncTime(float time, float hd)
{
    auto t = abs(hd);

    t = t * 2 + 1;

    if (hd > 0)
        t = 1 / t;

    return pow(time, t);
}

float heightFunc(float time, float hd)
{
    auto x = transformHeightFuncTime(time, hd);

    float off = 1;
    float ex = 2; //2,4,6
    float addH = -pow(x*off * 2 - off, ex) + pow(off, ex);
    addH /= off * 2;

    addH *= 1 - abs(hd);

    return addH;
}

void Slide::updateJumpToSlide(float time)
{
    float w = 0;
    bool beforeJump = false;

    if (headArrival.timer < 0)
    {
        auto t = headArrival.timer + time*2.0f;

        if (t < 0)
        {
            headArrival.timer = t;
            beforeJump = true;
        }
    }

    if (!beforeJump)
    {
        headArrival.timer = std::min(headArrival.timer + time*2.75f, headArrival.dist);
        w = headArrival.timer / headArrival.dist;

        //w = pow(w, 0.58f);
        //w = quickstep(w, 0.75f);
    }

    auto hDiff = headArrival.pos.y - headArrival.posTarget.y;
    auto tDist = headArrival.pos.distance(headArrival.posTarget);
    auto hd = hDiff / tDist;

    auto pos = MUtils::lerp(headArrival.pos, headArrival.posTarget, w);
    auto dir = Quaternion::nlerp(w, headArrival.dir, headArrival.dirTarget, true);

    auto maxH = tDist;// / 2.0f;
    auto hAdd = heightFunc(w, hd)*maxH;
    pos.y += hAdd;

    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Jumping: hadd " + Ogre::StringConverter::toString(hAdd) + ", hd " + Ogre::StringConverter::toString(hd), Ogre::LML_NORMAL);

    headArrival.tempNode->setPosition(pos);
    headArrival.tempNode->setOrientation(dir*Global::shaker->current);

    if (headArrival.timer==headArrival.dist)
    {
        jumpingToSlide = false;

        attach(true,0.2f);
    }
}

bool Slide::start(bool withJump)
{
    auto pos = Global::player->bodyPosition;
    pos.y += 1.5f;

    return start(pos, withJump);
}

bool Slide::start(Vector3& pos, bool withJump)
{
    if (sliding)
        return false;

    if (mTrackerState == nullptr)
        mTrackerState = Global::mSceneMgr->createAnimationState(animName);

    if (placePointOnLine(pos))
    {
        setCorrectDirection(bidirectional);
        removeControlFromPlayer();

        auto pdir = Global::player->getFacingDirection();
        auto slDir = getDirectionState()*Vector3(0, 0, -1);
        currentSpeed = Global::player->bodyVelocityL * std::max(0.0f, pdir.dotProduct(slDir));//  Global::player->bodyVelocity / avgSpeed;

        if (withJump)
            startJumpToSlide();
        else
            attach(true);

        Global::player->pSliding->slideStarted(this);

        return true;
    }

    return false;
}

bool Slide::start(float startOffset, bool withJump, float headArrivalTimer)
{
    if (sliding)
        return false;

    setCorrectDirection(bidirectional, startOffset);

    currentSpeed = 0;

    removeControlFromPlayer();

    if (withJump)
        startJumpToSlide();
    else
        attach(true, headArrivalTimer);

    Global::player->pSliding->slideStarted(this);

    return true;
}

void Slide::updateSlidingSpeed(float time)
{
    //auto verticalDir = tracker->getOrientation().getPitch().valueRadians();
    //auto dir = tracker->getOrientation()*Vector3(0, 0, -1);
    //currentSpeed = Math::Clamp(currentSpeed + -dir.y*0.5f*time, 1.0f, 2.5f);

    auto dir = tracker->getOrientation()*Vector3(0, 0, -1);
    float g = dir.y < 0 ? (1 + dir.y) : 0.5f * (1 - dir.y);

    if (sprint)
        g += 1.0f;

    g *= avgSpeed;

    float nextSpeed = currentSpeed + time*g;

    currentSpeed = nextSpeed;

    if (walkable)
        currentSpeed = 0;
}

void Slide::removeControlFromPlayer()
{
    Global::player->enableControl(false);
}

void Slide::attach(bool retainDirection, float headArrivalTime)
{
    resetHead();

    {
        Ogre::Camera* cam = Global::mSceneMgr->getCamera("Camera");

        auto state = getCurrentState();
        auto pDir = cam->getDerivedOrientation();
        auto slDir = getDirectionState();

        if (retainDirection)
        {
            headState.pitch = MUtils::getYawBetween(pDir, slDir);
            headState.yaw = MUtils::getPitchBetween(pDir, slDir);
        }
        else
        {
            headState.pitch = headState.yaw = 0;
        }

        headArrival.timer = headArrivalTime;
        headArrival.posTarget = state.getTranslate() + head->getPosition();
        headArrival.pos = cam->getDerivedPosition();
        headArrival.dir = pDir;

        if (headArrival.tempNode == nullptr)
        {
            headArrival.tempNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
        }

        cam->detachFromParent();
        headArrival.tempNode->attachObject(cam);

        headArrival.tempNode->setPosition(headArrival.pos);
        headArrival.tempNode->setOrientation(headArrival.dir);
    }

    Global::player->body->setPositionOrientation(Vector3(0,1000,0), Quaternion::IDENTITY);
    Global::player->body->freeze();

    unavailableTimer = 0.25f;

    Global::shaker->startShaking(0.85, 1.0, 0.25, 1, 1, 0.5, 0.35, 1, true);

    mTrackerState->setEnabled(true);
    mTrackerState->setLoop(loop);
    sliding = true;
    sprint = false;
}

void Slide::release(bool returnControl, bool manualJump)
{
    if (returnControl)
    {
        Ogre::Quaternion direction = !manualJump ? getDirectionState() : head->_getDerivedOrientation();

        Global::player->attachCameraWithTransition(0.2f, direction);
        Global::player->body->setPositionOrientation(head->_getDerivedPosition(), Ogre::Quaternion::IDENTITY);
        Global::player->body->unFreeze();

        float releaseSpeed = pow(std::max(1.0f, currentSpeed * 0.5f), 0.75f);

        if (!manualJump)
            Global::player->body->setVelocity(direction*Vector3(0, 0, -1 * releaseSpeed) + Vector3(0, 3, 0));
        else
            Global::player->body->setVelocity(10 * Global::player->getFacingDirection() + Vector3(0, 5, 0));

        Global::shaker->startShaking(0.85, 1.0, 0.25, 1, 1, 0.5, 0.35, 1, true);

        enablePlayerControl = true;
    }

    mTrackerState->setEnabled(false);

    sliding = false;
}

void Slide::updateHeadArrival(float time)
{
    headArrival.timer -= time*std::max(1.0f,currentSpeed);

    if (headArrival.timer <= 0)
    {
        Ogre::Camera* cam = Global::mSceneMgr->getCamera("Camera");
        cam->detachFromParent();
        head->attachObject(cam);

        Quaternion qpitch = Quaternion(Degree(headState.pitch), Vector3(0, 1, 0));
        Quaternion qyaw = Quaternion(Degree(headState.yaw), Vector3(1, 0, 0));
        head->setOrientation(qpitch*qyaw*Global::shaker->current);

        Global::mSceneMgr->destroySceneNode(headArrival.tempNode);
        headArrival.tempNode = nullptr;
    }
    else
    {
        auto w = headArrival.timer;

        Quaternion qpitch = Quaternion(Degree(headState.pitch), Vector3(0, 1, 0));
        Quaternion qyaw = Quaternion(Degree(headState.yaw), Vector3(1, 0, 0));
        Quaternion qCur = head->_getDerivedOrientation()*qpitch*qyaw*Global::shaker->current;

        Quaternion q = Quaternion::nlerp(1-w, headArrival.dir, qCur, true);
        headArrival.tempNode->setOrientation(q);

        Vector3 moveOffset = head->_getDerivedPosition() - headArrival.posTarget;

        Vector3 p = w*headArrival.pos + (1 - w)*headArrival.posTarget;
        headArrival.tempNode->setPosition(p + moveOffset);

    }

}

void Slide::updateSlidingCamera(float time)
{
    if (headArrival.timer > 0)
        updateHeadArrival(time);
    else
    {
        float headDif = abs(headState.pitch) + abs(headState.yaw);

        if (sprint && headDif>0)
        {
            float pW = time * 250 * headState.pitch / headDif;
            if (headState.pitch>0)
                headState.pitch = std::max(0.0f, headState.pitch - pW);
            else
                headState.pitch = std::min(0.0f, headState.pitch - pW);

            float yW = time * 90 * headState.yaw / headDif;
            if (headState.yaw > 0)
                headState.yaw = std::max(0.0f, headState.yaw - yW);
            else
                headState.yaw = std::min(0.0f, headState.yaw - yW);
        }

        Quaternion qpitch = Quaternion(Degree(headState.pitch), Vector3(0, 1, 0));
        Quaternion qyaw = Quaternion(Degree(headState.yaw), Vector3(1, 0, 0));

        head->setOrientation(qpitch*qyaw*Global::shaker->current);
    }
}

void Slide::updateSlidingState(float time)
{
    auto thisPos = tracker->getPosition();
    realSpeed = lastPos.distance(thisPos) / time;

    //auto log = Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log");
    //log->logMessage("LINE SPEED " + std::to_string(realSpeed), Ogre::LML_NORMAL);

    updateSlidingSpeed(time);

    updateTrack(time*currentSpeed);

    updateSlidingCamera(time);

    //past/near end
    if (!loop && mTrackerState->hasEnded())
    {
        release();
    }

    lastPos = tracker->getPosition();
}


bool Slide::update(Ogre::Real tslf)
{
    tslf *= Global::timestep;

    if (sliding)
        updateSlidingState(tslf);
    else if (jumpingToSlide)
        updateJumpToSlide(tslf);

    if (unavailableTimer>0)
        unavailableTimer -= tslf;

    if (enablePlayerControl)
    {
        Global::player->enableControl(true);
        enablePlayerControl = false;
    }

    return jumpingToSlide || sliding;
}