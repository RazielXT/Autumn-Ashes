#include "stdafx.h"
#include "ZipLine.h"
#include "Player.h"

using namespace Ogre;

void ZipLine::pressedKey(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_SPACE)
    {
        auto jumpSpeed = Global::player->getFacingDirection() * 12;
        jumpSpeed.y = std::max(jumpSpeed.y, 5.0f);

        release();
        Global::player->body->setVelocity(jumpSpeed);
    }

    if (arg.key == OIS::KC_C)
    {
        release();
    }
}

void ZipLine::movedMouse(const OIS::MouseEvent &e)
{

}

ZipLine::ZipLine(const std::vector<Ogre::Vector3>& points)
{
    tracker = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    head = tracker->createChildSceneNode();
    head->setPosition(0, -1.5f, 0);

    initZipLine(points);
}

void ZipLine::initZipLine(const std::vector<Ogre::Vector3>& points)
{
    zipLine.clear();
    zipLine.resize(points.size());

    for (size_t i = 0; i < points.size(); i++)
    {
        ZipLinePoint& point = zipLine[i];
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

    zipLine[0].startOffset=0;
    float timer = 0;
    for (size_t i = 1; i < points.size(); i++)
    {
        timer += points[i - 1].distance(points[i]) / avgSpeed;
        zipLine[i].startOffset = timer;
    }

    Animation* anim = Global::mSceneMgr->createAnimation(name, timer);
    anim->setInterpolationMode(Animation::IM_SPLINE);

    NodeAnimationTrack* track = anim->createNodeTrack(0, tracker);

    for (size_t i = 0; i < points.size(); i++)
    {
        Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(zipLine[i].startOffset);
        kf->setTranslate(points[i]);
    }
}

ZipLine::LineProjState ZipLine::getProjectedState(Ogre::Vector3& point, Ogre::Vector3& start, Ogre::Vector3& end)
{
    Vector3 dir = end - start;
    Vector3 pDir = point - start;
    dir.normalise();
    auto dp = pDir.dotProduct(dir);
    dir *= dp;

    //auto lToP = dir.length();
    // auto lleft = (end - dir).length();

    LineProjState state;
    state.projPos = dir + start;
    state.sqMinDistance = std::min(point.squaredDistance(end), std::min(point.squaredDistance(start), point.squaredDistance(state.projPos)));

    return state;
}

#define MAX_PLAYER_DISTANCE 5

bool ZipLine::placePointOnLine(Vector3& point)
{
    auto zipPos = zipLine[0];
    float minDist = MAX_PLAYER_DISTANCE*MAX_PLAYER_DISTANCE;

    for (size_t id = 1; id < zipLine.size(); id++)
    {
        auto state = getProjectedState(point, zipLine[id - 1].pos, zipLine[id].pos);

        if (state.sqMinDistance < minDist)
        {
            auto timePos = zipLine[id - 1].startOffset;
            timePos += state.projPos.distance(zipLine[id-1].pos)/avgSpeed;

            mTrackerState->setTimePosition(timePos);

            minDist = state.sqMinDistance;
        }
    }

    return (minDist != MAX_PLAYER_DISTANCE);
}

bool ZipLine::start()
{
    if (active || unavailableTimer>0)
        return false;

    //auto pos = Global::mSceneMgr->getSceneNode("Test")->getPosition();// zipLine[0].pos;//
    auto pos = Global::player->body->getPosition();
    pos.y += 1.5f;

    if (mTrackerState == nullptr)
        mTrackerState = Global::mSceneMgr->createAnimationState(name);

    if (placePointOnLine(pos))
    {
        attach();

        mTrackerState->setEnabled(true);
        mTrackerState->setLoop(true);

        //TODO figure out start speed, based on body speed
        currentSpeed = 0.5f;
        active = true;

        return true;
    }

    return false;
}

void ZipLine::updateSlidingSpeed(float time)
{
    currentSpeed = std::min(currentSpeed+time*0.4f, 2.5f);
}

void ZipLine::attach()
{
    registerInputListening();

    Ogre::Camera* cam = Global::mSceneMgr->getCamera("Camera");
    cam->detachFromParent();

    Global::player->enableControl(false);

    head->attachObject(cam);

    Global::player->body->freeze();
}

void ZipLine::release()
{
    Global::player->attachCamera();

    unregisterInputListening();

    enablePlayerControl = true;

    Global::player->body->setPositionOrientation(tracker->getPosition(), Ogre::Quaternion::IDENTITY);
    Global::player->body->unFreeze();

    unavailableTimer = 0.5f;
    active = false;
}

void ZipLine::updateSlidingCamera(float time)
{
    auto pos = mTrackerState->getTimePosition();

    for (size_t i = 1; i < zipLine.size(); i++)
    {
        auto& lp = zipLine[i-1];
        auto& p = zipLine[i];

        if (p.startOffset >= pos)
        {
            auto duration = p.startOffset - lp.startOffset;
            auto locPos = pos - lp.startOffset;
            auto w = locPos / duration;

            auto dir = (1 - w)*lp.dir + w*p.dir;
            auto q = Vector3(0, 0, -1).getRotationTo(Vector3(dir.x,0,dir.z), Vector3(0,1,0));
            auto q2 = Vector3(0, 0, -1).getRotationTo(Vector3(0, dir.y, -1));

            head->setOrientation(q);
            head->rotate(q2);

            break;
        }
    }
}

void ZipLine::updateSlidingState(float time)
{
    updateSlidingSpeed(time);

    mTrackerState->addTime(time*currentSpeed);

    updateSlidingCamera(time);

    //past/near end
    if (!loop && mTrackerState->hasEnded())
        release();
}


bool ZipLine::update(Ogre::Real tslf)
{
    if (enablePlayerControl)
    {
        Global::player->enableControl(true);
        enablePlayerControl = false;
    }

    if (unavailableTimer <= 0)
        updateSlidingState(tslf);
    else
        unavailableTimer -= tslf;

    return active || unavailableTimer>0;
}