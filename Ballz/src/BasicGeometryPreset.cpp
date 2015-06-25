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

    static int sgCount = 0;
    float staticEntitiesGridSize = 50;
    sg = Global::mSceneMgr->createStaticGeometry("basicDG" + std::to_string(sgCount++));
    sg->setRegionDimensions(Ogre::Vector3(staticEntitiesGridSize, staticEntitiesGridSize, staticEntitiesGridSize));
    sg->setOrigin(Ogre::Vector3(0, 0, 0));
    sg->setCastShadows(true);

    for (float x = xStart; x <= xEnd; x+=xStep)
        for (float y = yStart; y <= yEnd; y += yStep)
        {
            float xOffset = Ogre::Math::RangeRandom(-xStep, xStep) / 2.0f;
            float yOffset = Ogre::Math::RangeRandom(-yStep, yStep) / 2.0f;
            auto pos = getMaskAt(grid, gridInfo, x + xOffset, y + yOffset);

            auto wmasked = (pos.color*info.weightMask);
            float w = wmasked.r + wmasked.g + wmasked.b + wmasked.a;

            float scaleMask = 1.0f;
            if (info.customEdit.customScaleEnabled)
            {
                auto smasked = (pos.color*info.customEdit.customScaleMask);
                float scaleW = wmasked.r + wmasked.g + wmasked.b + wmasked.a;
                scaleMask = MathUtils::lerp(info.customEdit.customMinmaxScale.x, info.customEdit.customMinmaxScale.y, scaleW);
            }

            MathUtils::RayInfo ray;
            bool foundRay = false;

            if (gridInfo.target)
                foundRay = MathUtils::getRayFilteredInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray, gridInfo.target);
            else
                foundRay = MathUtils::getRayInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray);

            if (foundRay && ray.normal.y >= maxSteepY && acceptsWeight(w))
            {
                float scale = scaleMask*Ogre::Math::RangeRandom(info.minmaxScale.x, info.minmaxScale.y);
                placeObject(ray.pos, MathUtils::quaternionFromNormal(ray.normal), scale, info.color);
            }
        }

    sg->build();
}

void BasicGeometryPreset::clear()
{
    /*for (auto e : entities)
    {
        auto node = e->getParentSceneNode();
        node->detachAllObjects();

        Global::mSceneMgr->destroySceneNode(node);
        Global::mSceneMgr->destroyEntity(e);
    }*/

    Global::mSceneMgr->destroyStaticGeometry(sg);
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

    if (info.name == "Rocks")
    {
        possibleEntities.push_back("Rock1.mesh");
        possibleEntities.push_back("Rock2.mesh");
        possibleEntities.push_back("Rock3.mesh");
        possibleEntities.push_back("Rock4.mesh");
        possibleEntities.push_back("Rock5.mesh");
        possibleEntities.push_back("Rock6.mesh");
    }
}

void BasicGeometryPreset::placeObject(Vector3 pos, Quaternion or, float scale, Vector3 color)
{
    Quaternion randomYaw(Degree(Math::RangeRandom(0, 360)), Vector3(0, 1, 0));
    String meshName = possibleEntities[(int)Math::RangeRandom(0, possibleEntities.size()-0.01f)];
    //auto node = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(pos, randomYaw*or);
    auto ent = Global::mSceneMgr->createEntity(meshName);
    //node->attachObject(ent);
    //node->setScale(Vector3(scale, scale, scale));

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

    sg->addEntity(ent, pos, or, Vector3(scale));
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
