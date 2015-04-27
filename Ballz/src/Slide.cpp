#include "stdafx.h"
#include "Slide.h"
#include "Player.h"
#include "MathUtils.h"
#include "SlidesAutoTarget.h"

using namespace Ogre;

Slide::~Slide()
{
}

void Slide::pressedKey(const OIS::KeyEvent &arg)
{
    if (!sliding)
        return;

    if (arg.key == OIS::KC_SPACE && unavailableTimer<0)
    {

        if (slidesAutoTarget->targetInfo.targetSlide)
        {
            if (slidesAutoTarget->targetInfo.targetSlide->start(slidesAutoTarget->targetInfo.targetSlidePosOffset, true))
                release(false);
        }
        else
        {
            auto jumpSpeed = Global::player->getFacingDirection() * std::max(13.0f, realSpeed);
            jumpSpeed.y += 5.0f;//std::max(jumpSpeed.y, 5.0f);

            release();

            Global::player->body->setVelocity(jumpSpeed);
        }

    }

    if (arg.key == OIS::KC_C)
    {
        release();
    }
}

Vector3 Slide::getTrackPosition(float timeOffset)
{
    Ogre::TransformKeyFrame key(0, 0);
    track->getInterpolatedKeyFrame(timeOffset, &key);

    return key.getTranslate();
}

void Slide::movedMouse(const OIS::MouseEvent &e)
{
    if (!sliding)
        return;

    float mod = Global::timestep / -10.0f;
    float mouseX = e.state.X.rel*mod;
    float mouseY = e.state.Y.rel*mod;

    const float maxAngle = 70;

    headState.pitch = Math::Clamp(headState.pitch + mouseX, -maxAngle, maxAngle);
    headState.yaw = Math::Clamp(headState.yaw + mouseY, -maxAngle, maxAngle);
}

void Slide::initSlide(const std::string& zipAnimName)
{
    headArrival.tempNode = nullptr;
    Animation* anim = Global::mSceneMgr->getAnimation(zipAnimName);
    auto o_track = anim->getNodeTrack(0);
    float realLength = 0;

    for (size_t i = 1; i < o_track->getNumKeyFrames(); i++)
    {
        auto pKeyFrame = o_track->getNodeKeyFrame(i - 1);
        auto keyFrame = o_track->getNodeKeyFrame(i);
        realLength += pKeyFrame->getTranslate().distance(keyFrame->getTranslate());
    }

    auto mod = realLength / anim->getLength();

    slidePoints.clear();
    slidePoints.resize(o_track->getNumKeyFrames());

    animName = zipAnimName + "new";
    Animation* newAnim = Global::mSceneMgr->createAnimation(animName, realLength);
    newAnim->setInterpolationMode(Animation::IM_SPLINE);

    track = newAnim->createNodeTrack(0, tracker);
    track->setUseShortestRotationPath(true);

    Quaternion previous;
    Quaternion yawMe(Degree(-90), Vector3(0, 1, 0));

    for (size_t i = 0; i < o_track->getNumKeyFrames(); i++)
    {
        SlidePoint& point = slidePoints[i];
        auto keyFrame = o_track->getNodeKeyFrame(i);

        Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(keyFrame->getTime()*mod);
        kf->setTranslate(keyFrame->getTranslate());

        point.pos = kf->getTranslate();
        point.startOffset = kf->getTime();

        //slerp hotfix
        auto rotation = keyFrame->getRotation();
        rotation = rotation*yawMe;

        if (i > 0)
        {
            float fCos = previous.Dot(rotation);
            if (fCos < 0.0f)
                rotation = -rotation;

            keyFrame->setRotation(rotation);
        }

        kf->setRotation(keyFrame->getRotation());

        previous = rotation;
    }

}

