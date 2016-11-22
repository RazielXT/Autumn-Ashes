#include "stdafx.h"
#include "HandGripTask.h"
#include "EventsManager.h"
#include "Player.h"

HandGripReaction::HandGripReaction()
{
	running=false;
	id = 0;
}

HandGripReaction::~HandGripReaction()
{
}

void HandGripReaction::setIdentifier(short id)
{
	this->id = id;
}

//get stuff
void HandGripReaction::setUserData(void* data)
{
	mEntity = (Ogre::Entity*) data;

	OgreNewt::World* mWorld = Global::nWorld;
	node = mEntity->getParentSceneNode();

	node->setScale(0.02,0.02,0.02);

	OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld, mEntity,0));

	body = new OgreNewt::Body( mWorld, col );
	Ogre::Vector3 inertia, offset;
	col->calculateInertialMatrix(inertia, offset);

	body->setMassMatrix( 0, inertia );
	body->setCenterOfMass(offset);
	body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation() );
	body->attachNode( node );

}

bool HandGripReaction::start()
{
	if(running)
		return 0;

	mAnimationState = mEntity->getAnimationState("prsty");
	mAnimationState->setLoop(false);
	mAnimationState->setEnabled(true);

	Ogre::Vector3 targetScale = Ogre::Vector3(0.06,0.06,0.06);
	targetScale *= (id+1)/6.0f;
	targetScale += Ogre::Vector3(0.02,0.02,0.02);

	Ogre::Vector3 targetPos = Ogre::Vector3(35,36,27) - node->getPosition();
	targetPos *= (id+1)/6.0f;
	targetPos += node->getPosition();

	Ogre::Quaternion randomRot = Ogre::Quaternion::IDENTITY;

	if(id<5)
		randomRot.FromAngleAxis(Ogre::Degree(Ogre::Math::RangeRandom(-50,50)),Ogre::Vector3(0.2,1,0));

	Ogre::Quaternion randomRot2 = Ogre::Quaternion::IDENTITY;
	randomRot2.FromAngleAxis(Ogre::Degree(-100),Ogre::Vector3(1,0,0));

	Ogre::Quaternion targetOr = node->getOrientation()*randomRot;
	targetOr = targetOr*randomRot2;

	setTransformationParams(node->getOrientation(),targetOr,node->getPosition(),targetPos,Ogre::Vector3(0.02,0.02,0.02), targetScale);

	timer=0;
	running=true;

	if(id==0)
		targetTime=2;
	else
	{
		targetTime = (id+1)/6.0f;
		targetTime = 1.0f * targetTime;
		reactionTime = targetTime/(id+1);

		irrklang::ISound* music = Global::soundEngine->play3D("../../media/baium_earthquake.wav",irrklang::vec3df(targetPos.x,targetPos.y,targetPos.z), false, false, true, irrklang::ESM_AUTO_DETECT, true);
		music->setMaxDistance(25);
		music->setVolume(10);
		music->setPlaybackSpeed(Global::timestep);
		if(Global::timestep<1)
			music->getSoundEffectControl()->enableWavesReverbSoundEffect(0,-10*Global::timestep,2600,0.5);
	}

	return 1;
}

bool HandGripReaction::update(Ogre::Real tslf)
{

	tslf *= Global::timestep;
	timer+=tslf;

	if(id==0 && targetTime>1)
	{
		if(timer<2)
			return 1;

		targetTime = (id+1)/6.0f;
		targetTime = 1.0f * targetTime;
		reactionTime = targetTime/2.0f;

		irrklang::ISound* music = Global::soundEngine->play3D("../../media/baium_earthquake.wav",irrklang::vec3df(node->getPosition().x,node->getPosition().y,node->getPosition().z), false, false, true, irrklang::ESM_AUTO_DETECT, true);
		music->setMinDistance(5);
		music->setMaxDistance(25);
		music->setVolume(10);
		music->setPlaybackSpeed(Global::timestep);
		if(Global::timestep<1)
			music->getSoundEffectControl()->enableWavesReverbSoundEffect(0,-10*Global::timestep,2600,0.5);

		timer=tslf;
	}

	updateTransformation();

	node->setScale(currentScale);
	body->setPositionOrientation(currentPos,currentOr);

	float animUpdate = tslf*mAnimationState->getLength()/targetTime;
	mAnimationState->addTime(animUpdate);

	if(timer>reactionTime)
	{
		reactionTime = targetTime+20;

		if(id<5)
		{
			TriggerInfo* info = Ogre::any_cast<TriggerInfo*>(mEntity->getUserAny());

			EventTask* evt = *info->tasks.begin()->second.begin();

			evt->start();
			Global::eventsMgr->addCachedTask(evt);
		}
	}

	if(timer>targetTime)
	{
		reaction();
		running=false;
		return 0;
	}

	return 1;
}

//stuff
void HandGripReaction::reaction()
{

	if(id==5)
	{
		irrklang::ISound* music = Global::soundEngine->play3D("../../media/boom.wav",irrklang::vec3df(35,36,27), false, false, true, irrklang::ESM_AUTO_DETECT, true);
		music->setMinDistance(20);
		music->setMaxDistance(100);
		music->setVolume(10);
		music->setPlaybackSpeed(Global::timestep);
		if(Global::timestep<1)
			music->getSoundEffectControl()->enableWavesReverbSoundEffect(0,-10*Global::timestep,2600,0.5);

		Global::player->pCamera->startCameraShake(10,3,0.4);
	}

	OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(Global::nWorld, mEntity,0));
	body->setCollision(col);

}
