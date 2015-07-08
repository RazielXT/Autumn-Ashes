#include "stdafx.h"
#include "PgDetailGeometry.h"
#include "GeometryManager.h"
#include "MUtils.h"

using namespace Ogre;

void PgDetailGeometry::addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryParams& params)
{
    init(params);

    float xStart = 0;
    float xEnd = gridInfo.size.x;
    float xStep = info.stepSize.x / params.density;

    float yStart = 0;
    float yEnd = gridInfo.size.y;
    float yStep = info.stepSize.y / params.density;

    static int sgCount = 0;

    parent = gridInfo.parent;

    trees = new Forests::PagedGeometry();
    trees->setCamera(Global::mSceneMgr->getCamera("Camera"));	//Set the camera so PagedGeometry knows how to calculate LODs
    trees->setPageSize(info.gridSize);	//Set the size of each page of geometry
    trees->setInfinite();		//Use infinite paging mode
    //trees->addDetailLevel<Forests::BatchPage>(25, 10, Ogre::Any(1));		//Use batches up to 150 units away, and fade for 30 more units
    //trees->addDetailLevel<Forests::BatchPage>(50, 20, Ogre::Any(2));		//Use batches up to 150 units away, and fade for 30 more units
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

            auto wmasked = (pos.color*params.weightMask);
            float w = wmasked.r + wmasked.g + wmasked.b + wmasked.a;

            float scaleMask = 1.0f;
            if (params.customEdit.customScaleEnabled)
            {
                auto smasked = (pos.color*params.customEdit.customScaleMask);
                float scaleW = wmasked.r + wmasked.g + wmasked.b + wmasked.a;
                scaleMask = MUtils::lerp(params.customEdit.customMinmaxScale.x, params.customEdit.customMinmaxScale.y, scaleW);
            }

            MUtils::RayInfo ray;
            bool foundRay = false;

            if (gridInfo.target)
                foundRay = MUtils::getRayFilteredInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray, gridInfo.target);
            else
                foundRay = MUtils::getRayInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray);

            if (foundRay && ray.normal.y >= info.maxSteepY && acceptsWeight(w))
            {
                float scale = scaleMask*Ogre::Math::RangeRandom(params.minmaxScale.x, params.minmaxScale.y);
                placeObject(ray.pos, MUtils::quaternionFromNormal(ray.normal), info.generalScale, params.color, treeLoader);

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

void PgDetailGeometry::init(DetailGeometryParams& params)
{
    info = DetailGeometryInfo::get(params.name);
}

void PgDetailGeometry::placeObject(Vector3 pos, Quaternion or, float scale, Vector3 color, Forests::TreeLoader3D *treeLoader)
{
    Quaternion randomYaw(Degree(Math::RangeRandom(0, 360)), Vector3(0, 1, 0));
    String meshName = info.possibleEntities[(int)Math::RangeRandom(0, info.possibleEntities.size() - 0.01f)];

    Entity *myTree = Global::mSceneMgr->createEntity(meshName);
    myTree->setMaterialName("grassMat");
    //myTree->setCastShadows(false);

    treeLoader->addTree(myTree, pos, or.getYaw(), scale);

    temps.push_back(myTree);
}
