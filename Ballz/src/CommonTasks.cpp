#include "stdafx.h"
#include "CommonTasks.h"
#include "BodyUserData.h"
#include "Player.h"
#include "GameStateManager.h"
#include "GameUi.h"

#define BITS_20_TO_31 4290772992
#define BITS_0_TO_19 1048575
#define BIT_0 1
#define BIT_1 2
#define BIT_2 4
#define BIT_3 8
#define BIT_4 16
#define BIT_5 32
#define BIT_6 64
#define BIT_7 128
#define BIT_8 256
#define BIT_9 512
#define BIT_10 1024
#define BIT_11 2048
#define BIT_12 4096
#define BIT_13 8192
#define BIT_14 16384
#define BIT_15 32768
#define BIT_16 65536
#define BIT_17 131072
#define BIT_18 262144
#define BIT_19 524288
#define BIT_20 1048576
#define BIT_21 2097152
#define BIT_22 4194304
#define BIT_23 8388608
#define BIT_24 16777216
#define BIT_25 33554432
#define BIT_26 67108864
#define BIT_27 134217728
#define BIT_28 268435456
#define BIT_29 536870912
#define BIT_30 1073741824
#define BIT_31 2147483648

int markUniqInt = 0;

ActivateTriggerTask::ActivateTriggerTask() {}

void ActivateTriggerTask::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool ActivateTriggerTask::start()
{
	bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(mBody->getUserData());
	bUD->enabledTrigger = true;

	return false;
}


DeactivateTriggerTask::DeactivateTriggerTask() {}

void DeactivateTriggerTask::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}
bool DeactivateTriggerTask::start()
{
	bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(mBody->getUserData());
	bUD->enabledTrigger = false;

	return false;
}


GiveTargetMark::GiveTargetMark() {}

void GiveTargetMark::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool GiveTargetMark::start()
{

	Ogre::ParticleSystem* ps = Global::mSceneMgr->createParticleSystem("Mark"+Ogre::StringConverter::toString(markUniqInt++), "markTest");
	ps->setEmitting(false);
	ps->setVisibilityFlags(Global::mSceneMgr->getSceneNode(mBody->getOgreNode()->getName())->getAttachedObject(0)->getVisibilityFlags());
	((Ogre::SceneNode*)mBody->getOgreNode())->attachObject(ps);

	bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(mBody->getUserData());
	bUD->customData = ps;

	return false;
}


ShowTargetMark::ShowTargetMark() {}

void ShowTargetMark::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool ShowTargetMark::start()
{
	bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(mBody->getUserData());
	Ogre::ParticleSystem* ps = (Ogre::ParticleSystem*) bUD->customData;
	ps->setEmitting(true);

	return false;
}


HideTargetMark::HideTargetMark() {}

void HideTargetMark::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool HideTargetMark::start()
{
	bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(mBody->getUserData());
	Ogre::ParticleSystem* ps = (Ogre::ParticleSystem*) bUD->customData;
	ps->setEmitting(false);

	return false;
}

ChangeMass::ChangeMass(Ogre::String newmass)
{
	mass = Ogre::StringConverter::parseReal(newmass);
}

void ChangeMass::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
	inertia = mBody->getInertia();
}

bool ChangeMass::start()
{
	return true;
}

bool ChangeMass::update(float tslf)
{
	//Ogre::Vector3 offset = mBody->getCenterOfMass();

	mBody->setMassMatrix( mass, inertia );
	//mBody->setVelocity(Ogre::Vector3::ZERO);
	//mBody->setOmega(Ogre::Vector3::ZERO);
	//mBody->setCenterOfMass(offset);
	//mBody->setLinearDamping(0.3);
	//mBody->setStandardForceCallback();
	mBody->unFreeze();

	return false;
}

DeleteBody::DeleteBody()
{
}

void DeleteBody::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool DeleteBody::start()
{
	delete mBody;
	return false;
}

