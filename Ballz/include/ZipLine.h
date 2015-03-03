#include "stdafx.h"

class ZipLine : public EventTask
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

private:

	inline ZipLineState getProj(Ogre::Vector3 point);
	inline Vector3 getPPos(ZipLineState state);

	Ogre::Vector3 startD;
	Ogre::Vector3 endD;
};