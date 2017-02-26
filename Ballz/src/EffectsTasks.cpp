#include "stdafx.h"
#include "EffectsTasks.h"

QuickScaryBlink::QuickScaryBlink()
{
	timer = 0;
	totalTime = 0.55;
	effectStr = 1;
	doneBlink = false;
}

bool QuickScaryBlink::start()
{
	id = 1;

	if(timer>0)
		return false;

	doneBlink = false;
	mgr = (Global::ppMgr);

	std::string soundName;
	if(id==1)
	{
		//soundName = AudioLibrary::getPath("staticBlinkRip.wav");
		effectStr = 0.9;
		totalTime = 0.45f;
	}
	else if(id==2)
	{
		//soundName = AudioLibrary::getPath("staticBlinkShort.wav");
		effectStr = 0.7;
		totalTime = 0.4f;
	}
	else if(id==3)
	{
		//soundName = AudioLibrary::getPath("staticBlinkLong.wav");
		effectStr = 0.5;
		totalTime = 0.85f;
	}
	else
	{
		//soundName = AudioLibrary::getPath("staticBlink.wav");
		effectStr = 1;
		totalTime = 0.55f;
	}

	//Global::soundEngine->play2D(soundName.c_str(),false , false, false, irrklang::ESM_AUTO_DETECT, false);

	return true;
}

bool QuickScaryBlink::update(Ogre::Real tslf)
{
	timer+=tslf;

	float val = std::max(0.15f,std::min(timer*2.0f,totalTime*2-timer*2.0f));

	mgr->vars.radialHorizBlurVignette.y = val * 3 * effectStr;
	mgr->vars.ContSatuSharpNoise.w = val * 5 * effectStr;
	mgr->vars.ContSatuSharpNoise.z = val * 4 * effectStr;


	if(timer>totalTime)
	{
		timer = 0;
		mgr->vars.radialHorizBlurVignette.y = 0;
		mgr->vars.ContSatuSharpNoise.w = 0;
		mgr->vars.ContSatuSharpNoise.z = 0;
		return false;
	}

	return true;
}


SwitchColorSchemeFx::SwitchColorSchemeFx(Ogre::String info)
{
	const char* text = info.c_str();
	char* token;
	char* temp = strtok_s(const_cast<char *> (text), ",", &token);

	colorTarget.x = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL, ",", &token);

	colorTarget.y = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL, ",", &token);

	colorTarget.z = Ogre::StringConverter::parseReal(temp);
	temp = strtok_s(NULL, ",", &token);

	auto fulltime = Ogre::StringConverter::parseReal(temp);
	timeW = 1/fulltime;
}

bool SwitchColorSchemeFx::start()
{
	timer = 0;
	colorBase.x = Global::ppMgr->vars.ColouringShift.x;
	colorBase.y = Global::ppMgr->vars.ColouringShift.y;
	colorBase.z = Global::ppMgr->vars.ColouringShift.z;

	bloomStrBase = Global::ppMgr->vars.bloomStrDepAddSize.y;
	fovBase = Global::ppMgr->camera->getFOVy().valueDegrees();

	return true;
}

bool SwitchColorSchemeFx::update(float tslf)
{
	timer = std::min(1.0f, timer + tslf*timeW);

	const auto wHalfPoint = 0.25f;

	auto halfTopW = std::min(timer*1/wHalfPoint,1-(timer-wHalfPoint)/(1-wHalfPoint));
	auto fromHalfW = std::max(0.0f, (timer - wHalfPoint) / (1 - wHalfPoint));

	auto shift = colorTarget*fromHalfW + colorBase*(1 - fromHalfW);
	Global::ppMgr->vars.ColouringShift.x = shift.x;
	Global::ppMgr->vars.ColouringShift.y = shift.y;
	Global::ppMgr->vars.ColouringShift.z = shift.z;

	const auto stepMin = 0.15f;
	Global::timestep = pow(1 - halfTopW*(1-stepMin),1.5f);

	const auto blAdd = 3.0f;
	Global::ppMgr->vars.bloomStrDepAddSize.y = bloomStrBase + blAdd*halfTopW;

	Global::ppMgr->vars.ColouringShift.w = halfTopW;
	Global::ppMgr->vars.radialHorizBlurVignette.x = halfTopW;
	Global::ppMgr->vars.ContSatuSharpNoise.x = halfTopW;

	const auto fovAdd = 20.0f;
	Global::ppMgr->camera->setFOVy(Ogre::Degree(fovBase + fovAdd*halfTopW));

	if (timer == 1)
		return false;

	return true;
}

FovPeakEffect::FovPeakEffect()
{
}

bool FovPeakEffect::start(float time, float peakTime, float peak, float target /*= 0*/)
{
	duration = time;
	target = target != 0 ? target : Global::camera->defaultFov.valueDegrees();
	timer = 0;

	spline.clear();
	spline.addPoint(Ogre::Vector3(0, Global::camera->cam->getFOVy().valueRadians(), 0));
	spline.addPoint(Ogre::Vector3(peakTime, Ogre::Degree(peak).valueRadians(), 0));
	spline.addPoint(Ogre::Vector3(duration, Ogre::Degree(target).valueRadians(), 0));

	return true;
}

bool FovPeakEffect::start(float time, float peakTime, float peak, float peakTime2, float peak2, float target /*= 0*/)
{
	duration = time;
	target = target != 0 ? target : Global::camera->defaultFov.valueDegrees();
	timer = 0;

	spline.clear();
	spline.addPoint(Ogre::Vector3(0, Global::camera->cam->getFOVy().valueRadians(), 0));
	spline.addPoint(Ogre::Vector3(peakTime, Ogre::Degree(peak).valueRadians(), 0));
	spline.addPoint(Ogre::Vector3(peakTime2, Ogre::Degree(peak2).valueRadians(), 0));
	spline.addPoint(Ogre::Vector3(duration, Ogre::Degree(target).valueRadians(), 0));

	return true;
}

bool FovPeakEffect::update(Ogre::Real tslf)
{
	timer += tslf;

	if (timer > duration)
		timer = duration;

	float w = timer / duration;

	auto p = spline.interpolateX(w);
	Global::camera->cam->setFOVy(Ogre::Radian(p.y));

	return timer < duration;
}

Ogre::Vector3 ExtendedSpline::interpolateX(float t) const
{
	float end = mPoints.back().x;
	float currentD = end*t;

	unsigned int segIdx = 0;
	float segW = 0;

	for (size_t i = 1; i < mPoints.size(); i++)
	{
		auto& p = mPoints[i];
		if (p.x >= currentD)
		{
			segIdx = i - 1;
			float startPoint = mPoints[i - 1].x;
			float segLen = (p.x - startPoint);
			segW = (i == mPoints.size()) ? 1 : (currentD - startPoint) / segLen;
			break;
		}
	}

	return interpolate(segIdx, segW);
}