PlaySound::PlaySound(Ogre::String soundName)
{
	loop = false;
	volume = 1;
	withPosition = true;
	listed = false;
	fromBody = true;
	tracked = false;
	lifetime = 0;
	decay = 0;
	timer = 0;
	playPos = 0;

	const char* text = soundName.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);
	sound = AudioLibrary::getPath(Ogre::String(temp));

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	withPosition = Ogre::StringConverter::parseBool(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	volume = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	loop = Ogre::StringConverter::parseBool(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	fromBody = Ogre::StringConverter::parseBool(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	listed = Ogre::StringConverter::parseBool(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	tracked = Ogre::StringConverter::parseBool(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	lifetime = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	playPos = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	decay = Ogre::StringConverter::parseReal(temp);

	if(lifetime && decay*2>lifetime)
		decay = lifetime/2.0f;


	//Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("LOAD created sound "+sound+" and "+Ogre::StringConverter::toString(withPosition)+" and v "+Ogre::StringConverter::toString(volume)+" and l "+Ogre::StringConverter::toString(loop),Ogre::LML_NORMAL);
}

void PlaySound::setUserData(void* data)
{
	uData = (fromBody) ? data : ((Ogre::Entity*) data)->getParentSceneNode();
}

bool PlaySound::start()
{
	//Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Task playing sound "+sound+", lifetime "+Ogre::StringConverter::toString(lifetime),Ogre::LML_NORMAL);

	if(timer && lifetime)
	{
		s->stop();
		s->drop();
		timer = 0;
	}

	if(withPosition)
	{
		pos = (fromBody) ? ((OgreNewt::Body*) uData)->getPosition() : ((Ogre::SceneNode*) uData)->getPosition();
		s = Global::soundEngine->play3D(sound.c_str(),irrklang::vec3df(pos.x,pos.y,pos.z), loop, true, true, irrklang::ESM_AUTO_DETECT,false);
		s->setMinDistance(3);
	}
	else
		s = Global::soundEngine->play2D(sound.c_str(),loop, true, true, irrklang::ESM_AUTO_DETECT,false);

	s->setPlayPosition((UINT)playPos);

	if(decay)
	{
		s->setVolume(0);
		decayStage = 1;
	}
	else
	{
		s->setVolume(volume);
		decayStage = 0;
	}

	s->setIsPaused(false);

	if(listed)
		(*Global::globalData)[sound] = s;


	if(!lifetime && !decay && !tracked)
	{
		if(!listed)
			s->drop();

		return false;
	}
	else
		return true;
}

bool PlaySound::update(float tslf)
{
	timer += tslf;

	if((lifetime && lifetime<timer) || s->isFinished())
	{
		if(listed)
		{
			auto it = Global::globalData->find(sound);
			if(it != Global::globalData->end() && it->second == s)
				Global::globalData->erase(it);
		}

		s->stop();
		s->drop();

		timer = 0;
		return false;
	}

	if(decayStage == 1)
	{
		float curVol = Ogre::Math::Clamp(volume*(timer/decay),0.0f,volume);
		s->setVolume(curVol);

		if(curVol==volume)
		{
			if(lifetime)
				decayStage = 2;
			else
				return false;
		}
	}


	if(decayStage == 2 && (lifetime-decay)<timer)
	{
		float curVol = Ogre::Math::Clamp(volume*((lifetime-timer)/decay),0.0f,volume);
		s->setVolume(curVol);
	}

	if(tracked)
	{
		pos = (fromBody) ? ((OgreNewt::Body*) uData)->getPosition() : ((Ogre::SceneNode*) uData)->getPosition();
		s->setPosition(irrklang::vec3df(pos.x,pos.y,pos.z));
	}

	return true;
}

MakeSmoke::MakeSmoke(Ogre::String info)
{
	lifetime = 1;
	emission = 4;
	timer = 0;
	fromBody = false;
	mat = "SmokeMat1";

	const char* text = info.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);
	fromBody = Ogre::StringConverter::parseBool(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	lifetime = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	emission = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	mat = Ogre::String(temp);

}

void MakeSmoke::setUserData(void* data)
{
	uData = data;

	ps = Global::mSceneMgr->createParticleSystem("SmokeParticle"+Ogre::StringConverter::toString(markUniqInt++), mat);
	ps->setEmitting(false);
	ps->getEmitter(0)->setEmissionRate(emission);

	Ogre::SceneNode* sn;
	if(!fromBody)
	{
		Ogre::Entity* mEntity = (Ogre::Entity*) data;
		sn = mEntity->getParentSceneNode();
		ps->setVisibilityFlags(mEntity->getVisibilityFlags());
	}
	else
	{
		sn = Global::mSceneMgr->getSceneNode(((OgreNewt::Body*) data)->getOgreNode()->getName());
		ps->setVisibilityFlags(sn->getAttachedObject(0)->getVisibilityFlags());
	}

	sn->attachObject(ps);
}

bool MakeSmoke::start()
{
	ps->setEmitting(true);

	if(!lifetime)
		return false;

	timer = 0;

	return true;
}

bool MakeSmoke::update(float tslf)
{
	timer += tslf;

	if(timer>=lifetime)
	{
		ps->setEmitting(false);
		return false;
	}

	return true;
}

StopSound::StopSound(Ogre::String soundName)
{
	smooth = false;
	timer = 0;
	running = false;

	const char* text = soundName.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);
	sound = "../../media/"+Ogre::String(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	smooth = Ogre::StringConverter::parseReal(temp);
}

void StopSound::setUserData(void* data)
{
}

bool StopSound::start()
{
	//Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Task playing sound "+sound,Ogre::LML_NORMAL);

	auto it = Global::globalData->find(sound);
	if(it != Global::globalData->end() && !running)
	{
		s = (irrklang::ISound*) it->second;
		if(s->isFinished())
		{
			Global::globalData->erase(it);
			return false;
		}
	}
	else
		return false;


	if(smooth)
	{
		running = true;
		timer = 0;
		basicVolume = s->getVolume();
		return true;
	}
	else
	{
		s->stop();
		s->drop();
		return false;
	}
}

bool StopSound::update(float tslf)
{
	timer+=tslf;

	if(timer>=smooth)
	{
		s->stop();
		running = false;
		return false;
	}

	s->setVolume(basicVolume*(1-timer/smooth));

	return true;
}

PlayRandomSound::PlayRandomSound(Ogre::String soundGroup)
{
	type = 0;

	const char* text = soundGroup.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);
	Ogre::String soundG = Ogre::String(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp!=NULL)
	{
		type = Ogre::StringConverter::parseInt(temp);
	}


	if(soundG == "Bush")
	{
		sounds.push_back("bush1.wav");
		sounds.push_back("bush2.wav");
		sounds.push_back("bush3.wav");
	}
	if(soundG == "OldWood")
	{
		sounds.push_back("cracking.wav");
		sounds.push_back("wooden-plank-snap.wav");
		sounds.push_back("creak1.wav");
		sounds.push_back("creak2.wav");
	}

	Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("LOAD created random sound group "+soundG,Ogre::LML_NORMAL);
}

void PlayRandomSound::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool PlayRandomSound::start()
{
	Ogre::Vector3 pos;

	if(type == 2)
	{
		if(Global::player->body->getVelocity().squaredLength()<0.01)
			return false;

		pos = Global::player->body->getPosition();
	}
	else
	{
		if(type == 1)
		{
			if(mBody->getVelocity().squaredLength()<0.01)
				return false;
		}

		pos = mBody->getPosition();
	}

	int rand = (int) Ogre::Math::RangeRandom(0,sounds.size()-0.01f);
	Ogre::String sound = "../../media/" + sounds.at(rand);

	Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("PLAY random sound "+sound,Ogre::LML_NORMAL);
	Global::soundEngine->play3D(sound.c_str(),irrklang::vec3df(pos.x,pos.y,pos.z), false, false, false, irrklang::ESM_AUTO_DETECT,false);
	/*s->setMinDistance(3);
	s->setIsPaused(false);
	s->drop();*/

	return false;
}


ChangeEntityTask::ChangeEntityTask(Ogre::String entName)
{
	entityName = entName;
}

void ChangeEntityTask::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool ChangeEntityTask::start()
{
	Ogre::SceneNode* n = Global::mSceneMgr->getSceneNode(mBody->getOgreNode()->getName());
	Ogre::Entity* oldEnt = (Ogre::Entity*) n->getAttachedObject(0);
	n->detachAllObjects();
	Ogre::Entity* newEnt = Global::mSceneMgr->createEntity(entityName+".mesh");
	newEnt->setVisibilityFlags(oldEnt->getVisibilityFlags());
	Global::mSceneMgr->destroyEntity(oldEnt);

	n->attachObject(newEnt);

	return false;
}


CustomBodyForce::CustomBodyForce(Ogre::String info)
{
	const char* text = info.c_str();
	char* token;

	power = 1;
	lifetime = 0;
	gravity = -10;
	turbulence = 0;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);

	if(temp==NULL)
		return;
	power = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	lifetime = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	turbulence = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	gravity = Ogre::StringConverter::parseReal(temp);
}

void CustomBodyForce::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;

	dir = mBody->getOrientation()*Ogre::Vector3(1,0,0);
	dir.normalise();

	force=dir*power;
	force.y+=gravity;
}

bool CustomBodyForce::start()
{
	if(mBody->isFreezed())
		mBody->unFreeze();

	mBody->setCustomForceAndTorqueCallback<CustomBodyForce>(&CustomBodyForce::move_callback, this);

	if(!lifetime)
		return false;

	timer = lifetime;
	return true;
}

bool CustomBodyForce::update(float tslf)
{
	timer-=tslf;

	if(turbulence)
	{
		float timer1=std::fmod(timer,5.0f);
		float timer2 = std::fmod(timer1,0.75f);
		force = dir*power*Ogre::Math::Clamp(timer1/5.0f-timer2/3.0f,0.5f,1.0f);
		force.y+=gravity;
	}

	if(timer<=0)
	{
		mBody->setStandardForceCallback();
		return false;
	}

	return true;
}

void CustomBodyForce::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
	if(turbulence && !lifetime)
	{
		timer=std::fmod(timer+timeStep,5.0f);
		float timer2 = std::fmod(timer,0.75f);
		force = dir*power*Ogre::Math::Clamp(timer/5.0f-timer2/3.0f,0.5f,1.0f);
		force.y+=gravity;
	}

	mBody->addForce(force);
}

