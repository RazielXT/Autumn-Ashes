#include "stdafx.h"
#include "BasicGeometryPreset.h"
#include "MUtils.h"
#include "GameStateManager.h"

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

    sg = Global::mSceneMgr->createStaticGeometry("basicDG" + std::to_string(sgCount++));
    sg->setCastShadows(true);

    float staticEntitiesGridSize = 25;
    Ogre::Vector3 gridRegion(staticEntitiesGridSize, staticEntitiesGridSize, staticEntitiesGridSize);
    sg->setRegionDimensions(gridRegion);
    sg->setOrigin(gridRegion / 2.0f + gridInfo.node->getPosition());
	sg->setRenderingDistance(maxDistance);
    int bgc = 0;

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
                scaleMask = MUtils::lerp(info.customEdit.customMinmaxScale.x, info.customEdit.customMinmaxScale.y, scaleW);
            }

            MUtils::RayInfo ray;
            bool foundRay = false;

            if (gridInfo.target)
                foundRay = MUtils::getRayFilteredInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray, gridInfo.target);
            else
                foundRay = MUtils::getRayInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray);

            if (foundRay && ray.normal.y >= maxSteepY && acceptsWeight(w))
            {
                float scale = generalScale*scaleMask*Ogre::Math::RangeRandom(info.minmaxScale.x, info.minmaxScale.y);
                placeObject(ray.pos, MUtils::quaternionFromNormal(ray.normal), scale, info.color);


                bgc++;
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

    darkenVCMeshesDone.clear();
    Global::mSceneMgr->destroyStaticGeometry(sg);
}

bool BasicGeometryPreset::acceptsWeight(float w) const
{
    return w >= Ogre::Math::RangeRandom(0, 1);
}

void BasicGeometryPreset::init(GeometryPresetInfo& info)
{
    maxSteepY = 0;
	maxDistance = 50;

    if (darkenVCMeshes.empty())
    {
        darkenVCMeshes.push_back("aspenLeafs.mesh");
        darkenVCMeshes.push_back("aspen2Leafs.mesh");
        darkenVCMeshes.push_back("bush1.mesh");
    }

    if (info.name == "TreesAspen")
    {
        stepSize.x = 5;
        stepSize.y = 5;
        generalScale = 0.25f;
		maxDistance = 150;

        possibleEntities.push_back("aspenLeafs.mesh;aspenTrunk.mesh");
        possibleEntities.push_back("aspen2Leafs.mesh;aspen2Trunk.mesh");
    }
    else if (info.name == "Bush")
    {
        stepSize.x = 1;
        stepSize.y = 1;
        generalScale = 5.25f;

        possibleEntities.push_back("bush1.mesh");
    }
    else //if (info.name == "Rocks")
    {
        stepSize.x = 1;
        stepSize.y = 1;

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
    //node->setScale(Vector3(scale, scale, scale));

    while (!meshName.empty())
    {
        auto name = MUtils::strtok_str(meshName, ';');
        auto ent = Global::mSceneMgr->createEntity(name);
        //node->attachObject(ent);

        updateMaterial(ent, color);
        sg->addEntity(ent, pos, randomYaw, Vector3(scale));
    }

    /*OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(Global::mWorld, ent, 0));
    auto body = new OgreNewt::Body(Global::mWorld, col);
    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);
    auto userD = new bodyUserData();
    body->setUserData(Ogre::Any(userD));
    body->setMassMatrix(0, inertia);
    body->setCenterOfMass(offset);
    body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());*/
}

std::vector<std::string> BasicGeometryPreset::darkenVCMeshes;
std::vector<std::string> BasicGeometryPreset::darkenVCMeshesDone;

void BasicGeometryPreset::updateMaterial(Ogre::Entity* ent, Ogre::Vector3& color)
{
    if (std::find(darkenVCMeshes.begin(), darkenVCMeshes.end(), ent->getMesh()->getName()) != darkenVCMeshes.end())
    {
        if (std::find(darkenVCMeshesDone.begin(), darkenVCMeshesDone.end(), ent->getMesh()->getName()) == darkenVCMeshesDone.end())
        {
            auto darken = [](Entity* e, float* pos, float*, Ogre::RGBA* color)
            {
                if (!color)
                    return;

                float size = e->getBoundingBox().getHalfSize().x;
                Vector3 vpos(pos[0], 0, pos[2]);
                *color = Ogre::ColourValue(0, 0, 0, 0.25f + 0.75f*std::max<float>(0.0f, vpos.length() / size)).getAsARGB();
            };

            Global::gameMgr->geometryMgr->modifyVertexBuffer(ent, darken);

            darkenVCMeshesDone.push_back(ent->getMesh()->getName());
        }

    }


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
