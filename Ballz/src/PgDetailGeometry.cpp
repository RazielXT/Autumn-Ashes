#include "stdafx.h"
#include "PgDetailGeometry.h"
#include "GeometryManager.h"
#include "MUtils.h"

using namespace Ogre;

void PgDetailGeometry::addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryInfo& info)
{
    init(info);

    float xStart = 0;
    float xEnd = gridInfo.size.x;
    float xStep = stepSize.x / info.density;

    float yStart = 0;
    float yEnd = gridInfo.size.y;
    float yStep = stepSize.y / info.density;

    static int sgCount = 0;

    parent = gridInfo.parent;

    float staticEntitiesGridSize = 200;
    trees = new Forests::PagedGeometry();
    trees->setCamera(Global::mSceneMgr->getCamera("Camera"));	//Set the camera so PagedGeometry knows how to calculate LODs
    trees->setPageSize(staticEntitiesGridSize);	//Set the size of each page of geometry
    trees->setInfinite();		//Use infinite paging mode
    trees->addDetailLevel<Forests::BatchPage>(25, 0, Ogre::Any(0));		//Use batches up to 150 units away, and fade for 30 more units
    trees->addDetailLevel<Forests::BatchPage>(50, 20, Ogre::Any(2));		//Use batches up to 150 units away, and fade for 30 more units
    trees->addDetailLevel<Forests::ImpostorPage>(500, 50);	//Use impostors up to 400 units, and for for 50 more units

    Forests::TreeLoader3D *treeLoader = new Forests::TreeLoader3D(trees, Forests::TBounds(-1500, -1500, 1500, 1500));
    trees->setPageLoader(treeLoader);	//Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance
    static int pgc = 0;

    for (float x = xStart; x <= xEnd; x += xStep)
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
                float scale = scaleMask*Ogre::Math::RangeRandom(info.minmaxScale.x, info.minmaxScale.y);
                placeObject(ray.pos, MUtils::quaternionFromNormal(ray.normal), scale, info.color, treeLoader);


                pgc++;
            }
        }

    parent->addPagedGeometry(trees);
}

void PgDetailGeometry::clear()
{
    parent->forgetPagedGeometry(trees);

    delete trees->getPageLoader();
    delete trees;
}

bool PgDetailGeometry::acceptsWeight(float w) const
{
    return w >= Ogre::Math::RangeRandom(0, 1);
}

void PgDetailGeometry::init(DetailGeometryInfo& info)
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

void PgDetailGeometry::placeObject(Vector3 pos, Quaternion or, float scale, Vector3 color, Forests::TreeLoader3D *treeLoader)
{
    Quaternion randomYaw(Degree(Math::RangeRandom(0, 360)), Vector3(0, 1, 0));
    String meshName = possibleEntities[(int)Math::RangeRandom(0, possibleEntities.size() - 0.01f)];

    Entity *myTree = Global::mSceneMgr->createEntity("Rock1.mesh");
    myTree->setMaterialName("Examples/Rockwall");
    //myTree->setCastShadows(false);

    treeLoader->addTree(myTree, pos, or.getYaw(), scale);
}

void PgDetailGeometry::updateMaterial(Ogre::Entity* ent, Ogre::Vector3& color)
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