CustomOtherBodyForce::CustomOtherBodyForce(Ogre::String info)
{
	const char* text = info.c_str();
	char* token;

	lifetime = 0;
	gravity = -10;
	power = 1;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);
	Ogre::String name = Ogre::String(temp);

	auto bodies = Global::gameMgr->loadedBodies;
	auto it = bodies->find(name);
	if(it!=bodies->end())
		mBody = it->second;
	else
	{
		Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("WARNING! In task CustomOtherBodyForce, body "+name+" not found",Ogre::LML_NORMAL);
		mBody = NULL;
	}

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	power = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	lifetime = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	gravity = Ogre::StringConverter::parseReal(temp);
}

void CustomOtherBodyForce::setUserData(void* data)
{
	dir = (((Ogre::Entity*) data)->getParentSceneNode()->getOrientation()*Ogre::Vector3(1,0,0));
	dir.normalise();

	dir*=power;
	dir.y+=gravity;
}

bool CustomOtherBodyForce::start()
{
	if(mBody)
		mBody->setCustomForceAndTorqueCallback<CustomOtherBodyForce>(&CustomOtherBodyForce::move_callback, this);
	else
		return false;

	if(!lifetime)
		return false;

	timer = lifetime;
	return true;
}

bool CustomOtherBodyForce::update(float tslf)
{
	timer-=tslf;
	if(timer<=0)
	{
		mBody->setStandardForceCallback();
		return false;
	}

	return true;
}

