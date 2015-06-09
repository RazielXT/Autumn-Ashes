#include "stdafx.h"
#include "Crow.h"

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

    animation.fadeTo("flight1", 0);
    curAnimType = Flying;

    stateChangeTimer = 0;
	flightNoChangeTimer = 0;

    modelOr = Quaternion(Degree(-90), Vector3(0, 1, 0)) * Quaternion(Degree(90), Vector3(0, 0, 1));
    modelOffset = Vector3(0, 0.75f, 0);
}

Crow::~Crow()
{
}

void Crow::updateAnimationState()
{
    //update model anim
    if (curAnimType != Landing && path.state == Landing && path.getTempTimeLeft() < 0.75f)
    {
        animation.fadeTo("landing", 0.25f);
        curAnimType = Landing;
    }
    else if (curAnimType != Flying && path.state == Lifting && path.getTempTime() > 0.5f)
    {
        animation.fadeTo("flight1", 0.25f);
        curAnimType = Flying;
    }
    else if (curAnimType != Lifting && path.state == Lifting && path.getTempTime() < 0.5f)
    {
        animation.fadeTo("takeoff", 0.25f);
        curAnimType = Lifting;
    }
    else if (curAnimType != OnGround && path.state == OnGround)
    {
        animation.fadeTo("idle", 0.25f);
        curAnimType = OnGround;
    }

    Global::DebugPrint(std::to_string(curAnimType), true);
}

void Crow::update(Ogre::Real tslf)
{
    updateAnimationState();
    animation.update(tslf);

    //update node pos
	if (path.update(tslf / 3.0f, mNode, modelOr, modelOffset))
	{
		stateChangeTimer = path.state == Flying ? 5.0f : 3.0f;

		if (path.state == Flying)
			flightNoChangeTimer = 5;
	}       
	else
	{
		//update state timer
		stateChangeTimer -= tslf;
		flightNoChangeTimer -= tslf;
	}

}

bool Crow::readyToChangeFlyPath() const
{
	return (path.state == Flying && flightNoChangeTimer <= 0);
}

bool Crow::readyToFly() const
{
    return (path.state == OnGround && stateChangeTimer <= 0);
}

bool Crow::readyToLand() const
{
    return (path.state == Flying && stateChangeTimer <= 0);
}

void Crow::switchFlyTo(Ogre::Animation* flightAnim)
{
	//TODO try find pos where dir isnt against crow - must be in front + threshold, path dir dot cur or must not be < -0.5 ?
	float pos = Math::RangeRandom(0, flightAnim->getLength());

	path.setLiftingAnim(flightAnim, pos);
}

void Crow::flyTo(Ogre::Animation* flightAnim)
{
    //TODO try find pos where dir isnt against crow
    float pos = Math::RangeRandom(0, flightAnim->getLength());

    if (path.state == OnGround)
    {
        //start lifting
        path.setLiftingAnim(flightAnim, pos);
    }
    else
    {
        //force state
        path.setLiftingAnim(flightAnim, pos);
    }
}

void Crow::landTo(Ogre::Vector3 pos)
{
    //TODO try find offset + raycast
    Vector3 offset(Math::RangeRandom(-4, 4), 0, Math::RangeRandom(-4, 4));
    pos += offset;

    if (path.state == OnGround)
    {
        //start landing
        path.setLandingAnim(pos);
    }
    else
    {
        //force state
        path.setLandingAnim(pos);
    }
}
