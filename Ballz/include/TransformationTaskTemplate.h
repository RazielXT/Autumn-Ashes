#ifndef MOVETEMPLEVENTR_H
#define MOVETEMPLEVENTR_H

#include "stdafx.h"
#include "EventTask.h"

class TransformationTaskTemplate : public EventTask
{

public:

    TransformationTaskTemplate() {};
    ~TransformationTaskTemplate() {};

    virtual void setUserData(void* data) = 0;
    virtual bool start() = 0;
    virtual bool update(Ogre::Real tslf) = 0;

protected:

    void setTransformationParams(Ogre::Quaternion startOr,Ogre::Quaternion endOr,Ogre::Vector3 startPos,Ogre::Vector3 endPos,Ogre::Vector3 startScale = Ogre::Vector3::ZERO,Ogre::Vector3 endScale = Ogre::Vector3::ZERO);
    void updateTransformation();

    float timer;
    float targetTime;

    Ogre::Quaternion currentOr;
    Ogre::Vector3 currentPos;
    Ogre::Vector3 currentScale;

private:

    Ogre::Quaternion startOr;
    Ogre::Quaternion endOr;
    Ogre::Vector3 moveVector;
    Ogre::Vector3 startPos;
    Ogre::Vector3 startScale;
    Ogre::Vector3 scaleVector;

};

#endif