void CustomOtherBodyForce::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
	mBody->addForce(dir);
}

StartNodeAnimation::StartNodeAnimation(Ogre::String animationInfo)
{
	const char* text = animationInfo.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);
	animName = Ogre::String(temp);

	loop = false;
	running = false;
	time = 0;

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	loop = Ogre::StringConverter::parseBool(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	time = Ogre::StringConverter::parseReal(temp);
}

void StartNodeAnimation::setUserData(void* data)
{
	node = ((Ogre::Entity*) data)->getParentSceneNode();

	mAnimState = Global::mSceneMgr->createAnimationState(animName);
	mAnimState->setLoop(loop);

	animNode = Global::mSceneMgr->getSceneNode(node->getName()+"Anim");
}

bool StartNodeAnimation::start()
{
	if(running)
		return false;

	timer = time;
	mAnimState->setTimePosition(0);
	mAnimState->setEnabled(true);

	return true;
}

bool StartNodeAnimation::update(float tslf)
{

	if(mAnimState->hasEnded())
	{
		running = false;
		return false;
	}

	float t = tslf*Global::timestep;

	if(!time)
	{
		mAnimState->addTime(t);
		node->setPosition(animNode->getPosition());
		node->setOrientation(animNode->getOrientation());
	}
	else
	{
		timer-=t;

		if(timer<=0)
		{
			running = false;
			mAnimState->setEnabled(false);
			return false;
		}
	}

	return true;
}


Continual2StepPhysicalNodeAnimation::Continual2StepPhysicalNodeAnimation(Ogre::String animationInfo)
{
	const char* text = animationInfo.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);
	master = Ogre::StringConverter::parseBool(temp);

	temp = strtok_s(NULL,"/",&token);
	anim1 = Ogre::String(temp);
	temp = strtok_s(NULL,"/",&token);
	anim2 = Ogre::String(temp);

	running = false;
	backwards = false;
	backTimer = 0;
}

void Continual2StepPhysicalNodeAnimation::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
	node = Global::mSceneMgr->getSceneNode(mBody->getOgreNode()->getName()+"Anim");
	mBody->setPositionOrientation(node->_getDerivedPosition(),node->_getDerivedOrientation());

	if(master)
	{
		OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(Global::mWorld,Ogre::Vector3(0.1,0.1,0.1),0));
		mHelpBody = new OgreNewt::Body( Global::mWorld, col );
		mHelpBody->setPositionOrientation( mBody->getPosition(), Ogre::Quaternion::IDENTITY );
		Ogre::Vector3 inertia, offset;
		col->calculateInertialMatrix(inertia, offset);
		mHelpBody->setMassMatrix( 0, inertia );

		(*Global::globalData)[mBody->getOgreNode()->getName()+"AnimBody"] = mHelpBody;

		OgreNewt::BallAndSocket* j0=new OgreNewt::BallAndSocket(mBody,mHelpBody,mHelpBody->getPosition()+Ogre::Vector3(0,0.5,0),0);
		j0->setCollisionState(0);
		OgreNewt::BallAndSocket* j1=new OgreNewt::BallAndSocket(mBody,mHelpBody,mHelpBody->getPosition()+Ogre::Vector3(0.25,-0.5,0),0);
		j1->setCollisionState(0);
		OgreNewt::BallAndSocket* j2=new OgreNewt::BallAndSocket(mBody,mHelpBody,mHelpBody->getPosition()+Ogre::Vector3(-0.25,0.5,0),0);
		j2->setCollisionState(0);


		stage2Flag = new bool(false);
		Ogre::Any any = mBody->getUserData();
		bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(any);
		bUD->customData = stage2Flag;
	}
	else
	{
		Ogre::Any any = mBody->getUserData();
		bodyUserData* bUD=Ogre::any_cast<bodyUserData*>(any);
		stage2Flag = (bool*) bUD->customData;
		auto it = Global::globalData->find(mBody->getOgreNode()->getName()+"AnimBody");
		mHelpBody = (OgreNewt::Body*) it->second;
		Global::globalData->erase(it);
	}

	stage2LocalFlag = false;
	secondStage = false;
	mBody->setAutoSleep(0);
	mBody->setCustomForceAndTorqueCallback<Continual2StepPhysicalNodeAnimation>(&Continual2StepPhysicalNodeAnimation::move_callback, this);
}

bool Continual2StepPhysicalNodeAnimation::start()
{
	if(running)
		return false;

	Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Running12 with "+Ogre::StringConverter::toString(running),Ogre::LML_NORMAL);

	if(*stage2Flag!=stage2LocalFlag)
		secondStage = !secondStage;

	Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Running22 with "+Ogre::StringConverter::toString(running),Ogre::LML_NORMAL);

	if(backwards)
	{
		if(secondStage)
			mAnimState = Global::mSceneMgr->createAnimationState(anim2);
		else
			mAnimState = Global::mSceneMgr->createAnimationState(anim1);

		mAnimState->setLoop(false);
		backTimer = mAnimState->getLength();
		mAnimState->setTimePosition(backTimer);
		mAnimState->setEnabled(true);
	}
	else
	{
		if(secondStage)
			mAnimState = Global::mSceneMgr->createAnimationState(anim2);
		else
			mAnimState = Global::mSceneMgr->createAnimationState(anim1);

		mAnimState->setLoop(false);
		mAnimState->setEnabled(true);
	}

	*stage2Flag = stage2LocalFlag = !*stage2Flag;
	backwards = !backwards;
	running = true;

	return true;
}

