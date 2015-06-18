#include "stdafx.h"
#include "DynamicRope.h"

using namespace Ogre;

DynamicRope::DynamicRope(Ogre::Vector3 pos, int size, int parts, bool climbable)
{
	Ogre::String material = "";
	float partSize = size / (float)parts;

	for (int i = 0; i < parts; i++)
	{

		Entity* part = Global::mSceneMgr->createEntity("bridgePlank.mesh");
		part->setMaterialName(material);
		SceneNode* bNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
		bNode->attachObject(part);

		auto col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(Global::mWorld, part, 10));

		auto body = new OgreNewt::Body(Global::mWorld, col);

		Ogre::Vector3 inertia, offset;
		col->calculateInertialMatrix(inertia, offset);

		if (i == 0)
			body->setMassMatrix(0, inertia);
		else
			body->setMassMatrix(0.1, inertia);

		body->setCenterOfMass(offset);
		body->setPositionOrientation(position + float(i + 1)*flatDir, planksOr); //Vector3(0,0,scale*(1.8f+i*1.8f))
		body->attachNode(bNode);

		//bridgeB->setCustomForceAndTorqueCallback<BridgeMaker>(&BridgeMaker::move_callback, this);
		
		body->setAngularDamping(Vector3(1, 1, 1)*0.6);
		body->setLinearDamping(0.6);
		body->setType(PlayerIgnore);
		bodyUserData* userD = new bodyUserData();
		userD->customData = this;
		body->setUserData(Ogre::Any(userD));

		OgreNewt::BallAndSocket* b = new OgreNewt::BallAndSocket(parent, bridgeB, position + flatDir / 2.0f + (float)i*flatDir, pinAngle);//+Vector3(0,0,scale*(0.9f+i*1.8f))
		b->setCollisionState(0);


		parent = bridgeB;

		if (i == parts - 1)
			parent->setPositionOrientation(target, planksOr);

	}
}