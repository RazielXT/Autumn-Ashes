#include "stdafx.h"

class ZipLine : public EventTask, public PlayerListener
{
public:

	struct ZipLineState
	{
		Vector3 projPos;
		float fullL;
		float cProgress;
		float left;
	};

	ZipLine(Ogre::Vector3 startPoint, Ogre::Vector3 endPoint) : startD(startPoint), endD(endPoint) {};

	bool start();
	bool update(Ogre::Real tslf);
	void pressedKey(const OIS::KeyEvent &arg);

private:

	inline ZipLineState getProjectedState(Ogre::Vector3& point);
	inline Vector3 getLinePos(const ZipLineState& state);

	Ogre::Vector3 startD;
	Ogre::Vector3 endD;
};