bool Continual2StepPhysicalNodeAnimation::update(float tslf)
{
	Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Running with "+Ogre::StringConverter::toString(running),Ogre::LML_NORMAL);

	if(running)
	{

		if(!backwards)
		{
			mAnimState->addTime(-tslf*Global::timestep);

			if(!mAnimState->getTimePosition())
			{
				running = false;

				if(secondStage)
					Global::mSceneMgr->destroyAnimationState(anim2);
				else
					Global::mSceneMgr->destroyAnimationState(anim1);
			}
		}
		else
		{
			mAnimState->addTime(tslf*Global::timestep);

			if(mAnimState->hasEnded())
			{
				running = false;

				if(secondStage)
					Global::mSceneMgr->destroyAnimationState(anim2);
				else
					Global::mSceneMgr->destroyAnimationState(anim1);
			}
		}

		mHelpBody->setPositionOrientation(node->_getDerivedPosition(),node->_getDerivedOrientation());
	}

	return true;
}

void Continual2StepPhysicalNodeAnimation::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
}


StartPhysicalNodeAnimation::StartPhysicalNodeAnimation(Ogre::String animationInfo)
{
	const char* text = animationInfo.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);
	animName = Ogre::String(temp);

	loop = false;
	direct = false;
	running = false;
	firstTime = true;

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	loop = Ogre::StringConverter::parseBool(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	direct = Ogre::StringConverter::parseBool(temp);
}

void StartPhysicalNodeAnimation::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool StartPhysicalNodeAnimation::start()
{
	if(running)
		return false;

	node = Global::mSceneMgr->getSceneNode(mBody->getOgreNode()->getName()+"Anim");
	mBody->setPositionOrientation(node->_getDerivedPosition(),node->_getDerivedOrientation());

	if(!direct && !mBody->getMass())
		direct = true;

	if(!direct)
	{
		OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(Global::mWorld,Ogre::Vector3(0.1,0.1,0.1),0));
		mHelpBody = new OgreNewt::Body( Global::mWorld, col );
		mHelpBody->setPositionOrientation( mBody->getPosition(), Ogre::Quaternion::IDENTITY );
		Ogre::Vector3 inertia, offset;
		col->calculateInertialMatrix(inertia, offset);
		mHelpBody->setMassMatrix( 0, inertia );

		j0=new OgreNewt::BallAndSocket(mBody,mHelpBody,mHelpBody->getPosition()+Ogre::Vector3(0,0.5,0),0);
		j0->setCollisionState(0);
		j1=new OgreNewt::BallAndSocket(mBody,mHelpBody,mHelpBody->getPosition()+Ogre::Vector3(0.25,-0.5,0),0);
		j1->setCollisionState(0);
		j2=new OgreNewt::BallAndSocket(mBody,mHelpBody,mHelpBody->getPosition()+Ogre::Vector3(-0.25,0.5,0),0);
		j2->setCollisionState(0);
	}

	mBody->setAutoSleep(0);
	//mBody->setAngularDamping(Ogre::Vector3::ZERO);
	//mBody->setLinearDamping(0);

	if(!firstTime)
	{
		mAnimState->setTimePosition(0);
		mAnimState->setEnabled(true);
	}

	running = true;
	//delete mBody;

	//mBody->setCustomForceAndTorqueCallback<StartPhysicalNodeAnimation>(&StartPhysicalNodeAnimation::move_callback, this);
	return true;
}

bool StartPhysicalNodeAnimation::update(float tslf)
{

	if(firstTime)
	{
		firstTime = false;
		mAnimState = Global::mSceneMgr->createAnimationState(animName);
		mAnimState->setEnabled(true);
		mBody->setCustomForceAndTorqueCallback<StartPhysicalNodeAnimation>(&StartPhysicalNodeAnimation::move_callback, this);
		mAnimState->setLoop(loop);
	}

	if(mAnimState->hasEnded())
	{
		mBody->setAutoSleep(1);
		running = false;

		if(mBody->getMass())
			mBody->setStandardForceCallback();

		if(!direct)
		{
			delete j0;
			delete j1;
			delete j2;
		}

		return false;
	}
	//Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Updating anim "+node->getName()+" and0 npos "+Ogre::StringConverter::toString(node->_getDerivedPosition()),Ogre::LML_NORMAL);

	mAnimState->addTime(tslf*Global::timestep);
	//Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Updating anim "+node->getName()+" and0 npos "+Ogre::StringConverter::toString(node->_getDerivedPosition()),Ogre::LML_NORMAL);

	//Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Updating anim "+node->getName()+" and0 pos "+Ogre::StringConverter::toString(mBody->getPosition()),Ogre::LML_NORMAL);
	//Ogre::Vector3 vel = (node->getPosition()-mBody->getPosition())*5;

	if(direct)
		mBody->setPositionOrientation(node->_getDerivedPosition(),node->_getDerivedOrientation());
	else
		mHelpBody->setPositionOrientation(node->_getDerivedPosition(),node->_getDerivedOrientation());

	//mBody->setPositionOrientation(node->getPosition(),Ogre::Quaternion::IDENTITY);
	//mBody->setVelocity(vel);

	//Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Updating anim "+node->getName()+" and1 pos "+Ogre::StringConverter::toString(mBody->getPosition()),Ogre::LML_NORMAL);
//	Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Updating anim "+node->getName()+" and1 npos "+Ogre::StringConverter::toString(node->getPosition()),Ogre::LML_NORMAL);


	return true;
}

