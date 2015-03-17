#include "stdafx.h"
#include "Slide.h"
#include "Player.h"
#include "MathUtils.h"

using namespace Ogre;

void Slide::pressedKey(const OIS::KeyEvent &arg)
{
    if (!active)
        return;

    if (arg.key == OIS::KC_SPACE)
    {
        auto jumpSpeed = Global::player->getFacingDirection() * std::max(15.0f, realSpeed);
        jumpSpeed.y += 5.0f;//std::max(jumpSpeed.y, 5.0f);

        release();

        Global::player->body->setVelocity(jumpSpeed);
    }

    if (arg.key == OIS::KC_C)
    {
        release();
    }
}

void Slide::movedMouse(const OIS::MouseEvent &e)
{
    if (!active)
        return;

    float mod = Global::timestep / -10.0f;
    float mouseX = e.state.X.rel*mod;
    float mouseY = e.state.Y.rel*mod;

    const float maxAngle = 70;

    headState.pitch = Math::Clamp(headState.pitch + mouseX, -maxAngle, maxAngle);
    headState.yaw = Math::Clamp(headState.yaw + mouseY, -maxAngle, maxAngle);

    Quaternion qpitch = Quaternion(Degree(headState.pitch), Vector3(0,1,0));
    Quaternion qyaw = Quaternion(Degree(headState.yaw), Vector3(1, 0, 0));
    head->setOrientation(qpitch*qyaw);
}

