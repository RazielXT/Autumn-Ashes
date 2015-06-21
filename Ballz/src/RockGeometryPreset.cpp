#include "stdafx.h"
#include "RockGeometryPreset.h"
#include "MathUtils.h"

using namespace Ogre;

void RockGeometryPreset::placeObject(Vector3 pos, Quaternion or, float scale, Vector3 color)
{
    auto node = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(pos, or);
    auto ent = Global::mSceneMgr->createEntity("basicPyramid.mesh");
    node->attachObject(ent);
    node->setScale(Vector3(scale, scale, scale));

    updateMaterial(ent, color);
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
