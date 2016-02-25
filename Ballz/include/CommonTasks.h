#ifndef TRELR_H
#define TRELR_H

#include "stdafx.h"
#include "EventTask.h"
#include "TransformationTaskTemplate.h"
#include "InputListener.h"

class ActivateTriggerTask : public EventTask
{
public:

	ActivateTriggerTask();

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;

};

class DeactivateTriggerTask : public EventTask
{
public:

	DeactivateTriggerTask();

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;

};

class GiveTargetMark : public EventTask
{
public:

	GiveTargetMark();

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;

};

class ShowTargetMark : public EventTask
{
public:

	ShowTargetMark();

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;

};

class HideTargetMark : public EventTask
{
public:

	HideTargetMark();

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;

};

class CustomBodyForce : public EventTask
{
public:

	/*
	N/N/N/N
	power/lifetime/turbulence/gravity
	float/float/bool/float/float
	data Body
	*/
	CustomBodyForce(Ogre::String info);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

	void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex );

private:

	OgreNewt::Body* mBody;
	float timer,lifetime,power,gravity,turbulence;
	Ogre::Vector3 dir, force;
};

class CustomOtherBodyForce : public EventTask
{
public:

	/*
	P/N/N/N
	name/power/lifetime/gravity
	string/float/float/bool/float
	data Body
	*/
	CustomOtherBodyForce(Ogre::String info);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

	void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex );

private:

	OgreNewt::Body* mBody;
	float timer,lifetime,power,gravity;
	Ogre::Vector3 dir;
};

class MakeSmoke : public EventTask
{
public:

	/*
	N/N/N/N
	fromBody/lifetime/emission/material
	bool/float/float/string
	data Body/Entity
	*/
	MakeSmoke(Ogre::String info);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

private:

	Ogre::String mat;
	void* uData;
	bool fromBody;
	float timer,lifetime,emission;
	Ogre::ParticleSystem* ps;
	Ogre::Vector3 pos;
};

class PlaySound : public EventTask
{
public:

	/*
	P/N/N/N/N/N/N/N/N
	soundName/3D/volume/loop/fromBody/listed/tracked/lifetime/playPos/smoothing
	string/bool/0-1/bool/bool/bool/bool/float/float/float
	data Body
	*/
	PlaySound(Ogre::String soundName);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

private:

	void* uData;
	Ogre::String sound;
	Ogre::Vector3 pos;
	bool loop;
	bool withPosition, listed, tracked;
	char decayStage;
	float volume, lifetime, decay, timer, fromBody,playPos;
	irrklang::ISound* s;

};



class StopSound : public EventTask
{
public:

	/*
	P/N
	soundName/smoothing
	string/float
	data Body
	*/
	StopSound(Ogre::String soundName);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

private:

	Ogre::String sound;
	float smooth, timer, basicVolume;
	irrklang::ISound* s;
	bool running;

};


class PlayRandomSound : public EventTask
{
public:

	/*
	P/N
	soundGroup/body must be moving
	string/bool
	data Body
	*/
	PlayRandomSound(Ogre::String soundGroup);

	void setUserData(void* data);
	bool start();

private:

	char type;
	OgreNewt::Body* mBody;
	std::vector<Ogre::String> sounds;

};


class ChangeMat : public EventTask
{
public:

	/*
	P/N
	material/matID
	string/int
	data Body/Entity
	*/
	ChangeMat(Ogre::String info);

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;
	Ogre::Entity* ent;
	Ogre::String material;
	int matID;

};

class VisibilityMask : public EventTask
{
public:

	/*
	P/N
	mask/duration/type(0-just game/1-all/2-just visibility)
	int/float/char
	data -
	*/
	VisibilityMask(Ogre::String info);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

private:

	float duration, timer;
	char type;
	Ogre::uint32 oldMask, newMask;

};

class ShakeCamera : public EventTask
{
public:

	/*
	P/P/P/N
	impulse/power/steps/maxDistance
	float/float/float/float
	data Body
	*/
	ShakeCamera(Ogre::String shakeInfo);

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;
	float steps;
	float impulse, power, maxDistance;

};


class ChangeEntityTask : public EventTask
{
public:

	/*
	P
	entity
	string
	data Body
	*/
	ChangeEntityTask(Ogre::String entName);

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;
	Ogre::String entityName;

};

class ChangeMass : public EventTask
{
public:

	/*
	P
	mass
	float
	data Body
	*/
	ChangeMass(Ogre::String newmass);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

private:

	OgreNewt::Body* mBody;
	float mass;
	Ogre::Vector3 inertia;

};


class DeleteBody : public EventTask
{
public:

	DeleteBody();

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;

};

