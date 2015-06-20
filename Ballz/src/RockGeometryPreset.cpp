#include "stdafx.h"
#include "RockGeometryPreset.h"
#include "MathUtils.h"

using namespace Ogre;

void RockGeometryPreset::addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, GeometryPresetInfo& info)
{
    float xStart = 0;
    float xEnd = gridInfo.size.x;
    float xStep = 1;

    float yStart = 0;
    float yEnd = gridInfo.size.y;
    float yStep = 1;

    for (float x = xStart; x <= xEnd; x+=xStep)
        for (float y = yStart; y <= yEnd; y += yStep)
        {
            float xOffset = Ogre::Math::RangeRandom(-xStep, xStep) / 2.0f;
            float yOffset = Ogre::Math::RangeRandom(-yStep, yStep) / 2.0f;

            auto pos = getMaskAt(grid, gridInfo, x + xOffset, y + yOffset);

            MathUtils::RayInfo ray;
            if (pos.color.r > 0.25f && MathUtils::getRayInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray))
            {
                placeRock(ray.pos, ray.normal);
            }
        }
}

void RockGeometryPreset::placeRock(Vector3 pos, Quaternion or)
{
    auto node = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(pos, or);
    auto ent = Global::mSceneMgr->createEntity("basicSphere.mesh");
    node->attachObject(ent);
}
