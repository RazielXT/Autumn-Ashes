#include "stdafx.h"
#include "Tasks.h"

float getTaskDelay(Ogre::String& fname)
{
	//Delay(5):task
	if (!fname.compare(0, 6, "Delay("))
	{
		Ogre::String value = fname.substr(6, fname.find(')') - 6);
		fname.erase(0,fname.find(':')+1);
		return Ogre::StringConverter::parseReal(value);
	}
	else
		//Delay5/task
		if (!fname.compare(0, 5, "Delay"))
		{
			Ogre::String value = fname.substr(5, fname.find('/') - 5);
			size_t pos = fname.find('/');
			fname = fname.substr(pos + 1, fname.length() - pos - 1);
			return Ogre::StringConverter::parseReal(value);
		}
	return 0;
}

EventTask* makeTask(Ogre::String fname, float delay)
{
	if (fname == "TestReaction")
		return new TestReaction();
	else if (fname == "EntMovement")
		return new EntityMovementReaction();
	else if (fname == "ActivateTriggerTask")
		return new ActivateTriggerTask();
	else if (fname == "DeactivateTriggerTask")
		return new DeactivateTriggerTask();
	else if (fname == "ShowTargetMark")
		return new ShowTargetMark();
	else if (fname == "HideTargetMark")
		return new HideTargetMark();
	else if (fname == "GiveTargetMark")
		return new GiveTargetMark();
	else if (fname == "ChainsSoundTask")
		return new ChainsSoundTask();
	else if (fname == "ParkToDream")
		return new ParkToDreamReaction();
	else if (fname == "NearParkEnd")
		return new NearParkEndReaction();
	else if (fname == "OutOfCave")
		return new OutOfCave();
	else if (fname == "QuickBlink")
		return new QuickScaryBlink();
	else if (fname == "DeleteBody")
		return new DeleteBody();
	else if (fname == "ParkEnd")
		return new ParkEndReaction();
	else if (!fname.compare(0, 9, "PlaySound"))
	{
		Ogre::String sound = fname.substr(9, fname.length() - 9);
		return new PlaySound(sound);
	}
	else if (!fname.compare(0, 8, "ForceObj"))
	{
		Ogre::String info = fname.substr(8, fname.length() - 8);
		return new CustomOtherBodyForce(info);
	}
	else if (!fname.compare(0, 5, "Force"))
	{
		Ogre::String info = fname.substr(5, fname.length() - 5);
		return new CustomBodyForce(info);
	}
	else if (!fname.compare(0, 7, "VisMask"))
	{
		Ogre::String info = fname.substr(7, fname.length() - 7);
		return new VisibilityMask(info);
	}
	else if (!fname.compare(0, 9, "ChangeMat"))
	{
		Ogre::String sound = fname.substr(9, fname.length() - 9);
		return new ChangeMat(sound);
	}
	else if (!fname.compare(0, 8, "ShakeCam"))
	{
		Ogre::String info = fname.substr(8, fname.length() - 8);
		return new ShakeCamera(info);
	}
	else if (!fname.compare(0, 10, "ChangeMass"))
	{
		Ogre::String mass = fname.substr(10, fname.length() - 10);
		return new ChangeMass(mass);
	}
	else if (!fname.compare(0, 9, "StopSound"))
	{
		Ogre::String sound = fname.substr(9, fname.length() - 9);
		return new StopSound(sound);
	}
	else if (!fname.compare(0, 12, "ChangeEntity"))
	{
		Ogre::String entName = fname.substr(12, fname.length() - 12);
		return new ChangeEntityTask(entName);
	}
	else if (!fname.compare(0, 15, "PlayRandomSound"))
	{
		Ogre::String sound = fname.substr(15, fname.length() - 15);
		return new PlayRandomSound(sound);
	}
	else if (!fname.compare(0, 10, "C2PhysAnim"))
	{
		Ogre::String animInfo = fname.substr(10, fname.length() - 10);
		return new Continual2StepPhysicalNodeAnimation(animInfo);
	}
	else if (!fname.compare(0, 13, "StartPhysAnim"))
	{
		Ogre::String animInfo = fname.substr(13, fname.length() - 13);
		return new StartPhysicalNodeAnimation(animInfo);
	}
	else if (!fname.compare(0, 9, "StartAnim"))
	{
		Ogre::String animInfo = fname.substr(9, fname.length() - 9);
		return new StartNodeAnimation(animInfo);
	}
	else if (!fname.compare(0, 8, "ShowText"))
	{
		Ogre::String text = fname.substr(8, fname.length() - 8);
		return new ShowText(text);
	}
	else if (!fname.compare(0, 8, "Particle"))
	{
		Ogre::String text = fname.substr(8, fname.length() - 8);
		return new MakeParticle(text);
	}
	else if (!fname.compare(0, 8, "SetPosOr"))
	{
		Ogre::String text = fname.substr(8, fname.length() - 8);
		return new ChangePositionOrientation(text);
	}
	else if (!fname.compare(0, 6, "SetPos"))
	{
		Ogre::String text = fname.substr(6, fname.length() - 6);
		return new ChangePosition(text);
	}
	else if (!fname.compare(0, 12, "SetPlayerPos"))
	{
		Ogre::String text = fname.substr(12, fname.length() - 12);
		return new ChangePlayerPosition(text);
	}
	else if (!fname.compare(0, 5, "SetOr"))
	{
		Ogre::String text = fname.substr(5, fname.length() - 5);
		return new ChangeOrientation(text);
	}
	else if (!fname.compare(0, 12, "TranslatePos"))
	{
		Ogre::String text = fname.substr(12, fname.length() - 12);
		return new TranslatePosition(text);
	}
	else if (!fname.compare(0, 10, "PushObject"))
	{
		Ogre::String text = fname.substr(10, fname.length() - 10);
		return new PushObject(text);
	}
	else if (!fname.compare(0, 11, "WalkingAnim"))
	{
		Ogre::String animInfo = fname.substr(11, fname.length() - 11);
		return new WalkingAnim(animInfo);
	}

	return NULL;
}