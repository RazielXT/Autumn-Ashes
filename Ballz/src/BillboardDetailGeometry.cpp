#include "stdafx.h"
#include "BillboardDetailGeometry.h"
#include "MUtils.h"
#include "SUtils.h"
#include "GUtils.h"
#include "GameStateManager.h"

using namespace Ogre;


void BillboardDetailGeometry::addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryParams& params)
{
    init(params);

    float xStart = 0;
    float xEnd = gridInfo.size.x;
    float xStep = info.stepSize.x / params.density;

    float yStart = 0;
    float yEnd = gridInfo.size.y;
    float yStep = info.stepSize.y / params.density;

    int bgc = 0;

    for (float x = xStart; x <= xEnd; x += xStep)
        for (float y = yStart; y <= yEnd; y += yStep)
        {
            float xOffset = Ogre::Math::RangeRandom(-xStep, xStep) / 2.0f;
            float yOffset = Ogre::Math::RangeRandom(-yStep, yStep) / 2.0f;
            auto pos = getMaskAt(grid, gridInfo, x + xOffset, y + yOffset);

            auto wmasked = (pos.color*params.weightMask);
            float w = wmasked.r + wmasked.g + wmasked.b + wmasked.a;

            float scaleMask = 1.0f;
            if (params.customEdit.customScaleEnabled)
            {
                auto smasked = (pos.color*params.customEdit.customScaleMask);
                float scaleW = wmasked.r + wmasked.g + wmasked.b + wmasked.a;
                scaleMask = MUtils::lerp(params.customEdit.customMinmaxScale.x, params.customEdit.customMinmaxScale.y, scaleW);
            }

            GUtils::RayInfo ray;
            bool foundRay = false;

            if (gridInfo.target)
                foundRay = GUtils::getRayFilteredInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray, gridInfo.target);
            else
                foundRay = GUtils::getRayInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray);

            if (foundRay && ray.normal.y >= info.maxSteepY && acceptsWeight(w))
            {
                float scale = info.generalScale*scaleMask*Ogre::Math::RangeRandom(params.minmaxScale.x, params.minmaxScale.y);
                placeObject(ray.pos, MUtils::quaternionFromNormal(ray.normal), scale, params.color);

                bgc++;
            }
        }
}

void BillboardDetailGeometry::clear()
{
}

bool BillboardDetailGeometry::acceptsWeight(float w) const
{
    return w >= Ogre::Math::RangeRandom(0, 1);
}

void BillboardDetailGeometry::init(DetailGeometryParams& param)
{
    info = DetailGeometryInfo::get(param.name);
}

float sizeY = 8;

void BillboardDetailGeometry::placeObject(Vector3 pos, Quaternion or, float scale, Vector3 color)
{
    Quaternion randomYaw(Degree(Math::RangeRandom(0, 360)), Vector3(0, 1, 0));
    String meshName = info.possibleEntities[(int)Math::RangeRandom(0, info.possibleEntities.size() - 0.01f)];

    while (!meshName.empty())
    {
        auto name = SUtils::strtok_str(meshName, ';');

        auto set = getSet(name);
        pos.y += sizeY*0.5f;
        auto bb = set->createBillboard(pos);
    }
}

BillboardSet* BillboardDetailGeometry::getSet(std::string name)
{
    if (mySets.find(name) == mySets.end())
    {
        static int billboardSet = 0;
        auto mySet = Global::mSceneMgr->createBillboardSet("genBBS" + Ogre::StringConverter::toString(++billboardSet));
        mySet->setMaterialName("billboardTest");
        mySet->setBillboardType(Ogre::BillboardType::BBT_POINT);
        //mySet->setCommonDirection(Ogre::Vector3(0, 1, 0));
        mySet->setDefaultDimensions(sizeY*0.7f, sizeY);
        mySet->setTextureStacksAndSlices(1, 1);
        Global::mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mySet);

        mySets[name] = mySet;
    }

    return mySets[name];
}