void StartPhysicalNodeAnimation::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
	//me->setForce(Ogre::Vector3(0,0,0));
	//me->setPositionOrientation(node->getPosition(),node->getOrientation());
}


ChangeMat::ChangeMat(Ogre::String info)
{
	matID = -1;
	char* token;
	char* temp = strtok_s(const_cast<char *> (info.c_str()),"/",&token);
	material = Ogre::String(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	matID = Ogre::StringConverter::parseInt(temp);
}

void ChangeMat::setUserData(void* data)
{
	if(matID<0)
	{
		ent = (Ogre::Entity*) data;
	}
	else
	{
		mBody = (OgreNewt::Body*) data;
		ent = (Ogre::Entity*) Global::mSceneMgr->getSceneNode(((OgreNewt::Body*) data)->getOgreNode()->getName())->getAttachedObject(0);
	}
}

bool ChangeMat::start()
{
	ent->setMaterialName(material);

	if(matID>=0)
	{
		Ogre::Any any = mBody->getUserData();
		if(!any.isEmpty())
			Ogre::any_cast<bodyUserData*>(any)->material = matID;
	}

	return false;
}


ShakeCamera::ShakeCamera(Ogre::String shakeInfo)
{
	maxDistance = 0;

	char* token;
	char* temp = strtok_s(const_cast<char *> (shakeInfo.c_str()),"/",&token);
	impulse = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	power = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	steps = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	maxDistance = Ogre::StringConverter::parseReal(temp);
}

void ShakeCamera::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool ShakeCamera::start()
{
	Player* p = Global::player;

	if(!maxDistance)
		p->startCameraShake(steps,power,impulse);
	else
	{
		float mod = Ogre::Math::Clamp((p->body->getPosition()-mBody->getPosition()).length()/maxDistance,0.0f,1.0f);
		steps*=mod;
		power*=mod;
		impulse*=mod;
		p->startCameraShake(steps,power,impulse);
	}

	return false;
}


ShowText::ShowText(Ogre::String text)
{
	const char* parseText = text.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (parseText),"/",&token);
	this->text = Ogre::String(temp);

	temp = strtok_s(NULL,"/",&token);

	if(temp!=NULL)
		delay = Ogre::StringConverter::parseReal(temp);
	else
		delay = 0;
}

void ShowText::setUserData(void* data)
{
}

bool ShowText::start()
{
	timer = 0;

	if(delay)
		return true;

	Global::gameMgr->myMenu->gameUi->showIngameText(text);
	return false;
}

bool ShowText::update(float tslf)
{
	timer += tslf;

	if(timer>=delay)
	{
		Global::gameMgr->myMenu->gameUi->showIngameText(text);
		return false;
	}

	return true;
}


MakeParticle::MakeParticle(Ogre::String particleInfo)
{
	const char* parseText = particleInfo.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (parseText),"/",&token);
	Ogre::String particleName = Ogre::String(temp);

	ps = Global::mSceneMgr->createParticleSystem("LoadedParticle"+Ogre::StringConverter::toString(markUniqInt++), particleName);
	ps->setEmitting(false);

	temp = strtok_s(NULL,"/",&token);

	if(temp!=NULL)
	{
		//info
		lifetime = Ogre::StringConverter::parseReal(Ogre::String(temp));
	}
	else
		lifetime = 0;

	temp = strtok_s(NULL,"/",&token);

	if(temp!=NULL)
	{
		//info
		fromBody = Ogre::StringConverter::parseBool(Ogre::String(temp));
	}
	else
		fromBody = false;

}

void MakeParticle::setUserData(void* data)
{
	Ogre::SceneNode* sn;
	if(!fromBody)
	{
		Ogre::Entity* mEntity = (Ogre::Entity*) data;
		sn = mEntity->getParentSceneNode();
		ps->setVisibilityFlags(mEntity->getVisibilityFlags());
	}
	else
	{
		sn = Global::mSceneMgr->getSceneNode(((OgreNewt::Body*) data)->getOgreNode()->getName());
		ps->setVisibilityFlags(sn->getAttachedObject(0)->getVisibilityFlags());
	}

	sn->attachObject(ps);
}

bool MakeParticle::start()
{
	ps->setEmitting(true);

	if(!lifetime)
		return false;

	timer = 0;

	return true;
}

bool MakeParticle::update(float tslf)
{
	timer += tslf;

	if(timer>=lifetime)
	{
		ps->setEmitting(false);
		return false;
	}

	return true;
}


