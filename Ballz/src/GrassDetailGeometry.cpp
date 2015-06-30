#include "stdafx.h"
#include "GrassDetailGeometry.h"
#include "MUtils.h"
#include "GameStateManager.h"

using namespace Ogre;

void GrassDetailGeometry::addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryInfo& info)
{
    init(info);

    float xStart = 0;
    float xEnd = gridInfo.size.x;
    float xStep = stepSize.x / info.density;

    float yStart = 0;
    float yEnd = gridInfo.size.y;
    float yStep = stepSize.y / info.density;

    int bgc = 0;

    for (float x = xStart; x <= xEnd; x += xStep)
        for (float y = yStart; y <= yEnd; y += yStep)
        {
            float xOffset = stepDeviation*Ogre::Math::RangeRandom(-xStep, xStep) / 2.0f;
            float yOffset = stepDeviation*Ogre::Math::RangeRandom(-yStep, yStep) / 2.0f;
            auto pos = getMaskAt(grid, gridInfo, x + xOffset, y + yOffset);

            auto wmasked = (pos.color*info.weightMask);
            float w = wmasked.r + wmasked.g + wmasked.b + wmasked.a;

            if (acceptsWeight(w))
            {
                float scaleMask = 1.0f;
                if (info.customEdit.customScaleEnabled)
                {
                    auto smasked = (pos.color*info.customEdit.customScaleMask);
                    float scaleW = wmasked.r + wmasked.g + wmasked.b + wmasked.a;
                    scaleMask = MUtils::lerp(info.customEdit.customMinmaxScale.x, info.customEdit.customMinmaxScale.y, scaleW);
                }

                float scale = generalScale*scaleMask*Ogre::Math::RangeRandom(info.minmaxScale.x, info.minmaxScale.y);

                if (placeObject(gridInfo, pos.pos, scale, info.color))
                    bgc++;
            }
        }


    if (grassBuffer.size() > 0)
    {
        auto mesh = makeMesh();

        Entity *entity = Global::mSceneMgr->createEntity(mesh->getName());
        entity->setCastShadows(false);
        entity->setRenderingDistance(maxDistance);

        SceneNode *node = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(entity);

        entities.push_back(entity);
    }

}

void GrassDetailGeometry::clear()
{
    for (auto e : entities)
    {
        auto node = e->getParentSceneNode();
        node->detachAllObjects();

        Global::mSceneMgr->destroySceneNode(node);
        Global::mSceneMgr->destroyEntity(e);
    }
}

bool GrassDetailGeometry::acceptsWeight(float w) const
{
    return w >= Ogre::Math::RangeRandom(0, 1);
}

void GrassDetailGeometry::init(DetailGeometryInfo& info)
{
    grassBuffer.clear();

    minSteepY = 1.0f;
    maxSteepY = 0.25f;

    maxDistance = 50;

    //if (info.name == "TreesAspen")
    {
        stepSize.x = 0.5f;
        stepSize.y = 0.5f;
        generalScale = 1.0f;

        material = "branch_summerpng";
    }
}

bool GrassDetailGeometry::placeObject(GeometryMaskInfo& gridInfo, Vector3 pos, float scale, Vector3 color)
{
    MUtils::RayInfo ray;
    MUtils::RayInfo ray2;
    bool foundRay = false;

    Quaternion rayOr = gridInfo.node->getOrientation();
    Quaternion randomYaw(Degree(Math::RangeRandom(0, 360)), Vector3(0, 1, 0));
    Vector3 pos2 = pos + randomYaw*Vector3(scale, 0, 0);

    if (gridInfo.target)
    {
        foundRay = MUtils::getRayFilteredInfo(pos, rayOr*Vector3(0, -1, 0), gridInfo.rayDistance, ray, gridInfo.target);
        foundRay = foundRay && MUtils::getRayFilteredInfo(pos2, rayOr*Vector3(0, -1, 0), gridInfo.rayDistance, ray2, gridInfo.target);
    }
    else
    {
        foundRay = MUtils::getRayInfo(pos, rayOr*Vector3(0, -1, 0), gridInfo.rayDistance, ray);
        foundRay = foundRay && MUtils::getRayInfo(pos2, rayOr*Vector3(0, -1, 0), gridInfo.rayDistance, ray2);
    }

    if (foundRay && ray.normal.y >= maxSteepY && ray.normal.y <= minSteepY && ray2.normal.y >= maxSteepY && ray2.normal.y <= minSteepY)
    {
        GrassDefinition grass;
        grass.height = scale;
        grass.p1 = ray.pos;
        grass.p2 = ray2.pos;
        grass.vc = Ogre::ColourValue(color.x, color.y, color.z, 0);

        grassBuffer.push_back(grass);

        return true;
    }

    return false;
}