void Slide::initSlide(const std::vector<Ogre::Vector3>& points)
{
    headArrival.tempNode = nullptr;

    slidePoints.clear();
    slidePoints.resize(points.size());

    std::vector<Vector3> pointsDir;

    for (size_t i = 0; i < points.size(); i++)
    {
        SlidePoint& point = slidePoints[i];
        point.pos = points[i];

        Vector3 dir;

        //first 2
        if (i == 0 || i == 1)
            dir = (points[1] - points[0]);
        //last 2
        else if (i >= points.size() - 2)
            dir = (points[points.size() - 1] - points[points.size() - 2]);
        //else inside
        else
        {
            auto dirB = (points[i] - points[i - 1]);
            dirB.normalise();
            auto dirF = (points[i + 1] - points[i]);
            dirF.normalise();

            dir = (dirB + dirF);
        }

        dir.normalise();

        pointsDir.push_back(dir);
    }

    slidePoints[0].startOffset=0;
    float timer = 0;
    for (size_t i = 1; i < points.size(); i++)
    {
        timer += points[i - 1].distance(points[i]);
        slidePoints[i].startOffset = timer;
    }

    Animation* anim = Global::mSceneMgr->createAnimation(animName, timer);
    anim->setInterpolationMode(Animation::IM_SPLINE);

    track = anim->createNodeTrack(0, tracker);

    track->setUseShortestRotationPath(true);

    Quaternion previous;
    int loopEnd = loop ? 1 : 0;

    for (size_t i = 0; i < points.size(); i++)
    {
        Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(slidePoints[i].startOffset);
        kf->setTranslate(points[i]);

        auto& dir = pointsDir[i];
        auto q = Vector3(0, 0, -1).getRotationTo(Vector3(dir.x, 0, dir.z));
        auto q2 = Vector3(0, 0, -1).getRotationTo(Vector3(0, dir.y, -1));

        auto rotation = q*q2;

        //slerp hotfix by http://ogre3d.org/forums/viewtopic.php?f=2&t=47636
        if (i > 0)
        {
            float fCos = previous.Dot(rotation);
            if (fCos < 0.0f)
                rotation = -rotation;
        }

        kf->setRotation(rotation);

        previous = rotation;
    }
}

#define MAX_PLAYER_DISTANCE_SQ 4*4

bool Slide::placePointOnLine(Vector3& point)
{
    //auto log = Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log");

    auto zipPos = slidePoints[0];
    float minDist = MAX_PLAYER_DISTANCE_SQ;

    //log->logMessage("START------TRY TO PLACE POINT ON LINE", Ogre::LML_NORMAL);

    for (size_t id = 1; id < slidePoints.size(); id++)
    {
        auto state = MathUtils::getProjectedState(point, slidePoints[id - 1].pos, slidePoints[id].pos);

        //log->logMessage(std::to_string(id) + ". state: " + Ogre::StringConverter::toString(state.projPos) + " distance " + Ogre::StringConverter::toString(state.sqMinDistance), Ogre::LML_NORMAL);
        //log->logMessage(std::to_string(id) + ". line: " + Ogre::StringConverter::toString(slidePoints[id - 1].pos) + " to " + Ogre::StringConverter::toString(slidePoints[id].pos), Ogre::LML_NORMAL);

        if (state.sqMinDistance < minDist)
        {
            //log->logMessage("ACCEPTED");

            auto timePos = slidePoints[id - 1].startOffset;
            timePos += state.projPos.distance(slidePoints[id-1].pos)/avgSpeed;

            mTrackerState->setTimePosition(timePos);

            minDist = state.sqMinDistance;
        }
    }

    //log->logMessage("END------TRY TO PLACE POINT ON LINE", Ogre::LML_NORMAL);

    return (minDist != MAX_PLAYER_DISTANCE_SQ);
}

inline void fixSpline(Quaternion& rotation, Quaternion previous)
{
    float fCos = previous.Dot(rotation);
    if (fCos < 0.0f)
        rotation = -rotation;
}