void Slide::initSlide(const std::string& zipAnimName)
{
    Animation* anim = Global::mSceneMgr->getAnimation(zipAnimName);
    auto track = anim->getNodeTrack(0);
    float realLength = 0;

    for (size_t i = 1; i < track->getNumKeyFrames(); i++)
    {
        auto pKeyFrame = track->getNodeKeyFrame(i-1);
        auto keyFrame = track->getNodeKeyFrame(i);
        realLength += pKeyFrame->getTranslate().distance(keyFrame->getTranslate());
    }

    auto mod = realLength / anim->getLength();

    slidePoints.clear();
    slidePoints.resize(track->getNumKeyFrames());

    animName = zipAnimName + "new";
    Animation* newAnim = Global::mSceneMgr->createAnimation(animName, realLength);
    newAnim->setInterpolationMode(Animation::IM_SPLINE);

    NodeAnimationTrack* newTrack = newAnim->createNodeTrack(0, tracker);
    newTrack->setUseShortestRotationPath(true);

    Quaternion previous;

    for (size_t i = 0; i < track->getNumKeyFrames(); i++)
    {
        SlidePoint& point = slidePoints[i];
        auto keyFrame = track->getNodeKeyFrame(i);

        Ogre::TransformKeyFrame* kf = newTrack->createNodeKeyFrame(keyFrame->getTime()*mod);
        kf->setTranslate(keyFrame->getTranslate());

        point.pos = kf->getTranslate();
        point.startOffset = kf->getTime();

        //slerp hotfix
        auto rotation = keyFrame->getRotation();
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
    slidePoints.clear();
    slidePoints.resize(points.size());

    for (size_t i = 0; i < points.size(); i++)
    {
        SlidePoint& point = slidePoints[i];
        point.pos = points[i];

        //first 2
        if (i == 0 || i == 1)
            point.dir = (points[1] - points[0]);
        //last 2
        else if (i >= points.size() - 2)
            point.dir = (points[points.size() - 1] - points[points.size() - 2]);
        //else inside
        else
        {
            auto dirB = (points[i] - points[i - 1]);
            dirB.normalise();
            auto dirF = (points[i + 1] - points[i]);
            dirF.normalise();

            point.dir = (dirB + dirF);
        }

        point.dir.normalise();
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

    NodeAnimationTrack* track = anim->createNodeTrack(0, tracker);

    track->setUseShortestRotationPath(true);

    Quaternion previous;
    int loopEnd = loop ? 1 : 0;

    for (size_t i = 0; i < points.size(); i++)
    {
        Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(slidePoints[i].startOffset);
        kf->setTranslate(points[i]);

        auto dir = slidePoints[i].dir;
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

bool Slide::start()
{
    auto pos = Global::player->bodyPosition;
    pos.y += 1.5f;

    return start(pos);
}

bool Slide::start(Vector3& pos)
{
    if (active || unavailableTimer>0)
        return false;

    if (mTrackerState == nullptr)
        mTrackerState = Global::mSceneMgr->createAnimationState(animName);

    if (placePointOnLine(pos))
    {
        currentSpeed = Global::player->bodyVelocity/avgSpeed;

        attach();

        mTrackerState->setEnabled(true);
        mTrackerState->setLoop(loop);
        active = true;

        return true;
    }

    return false;
}

void Slide::updateSlidingSpeed(float time)
{
    //auto verticalDir = tracker->getOrientation().getPitch().valueRadians();
    //auto dir = tracker->getOrientation()*Vector3(0, 0, -1);
    //currentSpeed = Math::Clamp(currentSpeed + -dir.y*0.5f*time, 1.0f, 2.5f);

    auto diff = time*1.0f;
    currentSpeed = std::min(1.0f, currentSpeed + diff);
}

void Slide::attach()
{
    resetHead();

    Ogre::Camera* cam = Global::mSceneMgr->getCamera("Camera");

    headArrival.timer = 1.0f;
    headArrival.pos = cam->getDerivedPosition();
    headArrival.dir = cam->getDerivedOrientation();
    headArrival.pitch = Math::Clamp(Global::player->bodyVelocity/10,-1.0f,1.0f);

    headState.pitch = 0;
    headState.yaw = 0;

    registerInputListening();

    Global::player->enableControl(false);

    headArrival.tempNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headArrival.tempNode->setPosition(headArrival.pos);
    headArrival.tempNode->setOrientation(headArrival.dir);

    cam->detachFromParent();
    headArrival.tempNode->attachObject(cam);

    Global::player->body->setPositionOrientation(Vector3(0,1000,0), Quaternion::IDENTITY);
    Global::player->body->freeze();

    unavailableTimer = 1;
}

void Slide::release()
{
    Global::player->attachCameraWithTransition();
    Global::player->body->setPositionOrientation(tracker->getPosition(), Ogre::Quaternion::IDENTITY);
    Global::player->body->unFreeze();
    Global::player->body->setVelocity(tracker->getOrientation()*Vector3(0, 0, -1 * realSpeed));

    unregisterInputListening();
    mTrackerState->setEnabled(false);

    unavailableTimer = 0.5f;
    active = false;
    enablePlayerControl = true;
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
    }
    else
    {
        auto w = headArrival.timer;
        Quaternion q = Quaternion::Slerp(1-w, headArrival.dir, head->_getDerivedOrientation(), true);
        Vector3 p = w*headArrival.pos + (1 - w)*head->_getDerivedPosition();

        auto pitchW = 1-pow(1-std::min(headArrival.timer, 1 - headArrival.timer),1.5f);
        auto mPitch = -headArrival.pitch * pitchW * 50;
        Quaternion pq(Degree(mPitch), Vector3(1, 0, 0));

        headArrival.tempNode->setPosition(p);
        headArrival.tempNode->setOrientation(q*pq);
    }

}

void Slide::updateSlidingCamera(float time)
{
    if (headArrival.timer > 0)
        updateHeadArrival(time);
}

Vector3 Slide::updateTargetSlide()
{
    float rayDist = 35;
    auto pos = head->_getDerivedPosition();
    auto dir = Global::player->getFacingDirection();
    auto target = pos + dir*rayDist;
    auto ray = OgreNewt::BasicRaycast(Global::mWorld, pos, target, false);
    auto info = ray.getFirstHit();

    if (info.mBody && (info.mBody->getType() == TopSlidePart || info.mBody->getType() == ZipLinePart))
    {
        auto a = any_cast<bodyUserData*>(info.mBody->getUserData());

        auto tSlide = (Slide*)a->customData;

        if (tSlide != this)
        {
            targetSlide = tSlide;
            Global::gameMgr->myMenu->showUseGui(Ui_Target);
            return pos + dir*(info.mDistance*rayDist);
        }
    }

    targetSlide = nullptr;
    return Vector3::ZERO;
}

void Slide::updateSlidingState(float time)
{
    auto thisPos = tracker->getPosition();
    realSpeed = lastPos.distance(thisPos) / time;

    updateSlidingSpeed(time);

    mTrackerState->addTime(time*currentSpeed*avgSpeed);

    updateSlidingCamera(time);
    updateTargetSlide();

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

    if (active)
        updateSlidingState(tslf);

    if (unavailableTimer>0)
        unavailableTimer -= tslf;

    return active || unavailableTimer>0;
}