Mesh* GrassDetailGeometry::makeMesh()
{
    static int grassID = 0;
    //Calculate the number of quads to be added
    unsigned int quadCount = grassBuffer.size();

    // check for overflows of the uint16's
    unsigned int maxUInt16 = std::numeric_limits<uint16>::max();
    if (quadCount > maxUInt16)
    {
        LogManager::getSingleton().logMessage("grass count overflow: you tried to use more than " + StringConverter::toString(maxUInt16) + " (thats the maximum) grass meshes for one page");
        return 0;
    }

    //Create manual mesh to store grass quads
    MeshPtr mesh = MeshManager::getSingleton().createManual("genGrass" + std::to_string(++grassID), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    SubMesh *subMesh = mesh->createSubMesh();
    subMesh->useSharedVertices = false;

    //Setup vertex format information
    subMesh->vertexData = new VertexData;
    subMesh->vertexData->vertexStart = 0;
    subMesh->vertexData->vertexCount = 4 * quadCount;

    VertexDeclaration* dcl = subMesh->vertexData->vertexDeclaration;
    size_t offset = 0;
    dcl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
    offset += VertexElement::getTypeSize(VET_FLOAT3);
    dcl->addElement(0, offset, VET_COLOUR, VES_DIFFUSE);
    offset += VertexElement::getTypeSize(VET_COLOUR);
    dcl->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
    offset += VertexElement::getTypeSize(VET_FLOAT2);
    //dcl->addElement(0, offset, VET_FLOAT2, VES_NORMAL);
    //offset += VertexElement::getTypeSize(VET_FLOAT3);

    //Populate a new vertex buffer with grass
    HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton()
                                         .createVertexBuffer(offset, subMesh->vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
    float* pReal = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

    Ogre::Vector3 maxPos(Math::POS_INFINITY, Math::POS_INFINITY, Math::POS_INFINITY);
    Ogre::Vector3 minPos(Math::NEG_INFINITY, Math::NEG_INFINITY, Math::NEG_INFINITY);

    const GrassDefinition* posPtr = grassBuffer.data();	//Position array "iterator"

    for (uint16 i = 0; i < quadCount; ++i)
    {
        //Get the color at the grass position
        uint32 color = posPtr->vc.getAsARGB();

        //Add vertices
        *pReal++ = posPtr->p1.x;
        *pReal++ = posPtr->p1.y + posPtr->height;
        *pReal++ = posPtr->p1.z;   //pos

        *((uint32*)pReal++) = color;                 //color
        *pReal++ = 0.f;
        *pReal++ = 0.f;              //uv
        //*pReal++ = n1_x; *pReal++ = n1_y;  *pReal++ = n1_z;           //normal


        *pReal++ = posPtr->p2.x;
        *pReal++ = posPtr->p2.y + posPtr->height;
        *pReal++ = posPtr->p2.z;   //pos
        *((uint32*)pReal++) = color;                 //color
        *pReal++ = 1.f;
        *pReal++ = 0.f;              //uv
        //*pReal++ = n2_x; *pReal++ = n2_y;  *pReal++ = n2_z;           //normal

        *pReal++ = posPtr->p1.x;
        *pReal++ = posPtr->p1.y;
        *pReal++ = posPtr->p1.z;   //pos
        *((uint32*)pReal++) = color;                 //color
        *pReal++ = 0.f;
        *pReal++ = 1.f;              //uv
        //*pReal++ = n1_x; *pReal++ = n1_y;  *pReal++ = n1_z;           //normal

        *pReal++ = posPtr->p2.x;
        *pReal++ = posPtr->p2.y;
        *pReal++ = posPtr->p2.z;   //pos
        *((uint32*)pReal++) = color;                 //color
        *pReal++ = 1.f;
        *pReal++ = 1.f;              //uv
        //*pReal++ = n2_x; *pReal++ = n2_y;  *pReal++ = n2_z;           //normal

        //Update bounds
        maxPos = MUtils::getMaximum(maxPos, posPtr->p1);
        minPos = MUtils::getMinimum(minPos, posPtr->p1);

        posPtr++;
    }

    vbuf->unlock();
    subMesh->vertexData->vertexBufferBinding->setBinding(0, vbuf);

    //Populate index buffer
    subMesh->indexData->indexStart = 0;
    subMesh->indexData->indexCount = 6 * quadCount;
    subMesh->indexData->indexBuffer = HardwareBufferManager::getSingleton()
                                      .createIndexBuffer(HardwareIndexBuffer::IT_16BIT, subMesh->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    uint16* pI = static_cast<uint16*>(subMesh->indexData->indexBuffer->lock(HardwareBuffer::HBL_DISCARD));
    for (uint16 i = 0; i < quadCount; ++i)
    {
        uint16 offset = i * 4;

        *pI++ = 0 + offset;
        *pI++ = 2 + offset;
        *pI++ = 1 + offset;

        *pI++ = 1 + offset;
        *pI++ = 2 + offset;
        *pI++ = 3 + offset;
    }

    subMesh->indexData->indexBuffer->unlock();
    //subMesh->setBuildEdgesEnabled(autoEdgeBuildEnabled);

    //Finish up mesh
    AxisAlignedBox bounds(minPos, maxPos);
    mesh->_setBounds(bounds);
    Vector3 temp = bounds.getMaximum() - bounds.getMinimum();
    mesh->_setBoundingSphereRadius(temp.length() * 0.5f);

    LogManager::getSingleton().setLogDetail(static_cast<LoggingLevel>(0));
    mesh->setAutoBuildEdgeLists(false);
    mesh->load();
    LogManager::getSingleton().setLogDetail(LL_NORMAL);

    //Apply grass material to mesh
    subMesh->setMaterialName(material);

    //Return the mesh
    return mesh.getPointer();
}