WalkingAnim::WalkingAnim(Ogre::String animationInfo)
{
	const char* text = animationInfo.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);
	animName = Ogre::String(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp!=NULL)
	{
		Ogre::String walkToStartS = Ogre::String(temp);

		if(walkToStartS=="1")
			walkToStartPos = true;
		else
			walkToStartPos = false;
	}
	else
		walkToStartPos = false;

	temp = strtok_s(NULL,"/",&token);
	if(temp!=NULL)
	{
		Ogre::String walkWave = Ogre::String(temp);

		if(walkWave=="1")
			walking = true;
		else
			walking = false;
	}
	else
		walking = true;

	temp = strtok_s(NULL,"/",&token);
	if(temp!=NULL)
	{
		startWalkingSpeed = Ogre::StringConverter::parseReal(Ogre::String(temp));
	}
	else
		startWalkingSpeed = 1;

}

void WalkingAnim::setUserData(void* data)
{
	Ogre::SceneNode* node = ((Ogre::Entity*) data)->getParentSceneNode();
	node->detachAllObjects();
	Global::mSceneMgr->destroyEntity((Ogre::Entity*) data);
	animNode = Global::mSceneMgr->getSceneNode(node->getName()+"Anim");
	Global::mSceneMgr->destroySceneNode(node);
	animNode = animNode->createChildSceneNode();
}

bool WalkingAnim::start()
{
	registerInputListening();

	Ogre::Camera* cam = Global::mSceneMgr->getCamera("Camera");
	walkSinTimer = 0;
	timer = 0;
	finisher = 0;
	firstFrame = true;
	Global::player->enableControl(false);
	mAnimState = Global::mSceneMgr->createAnimationState(animName);
	mAnimState->setEnabled(true);
	mAnimState->setLoop(false);

	if(!walkToStartPos)
	{
		cam->detachFromParent();
		animNode->attachObject(cam);
		lastPos = animNode->_getDerivedPosition();
		lastPos.y = 0;
	}
	else
	{
		setTransformationParams(cam->getParentSceneNode()->_getDerivedOrientation(),animNode->_getDerivedOrientation(),cam->getParentSceneNode()->_getDerivedPosition(),animNode->_getDerivedPosition());
		cam->detachFromParent();
		startAnimNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(currentPos,currentOr);
		startAnimNode->attachObject(cam);
	}

	return true;
}

void WalkingAnim::pressedKey(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_SPACE)
	{
		mAnimState->setTimePosition(mAnimState->getLength());
		unregisterInputListening();
	}
}

bool WalkingAnim::update(float tslf)
{
	//Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Trans !!!"+ Ogre::StringConverter::toString(animNode->_getDerivedPosition())+ " and " + Ogre::StringConverter::toString(animNode->_getDerivedOrientation()),Ogre::LML_NORMAL);

	if(firstFrame)
	{
		firstFrame = false;
		if(walkToStartPos)
		{
			setTransformationParams(currentOr,animNode->_getDerivedOrientation(),currentPos,animNode->_getDerivedPosition());
			targetTime = (currentPos-animNode->_getDerivedPosition()).length()/(4*startWalkingSpeed);
		}

		return true;
	}

	//initial phase
	if(walkToStartPos)
	{
		timer+=tslf;
		updateTransformation();
		startAnimNode->setPosition(currentPos);
		startAnimNode->setOrientation(currentOr);

		if(timer>targetTime)
		{
			Global::mSceneMgr->getCamera("Camera")->detachFromParent();
			Global::mSceneMgr->destroySceneNode(startAnimNode);
			animNode->attachObject(Global::mSceneMgr->getCamera("Camera"));
			lastPos = animNode->_getDerivedPosition();
			lastPos.y = 0;
			walkToStartPos = false;
		}

	}
	else
		//animation phase
	{

		if(mAnimState->hasEnded())
		{
			if(walking)
			{
				if(!finisher)
				{
					if(animNode->getPosition().y>0)
						finisher = 1;
					else
						finisher = 2;
				}

				walkSinTimer += tslf*10;
				float pos = sin(walkSinTimer)/4;

				if(finisher==1 && pos>0)
				{
					animNode->setPosition(0,pos,0);
					return true;
				}

				if(finisher==2 && pos<0)
				{
					animNode->setPosition(0,pos,0);
					return true;
				}

				animNode->setPosition(0,0,0);
			}

			Global::player->body->setPositionOrientation(animNode->_getDerivedPosition()-Ogre::Vector3(0,2,0),Ogre::Quaternion::IDENTITY);
			Global::player->enableControl(true);
			Global::player->attachCamera();

			return false;
		}

		if(walking)
		{
			Ogre::Vector3 curPos = animNode->_getDerivedPosition();
			curPos.y = 0;
			walkSinTimer += (curPos-lastPos).length()/1.35f;
			lastPos = curPos;
			animNode->setPosition(0,sin(walkSinTimer)/4,0);
		}

		mAnimState->addTime(tslf);
	}

	return true;
}

PushObject::PushObject(Ogre::String pushInfo)
{
	const char* text = pushInfo.c_str();
	char* token;

	char* temp = strtok_s(const_cast<char *> (text),"/",&token);

	auto bodies = Global::gameMgr->loadedBodies;
	auto it = bodies->find(Ogre::String(temp));
	if(it!=bodies->end())
	{
		body = it->second;
		dirFromPlayer = false;
	}
	else
	{
		dirFromPlayer = true;
		body = NULL;
	}

	temp = strtok_s(NULL,"/",&token);
	power = Ogre::StringConverter::parseReal(temp);

	temp = strtok_s(NULL,"/",&token);
	if(temp!=NULL)
		totalTime = Ogre::StringConverter::parseReal(temp);

}