void Slide::startJumpToSlide(bool fromGround)
{
    Global::shaker->startShaking(1.0, 1.0, 0.5, 1, 1, 0.5, 0.35, 1, true);

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
    headArrival.timer = fromGround ? -0.5f : 0;
    headArrival.pitch = std::max(0.5f, pos.distance(target) / 10.0f);
    headArrival.dir = or;

    headArrival.dirTarget.FromAngleAxis(or.getYaw(), Vector3(0,1,0));
    headArrival.dirTarget = headArrival.dirTarget*Quaternion(Ogre::Degree(-30), Vector3(1, 0, 0));

    jumpingToSlide = true;
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
        headArrival.timer = std::min(headArrival.timer + time*1.75f, headArrival.pitch);
        w = headArrival.timer / headArrival.pitch;
        w = pow(w, 0.8f);
    }

    auto hDiff = headArrival.pos.y - headArrival.posTarget.y;
    auto tDist = headArrival.pos.distance(headArrival.posTarget);
    auto hd = hDiff / tDist;

    auto pos = MathUtils::lerp(headArrival.pos, headArrival.posTarget, w);
    auto dir = Quaternion::nlerp(w, headArrival.dir, headArrival.dirTarget, true);

    auto maxH = tDist;// / 2.0f;
    auto hAdd = heightFunc(w, hd)*maxH;
    pos.y += hAdd;

    Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Jumping: hadd " + Ogre::StringConverter::toString(hAdd) + ", hd " + Ogre::StringConverter::toString(hd), Ogre::LML_NORMAL);

    headArrival.tempNode->setPosition(pos);
    headArrival.tempNode->setOrientation(dir*Global::shaker->current);

    if (headArrival.timer==headArrival.pitch)
    {
        jumpingToSlide = false;

        attach();
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
    if (sliding || unavailableTimer>0)
        return false;

    if (mTrackerState == nullptr)
        mTrackerState = Global::mSceneMgr->createAnimationState(animName);

    if (placePointOnLine(pos))
    {
        currentSpeed = 1;// Global::player->bodyVelocity / avgSpeed;

        bool onGround = Global::player->isInControl();
        removeControlFromPlayer();

        if (withJump)
            startJumpToSlide(onGround);
        else
            attach();

        Global::mEventsMgr->addCachedTask(this);

        return true;
    }

    return false;
}

bool Slide::start(float startOffset, bool withJump)
{
    if (sliding || unavailableTimer > 0)
        return false;

    if (mTrackerState == nullptr)
        mTrackerState = Global::mSceneMgr->createAnimationState(animName);

    if (mTrackerState->getLength() < startOffset)
        return false;

    mTrackerState->setTimePosition(startOffset);

    currentSpeed = 1;

    bool onGround = Global::player->isInControl();
    removeControlFromPlayer();

    if (withJump)
        startJumpToSlide(onGround);
    else
        attach();

    Global::mEventsMgr->addCachedTask(this);

    return true;
}

void Slide::updateSlidingSpeed(float time)
{
    //auto verticalDir = tracker->getOrientation().getPitch().valueRadians();
    //auto dir = tracker->getOrientation()*Vector3(0, 0, -1);
    //currentSpeed = Math::Clamp(currentSpeed + -dir.y*0.5f*time, 1.0f, 2.5f);

    auto diff = time*1.0f;
    currentSpeed = std::min(1.0f, currentSpeed + diff);
}

void Slide::removeControlFromPlayer()
{
    Global::player->enableControl(false);
    slidesAutoTarget = Global::player->slidesAutoTarget;
}

Ogre::TransformKeyFrame Slide::getCurrentState()
{
    Ogre::TransformKeyFrame key(0, 0);
    track->getInterpolatedKeyFrame(mTrackerState->getTimePosition(), &key);

    return key;
}

void Slide::attach()
{
    resetHead();

    registerInputListening();

    Ogre::Camera* cam = Global::mSceneMgr->getCamera("Camera");

    auto state = getCurrentState();

    headArrival.timer = 1.0f;
    headArrival.posTarget = state.getTranslate();
    headArrival.pos = cam->getDerivedPosition();
    headArrival.dir = cam->getDerivedOrientation();
    headArrival.pitch = 0;// Math::Clamp(Global::player->bodyVelocity / 10, -1.0f, 1.0f);

    headState.pitch = 0;
    headState.yaw = 0;

    /*
    auto e = Global::mSceneMgr->createEntity("Teapot01.mesh");
    auto sn = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0, 20, 0));
    sn->attachObject(e);

    e = Global::mSceneMgr->createEntity("Teapot01.mesh");
    sn = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(10, 20, 0), headArrival.dir);
    sn->attachObject(e);

    e = Global::mSceneMgr->createEntity("Teapot01.mesh");
    sn = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(20, 20, 0), state.getRotation());
    sn->attachObject(e);

    e = Global::mSceneMgr->createEntity("Teapot01.mesh");
    sn = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(30, 20, 0), cam->getDerivedOrientation());
    sn->attachObject(e);
    */

    if (headArrival.tempNode == nullptr)
    {
        cam->detachFromParent();
        headArrival.tempNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
        headArrival.tempNode->attachObject(cam);
    }

    headArrival.tempNode->setPosition(headArrival.pos);
    headArrival.tempNode->setOrientation(headArrival.dir);

    Global::player->body->setPositionOrientation(Vector3(0,1000,0), Quaternion::IDENTITY);
    Global::player->body->freeze();

    unavailableTimer = 1;

    mTrackerState->setEnabled(true);
    mTrackerState->setLoop(loop);
    sliding = true;
}

