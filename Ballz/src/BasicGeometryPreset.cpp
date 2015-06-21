#include "stdafx.h"
#include "BasicGeometryPreset.h"
#include "MathUtils.h"

using namespace Ogre;

int GeometryPreset::matID = 0;

void BasicGeometryPreset::addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, GeometryPresetInfo& info)
{
    init(info);

    float xStart = 0;
    float xEnd = gridInfo.size.x;
    float xStep = stepSize.x / info.density;

    float yStart = 0;
    float yEnd = gridInfo.size.y;
    float yStep = stepSize.y / info.density;

    for (float x = xStart; x <= xEnd; x+=xStep)
        for (float y = yStart; y <= yEnd; y += yStep)
        {
            float xOffset = Ogre::Math::RangeRandom(-xStep, xStep) / 2.0f;
            float yOffset = Ogre::Math::RangeRandom(-yStep, yStep) / 2.0f;
            auto pos = getMaskAt(grid, gridInfo, x + xOffset, y + yOffset);

            auto wmasked = (pos.color*info.weightMask);
            float w = wmasked.r + wmasked.g + wmasked.b + wmasked.a;

            MathUtils::RayInfo ray;
            if (ray.normal.y >= maxSteepY && acceptsWeight(w) && MathUtils::getRayInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray))
            {
                float scale = Ogre::Math::RangeRandom(info.minmaxScale.x, info.minmaxScale.y);
                placeObject(ray.pos, MathUtils::quaternionFromNormal(ray.normal), scale, info.color);
            }
        }
}

bool BasicGeometryPreset::acceptsWeight(float w) const
{
    return w >= Ogre::Math::RangeRandom(0, 1);
}

void BasicGeometryPreset::init(GeometryPresetInfo& info)
{
    stepSize.x = 1;
    stepSize.y = 1;

    maxSteepY = 0;
}

void BasicGeometryPreset::updateMaterial(Ogre::Entity* ent, Ogre::Vector3& color)
{
    if (color.x != 1 || color.y != 1 || color.z != 1)
    {
        auto name = ent->getSubEntity(0)->getMaterialName();

        if (materials.find(name) == materials.end())
        {
            auto mat = (Ogre::Material*)MaterialManager::getSingleton().getByName(name).get();
            auto newMat = mat->clone(mat->getName() + "Gen" + std::to_string(++matID)).get();
            newMat->setDiffuse(ColourValue(color.x, color.y, color.z, 1));

            materials[name] = newMat;
        }

        auto mat = materials[name];
        ent->getSubEntity(0)->setMaterialName(mat->getName());
    }
}
