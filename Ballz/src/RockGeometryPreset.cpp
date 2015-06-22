#include "stdafx.h"
#include "RockGeometryPreset.h"
#include "MathUtils.h"
#include <string>

using namespace Ogre;

void RockGeometryPreset::placeObject(Vector3 pos, Quaternion or, float scale, Vector3 color)
{
    Quaternion randomYaw(Degree(Math::RangeRandom(0, 360)), Vector3(0, 1, 0));
    String meshName = "Rock" + std::to_string((int)Math::RangeRandom(1, 6.9f)) + ".mesh";
    auto node = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(pos, randomYaw*or);
    auto ent = Global::mSceneMgr->createEntity(meshName);
    node->attachObject(ent);
    node->setScale(Vector3(scale, scale, scale));

    updateMaterial(ent, color);

    /*OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(Global::mWorld, ent, 0));
    auto body = new OgreNewt::Body(Global::mWorld, col);
    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);
    auto userD = new bodyUserData();
    body->setUserData(Ogre::Any(userD));
    body->setMassMatrix(0, inertia);
    body->setCenterOfMass(offset);
    body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());*/

    entities.push_back(ent);
}

bool RockGeometryPreset::acceptsWeight(float w) const
{
    return w >= Ogre::Math::RangeRandom(0,1);
}

void RockGeometryPreset::init(GeometryPresetInfo& info)
{
    stepSize.x = 1;
    stepSize.y = 1;

    maxSteepY = 0;
}