void Slide::release(bool returnControl)
{
    if (returnControl)
    {
        Global::player->attachCameraWithTransition();
        Global::player->body->setPositionOrientation(tracker->getPosition(), Ogre::Quaternion::IDENTITY);
        Global::player->body->unFreeze();
        Global::player->body->setVelocity(tracker->getOrientation()*Vector3(0, 0, -1 * realSpeed) + Vector3(0,3,0));

        enablePlayerControl = true;
    }

    unregisterInputListening();
    mTrackerState->setEnabled(false);

    unavailableTimer = 1.5f;
    sliding = false;

}

void Slide::updateHeadArrival(float time)
{
    headArrival.timer -= time*2*currentSpeed;

    if (headArrival.timer <= 0)
    {
        Ogre::Camera* cam = Global::mSceneMgr->getCamera("Camera");
        cam->detachFromParent();
        head->attachObject(cam);

        Global::mSceneMgr->destroySceneNode(headArrival.tempNode);
        headArrival.tempNode = nullptr;
    }
    else
    {
        auto w = headArrival.timer;
        Quaternion q = Quaternion::nlerp(1-w, headArrival.dir, head->_getDerivedOrientation(), true);

        Vector3 moveOffset = head->_getDerivedPosition() - headArrival.posTarget;
        Vector3 p = w*headArrival.pos + (1 - w)*headArrival.posTarget;

        auto pitchW = 1-pow(1-std::min(headArrival.timer, 1 - headArrival.timer),1.5f);
        auto mPitch = -headArrival.pitch * pitchW * 50;
        Quaternion pq(Degree(mPitch), Vector3(1, 0, 0));
        pq = pq*Global::shaker->current;

        headArrival.tempNode->setPosition(p + moveOffset);
        headArrival.tempNode->setOrientation(q*pq);
    }

}

void Slide::updateSlidingCamera(float time)
{
    if (headArrival.timer > 0)
        updateHeadArrival(time);
    else
    {
        Quaternion qpitch = Quaternion(Degree(headState.pitch), Vector3(0, 1, 0));
        Quaternion qyaw = Quaternion(Degree(headState.yaw), Vector3(1, 0, 0));

        head->setOrientation(qpitch*qyaw*Global::shaker->current);
    }
}

void Slide::updateTargetSlide(float time)
{
    slidesAutoTarget->updateAutoTarget(head->_getDerivedPosition(), Global::player->getFacingDirection(), time, 30, this);
}

void Slide::updateSlidingState(float time)
{
    auto thisPos = tracker->getPosition();
    realSpeed = lastPos.distance(thisPos) / time;

    auto log = Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log");
    log->logMessage("LINE SPEED " + std::to_string(realSpeed), Ogre::LML_NORMAL);

    updateSlidingSpeed(time);

    mTrackerState->addTime(time*currentSpeed*avgSpeed);

    updateSlidingCamera(time);

    updateTargetSlide(time);

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

    if (enablePlayerControl)
    {
        Global::player->enableControl(true);
        enablePlayerControl = false;
    }

    if (sliding)
        updateSlidingState(tslf);
    else if (jumpingToSlide)
        updateJumpToSlide(tslf);

    if (unavailableTimer>0)
        unavailableTimer -= tslf;

    return jumpingToSlide || sliding || unavailableTimer>0;
}