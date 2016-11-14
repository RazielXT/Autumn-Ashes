#ifndef PPMGR
#define PPMGR

#include "stdafx.h"
#include "LutTexture.h"
#include "PPListener.h"

class PostProcessMgr
{
public:

	PostProcessMgr(Ogre::Camera* camera);
	~PostProcessMgr();

	void update(float tslf);
	void resetValues();

	void fadeOut(Ogre::Vector3 targetColor, float duration, bool skipFrame = false);
	void fadeIn(Ogre::Vector3 startColor, float duration, bool skipFrame = false);

	void setGodraySunPosition(Ogre::Vector3 pos);
	void setGodraySunPositionFar(Ogre::Vector3 pos);
	void setAutoGodraySunDirection();

	PostProcessVariables vars;
	Ogre::Camera* camera;

	std::string getCurrentGradingPreset();
	std::vector<std::string> getColorGradingPresets();
	void setColorGradingPreset(std::string name);

private:

	PostProcessListener ppListener;
	LutTexture	lut;

	bool skipFadeFrame = false;
	float totalBlacktime,currentBlacktime;

	Ogre::Vector3 sunPosition;
	float sunDistance;
};

#endif