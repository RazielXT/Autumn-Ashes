#include "stdafx.h"
#include "TrackBuilder.h"

using namespace Ogre;

int TrackBuilder::counter = 0;

void TrackBuilder::init(float length)
{
    Animation* anim = Global::mSceneMgr->createAnimation("gen" + std::to_string(counter++), length);
    anim->setInterpolationMode(Animation::IM_SPLINE);
    anim->setRotationInterpolationMode(Animation::RIM_SPHERICAL);

    track = anim->createNodeTrack(0);
    track->setUseShortestRotationPath(true);

    lastOr = Quaternion::IDENTITY;
}

void TrackBuilder::fixSpline(Quaternion& rotation)
{
    float fCos = lastOr.Dot(rotation);
    if (fCos < 0.0f)
        rotation = -rotation;
}

void TrackBuilder::addKey(float t, Ogre::Vector3 pos, Ogre::Quaternion or)
{
    Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(t);

    if (t != 0)
        fixSpline(or);

    lastOr = or;

    kf->setTranslate(pos);
    kf->setRotation(or);
}