class Continual2StepPhysicalNodeAnimation : public EventTask
{
public:

	/*
	P/P/P
	master/anim1/anim2
	bool/string/string
	data Body
	*/
	Continual2StepPhysicalNodeAnimation(Ogre::String animationInfo);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);
	void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex );

private:

	bool running;
	bool master, firstTime, backwards;
	Ogre::String anim1,anim2;
	OgreNewt::Body* mBody,*mHelpBody;
	Ogre::SceneNode* node;
	Ogre::AnimationState* mAnimState;
	float backTimer;
	bool *stage2Flag, stage2LocalFlag, secondStage;

};

class StartPhysicalNodeAnimation : public EventTask
{
public:

	/*
	P/N/N
	animation/loop/direct
	string/bool/bool
	data Body
	*/
	StartPhysicalNodeAnimation(Ogre::String animationInfo);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);
	void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex );

private:

	bool running;
	bool loop, firstTime, direct;
	Ogre::String animName;
	OgreNewt::Body* mBody,*mHelpBody;
	Ogre::SceneNode* node;
	Ogre::AnimationState* mAnimState;
	OgreNewt::BallAndSocket* j0,*j1,*j2;

};


class StartNodeAnimation : public EventTask
{
public:

	/*
	P/N/N
	animation/loop/timer
	string/bool/float
	data Entity
	*/
	StartNodeAnimation(Ogre::String animationInfo);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

private:

	bool running;
	bool loop;
	float time,timer;
	Ogre::String animName;
	Ogre::SceneNode* node, *animNode;
	Ogre::AnimationState* mAnimState;

};


class ShowText : public EventTask
{
public:

	/*
	P/N
	text/delay
	string/float
	data -
	*/
	ShowText(Ogre::String text);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

private:

	Ogre::String text;
	float delay,timer;
};


class MakeParticle : public EventTask
{
public:

	/*
	P/N/N
	particle/lifetime/body
	string/float/bool
	data entity/body
	*/
	MakeParticle(Ogre::String particleInfo);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

private:

	bool fromBody;
	char type;
	float timer,lifetime;
	Ogre::ParticleSystem* ps;
};

class WalkingAnim : public TransformationTaskTemplate, public InputListener
{
public:

	/*
	P/N/N/N
	animation,walk to start(F),walking(T),walkingSpeed
	string,bool,bool,float
	data Entity
	*/
	WalkingAnim(Ogre::String animationInfo);
	virtual ~WalkingAnim() {}

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

	void pressedKey(const OIS::KeyEvent &arg);

private:

	Ogre::AnimationState* mAnimState;
	Ogre::String animName;
	Ogre::Vector3 lastPos;
	Ogre::SceneNode* animNode;
	Ogre::SceneNode* startAnimNode;
	float walkSinTimer, startWalkingSpeed;
	char finisher;
	bool walkToStartPos,walking,firstFrame;

};


class PushObject : public EventTask
{
public:

	/*
	P/P/N/N
	body/power/time
	string(0 to self)/float/float
	data Body
	*/
	PushObject(Ogre::String pushInfo);

	void setUserData(void* data);
	bool start();
	bool update(float tslf);

private:

	float power,timer,totalTime;
	OgreNewt::Body* body;
	Ogre::Vector3 direction;
	bool dirFromPlayer;

};


class ChangePositionOrientation : public EventTask
{
public:

	/*
	P/P/P/P/P/P/P
	x/y/z/x/y/z/w
	float/float/float/float/float/float/float
	data Body
	*/
	ChangePositionOrientation(Ogre::String coord);

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;
	Ogre::Quaternion or;
	Ogre::Vector3 pos;
};

class ChangeOrientation : public EventTask
{
public:

	/*
	P/P/P/P
	x/y/z/w
	float/float/float/float
	data Body
	*/
	ChangeOrientation(Ogre::String coord);

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;
	Ogre::Quaternion or;

};

class ChangePosition : public EventTask
{
public:

	/*
	P/P/P
	x/y/z
	float/float/float
	data Body
	*/
	ChangePosition(Ogre::String coord);

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;
	Ogre::Vector3 pos;

};

class ChangePlayerPosition : public EventTask
{
public:

	/*
	P/P/P
	x/y/z
	float/float/float
	data Body
	*/
	ChangePlayerPosition(Ogre::String coord);

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;
	Ogre::Vector3 pos;

};

class TranslatePosition : public EventTask
{
public:

	/*
	P/P/P
	x/y/z
	float/float/float
	data Body
	*/
	TranslatePosition(Ogre::String coord);

	void setUserData(void* data);
	bool start();

private:

	OgreNewt::Body* mBody;
	Ogre::Vector3 pos;

};

#endif