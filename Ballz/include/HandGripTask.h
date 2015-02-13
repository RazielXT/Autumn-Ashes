#ifndef HANDEVENTR_H
#define HANDEVENTR_H

#include "stdafx.h"
#include "TransformationTaskTemplate.h"

class HandGripReaction : public TransformationTaskTemplate
{
public:

    HandGripReaction();
    ~HandGripReaction();

    void setUserData(void* data);
    bool start();
    bool update(Ogre::Real tslf);
    void reaction();
    void setIdentifier(short id);

private:

    OgreNewt::Body* body;
    Ogre::SceneNode* node;
    Ogre::AnimationState *mAnimationState;
    Ogre::Entity* mEntity;

    float reactionTime;
    bool running;
    short id;
};

#endif //TEVENTR_H