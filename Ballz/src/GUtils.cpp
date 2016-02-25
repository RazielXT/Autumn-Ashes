#include "stdafx.h"
#include "GUtils.h"

using namespace Ogre;

namespace GUtils
{

std::vector<std::string> debug;

void DebugPrint(const std::string line, bool logFile)
{
	static int counter = 1;

	if (logFile)
		Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(line);

	if (debug.size() >= MAX_DEBUG_LINES)
		debug.erase(debug.begin());

	debug.push_back(std::to_string(counter++) + ": " + line);
}

OgreNewt::Body* createConvexBody(Entity* ent, float mass, float linearDamping, float angularDamping)
{
	OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(Global::mWorld, ent, 0));
	auto node = ent->getParentNode();

	auto body = new OgreNewt::Body(Global::mWorld, col);

	Ogre::Vector3 inertia, offset;
	col->calculateInertialMatrix(inertia, offset);

	body->setMassMatrix(mass, inertia);
	body->setCenterOfMass(offset);

	body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());

	body->attachNode(node);

	if (mass > 0)
	{
		body->setStandardForceCallback();
		body->setLinearDamping(linearDamping);
		body->setAngularDamping(Vector3(angularDamping));
	}

	return body;
}
}