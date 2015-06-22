#include "stdafx.h"
#include "GeometryManager.h"
#include "RockGeometryPreset.h"

using namespace Ogre;

void GeometryManager::addPagedGeometry(Forests::PagedGeometry *g)
{
    pagedGeometries.push_back(g);
}

void GeometryManager::clear()
{
    auto it = pagedGeometries.begin();

    while(it != pagedGeometries.end())
    {
        delete (*it)->getPageLoader();
        delete (*it);
        it++;
    }

    pagedGeometries.clear();

    for (auto g : detailGeometries)
    {
        delete g;
    }

    detailGeometries.clear();
}

void GeometryManager::update()
{
    auto it = pagedGeometries.begin();

    while(it != pagedGeometries.end())
    {
        (*it)->update();
        it++;
    }
}

GeometryPreset* GeometryManager::getPreset(std::string name)
{
    if (name == "Rocks")
        return new RockGeometryPreset();

    return nullptr;
}


void GeometryManager::resetDetailGeometries()
{
    for (auto g : detailGeometries)
    {
        g->clear();
        delete g;
    }

    detailGeometries.clear();
}

void GeometryManager::addDetailGeometry(Ogre::Entity* maskEnt, std::vector<GeometryPresetInfo>& geometries, OgreNewt::Body* targetarget, float rayDistance)
{
    MaskGrid posGrid;
    GeometryMaskInfo info;
    info.rayDistance = rayDistance;
    info.target = targetarget;
    info.node = maskEnt->getParentSceneNode();

    generateGeometryMask(maskEnt, posGrid, info.size);

    for (auto g : geometries)
    {
        auto p = getPreset(g.name);

        if (p)
        {
            detailGeometries.push_back(p);

            p->addGeometry(posGrid, info, g);
        }
    }


}

void loadPoint(float* pReal, Ogre::RGBA pCReal, GeometryMaskPoint& out, SceneNode* node)
{
    out.pos.x = (*pReal++);
    out.pos.y = (*pReal++);
    out.pos.z = (*pReal++);

    out.pos *= node->getScale();
    // out[i] = node->getOrientation()*out[i];
    // out.pos += node->getPosition();

    out.color.setAsARGB(pCReal);
}

void GeometryManager::generateGeometryMask(Ogre::Entity* maskEnt, MaskGrid& posGrid, Vector2& size)
{
    auto node = maskEnt->getParentSceneNode();
    auto m = maskEnt->getMesh().get()->getSubMesh(0);

    const Ogre::VertexElement* posElem = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
    const Ogre::VertexElement* posElemCol = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_DIFFUSE);
    Ogre::HardwareVertexBufferSharedPtr vbuf = m->vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
    unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
    Ogre::Real* pReal;
    Ogre::RGBA* pCReal;

    std::vector<GeometryMaskPoint> loadedRow;
    GeometryMaskPoint temp[4];

    bool isFirstRow = true;
    bool firstCol = true;
    std::vector<GeometryMaskPoint> firstRow;
    float lastRowStart = 0;

    if ((m->vertexData->vertexCount % 6) == 0 && posElemCol != NULL)
    {
        for (size_t j = 0; j < m->vertexData->vertexCount; j += 6)
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            posElemCol->baseVertexPointerToElement(vertex, &pCReal);
            loadPoint(pReal, *pCReal, temp[0], node);
            vertex += vbuf->getVertexSize();

            posElem->baseVertexPointerToElement(vertex, &pReal);
            posElemCol->baseVertexPointerToElement(vertex, &pCReal);
            loadPoint(pReal, *pCReal, temp[1], node);
            vertex += vbuf->getVertexSize();

            posElem->baseVertexPointerToElement(vertex, &pReal);
            posElemCol->baseVertexPointerToElement(vertex, &pCReal);
            loadPoint(pReal, *pCReal, temp[2], node);
            vertex += vbuf->getVertexSize() * 2;

            posElem->baseVertexPointerToElement(vertex, &pReal);
            posElemCol->baseVertexPointerToElement(vertex, &pCReal);
            loadPoint(pReal, *pCReal, temp[3], node);
            vertex += vbuf->getVertexSize() * 2;

            //next row, unless starting
            if (!(isFirstRow && firstCol) && lastRowStart >= temp[0].pos.x)
            {
                if (isFirstRow)
                    posGrid.push_back(firstRow);

                posGrid.push_back(loadedRow);

                firstCol = true;
                isFirstRow = false;

                loadedRow.clear();
            }

            if (isFirstRow)
            {
                if (firstCol)
                    firstRow.push_back(temp[0]);

                firstRow.push_back(temp[1]);
            }

            if (firstCol)
                loadedRow.push_back(temp[3]);
            loadedRow.push_back(temp[2]);

            lastRowStart = temp[0].pos.x;
            firstCol = false;
        }

        posGrid.push_back(loadedRow);

        size.x = temp[2].pos.x - posGrid[0][0].pos.x;
        size.y = posGrid[0][0].pos.z - temp[2].pos.z;
    }

    vbuf->unlock();
}