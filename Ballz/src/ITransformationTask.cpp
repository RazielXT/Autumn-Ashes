#include "stdafx.h"
#include "ITransformationTask.h"

void ITransformationTask::setTransformationParams(Ogre::Quaternion startOr,Ogre::Quaternion endOr,Ogre::Vector3 startPos,Ogre::Vector3 endPos,Ogre::Vector3 startScale,Ogre::Vector3 endScale)
{
    this->startOr = this->currentOr = startOr;
    this->endOr = endOr;
    this->startPos = this->currentPos = startPos;
    this->moveVector = endPos - startPos;
    this->startScale = this->currentScale = startScale;
    this->scaleVector = endScale - startScale;
}

void ITransformationTask::updateTransformation()
{
    float interpolationFactor = timer/targetTime;

    currentPos = startPos + moveVector*interpolationFactor;
    currentScale = startScale + scaleVector*interpolationFactor;
    currentOr = Ogre::Quaternion::nlerp(interpolationFactor,startOr,endOr);
}