void PushObject::setUserData(void* data)
{
	if(body==NULL)
	{
		body = (OgreNewt::Body*) data;
	}
	else
	{
		direction = ((OgreNewt::Body*) data)->getOrientation()*Ogre::Vector3(1,0,0);
		direction.normalise();
	}
}

bool PushObject::start()
{
	if(dirFromPlayer)
	{
		direction = Global::player->getFacingDirection();
		direction.normalise();
	}

	if(totalTime==0)
	{
		body->setVelocity(body->getVelocity() + direction*power);
		return false;
	}

	timer = totalTime;
	return true;
}

bool PushObject::update(float tslf)
{
	body->setVelocity(body->getVelocity() + direction*power*tslf*10);

	timer-=tslf;

	if(timer<=0)
		return false;
	else
		return true;
}

VisibilityMask::VisibilityMask(Ogre::String info)
{
	const char* text = info.c_str();
	char* token;
	char* temp = strtok_s(const_cast<char *> (text),"/",&token);

	duration = 0;
	timer = 0;
	type = 0;

	newMask = Ogre::StringConverter::parseUnsignedInt(temp);
	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	duration = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	if(temp==NULL)
		return;
	type = Ogre::StringConverter::parseInt(temp);
}

void VisibilityMask::setUserData(void* data)
{
	if(type == 0)
	{
		newMask = newMask & BITS_0_TO_19;
	}
	else if(type == 2)
	{
		newMask = newMask & BITS_20_TO_31;
	}
}

bool VisibilityMask::start()
{
	if(timer!=0)
		return false;

	oldMask = Global::mSceneMgr->getVisibilityMask();

	if(type == 0)
	{
		newMask |= oldMask & BITS_20_TO_31;
	}
	else if(type == 2)
	{
		newMask |= oldMask & BITS_0_TO_19;
	}

	Global::mSceneMgr->setVisibilityMask(newMask);

	if(!duration)
		return false;
	else
		timer = duration;

	return true;
}

bool VisibilityMask::update(float tslf)
{
	timer-=tslf;

	if(timer<=0)
	{
		timer = 0;
		Global::mSceneMgr->setVisibilityMask(oldMask);
		return false;
	}

	return true;
}

ChangePosition::ChangePosition(Ogre::String coord)
{
	const char* text = coord.c_str();
	char* token;
	char* temp = strtok_s(const_cast<char *> (text),"/",&token);

	pos.x = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	pos.z = -Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	pos.y = Ogre::StringConverter::parseReal(temp);

}

void ChangePosition::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool ChangePosition::start()
{
	mBody->setPositionOrientation(pos, mBody->getOrientation());

	return false;
}

ChangePlayerPosition::ChangePlayerPosition(Ogre::String coord)
{
	const char* text = coord.c_str();
	char* token;
	char* temp = strtok_s(const_cast<char *> (text),"/",&token);

	pos.x = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	pos.z = -Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	pos.y = Ogre::StringConverter::parseReal(temp);

}

void ChangePlayerPosition::setUserData(void* data)
{
	mBody = Global::player->body;
}

bool ChangePlayerPosition::start()
{
	mBody->setPositionOrientation(pos, mBody->getOrientation());

	return false;
}

ChangeOrientation::ChangeOrientation(Ogre::String coord)
{
	const char* text = coord.c_str();
	char* token;
	char* temp = strtok_s(const_cast<char *> (text),"/",&token);

	or.x = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	or.y = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	or.z = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	or.w = Ogre::StringConverter::parseReal(temp);

}

void ChangeOrientation::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool ChangeOrientation::start()
{
	mBody->setPositionOrientation(mBody->getPosition() , or);

	return false;
}

ChangePositionOrientation::ChangePositionOrientation(Ogre::String coord)
{
	const char* text = coord.c_str();
	char* token;
	char* temp = strtok_s(const_cast<char *> (text),"/",&token);

	pos.x = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	pos.z = -Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	pos.y = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	or.x = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	or.y = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	or.z = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	or.w = Ogre::StringConverter::parseReal(temp);


	//Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("MOVING  "+Ogre::StringConverter::toString(pos)+" and "+Ogre::StringConverter::toString(or),Ogre::LML_NORMAL);
}

void ChangePositionOrientation::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool ChangePositionOrientation::start()
{
	mBody->setPositionOrientation(pos , or);

	return false;
}

TranslatePosition::TranslatePosition(Ogre::String coord)
{
	const char* text = coord.c_str();
	char* token;
	char* temp = strtok_s(const_cast<char *> (text),"/",&token);

	pos.x = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	pos.y = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL,"/",&token);
	pos.z = Ogre::StringConverter::parseReal(temp);

}

void TranslatePosition::setUserData(void* data)
{
	mBody = (OgreNewt::Body*) data;
}

bool TranslatePosition::start()
{
	mBody->setPositionOrientation(mBody->getPosition() + pos, mBody->getOrientation());

	return false;
}