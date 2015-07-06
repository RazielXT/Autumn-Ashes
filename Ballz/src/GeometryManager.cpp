#include "stdafx.h"
#include "GeometryManager.h"
#include "BasicDetailGeometry.h"
#include "PgDetailGeometry.h"
#include "GrassDetailGeometry.h"
#include "ManualDetailGeometry.h"

using namespace Ogre;

std::vector<std::string> DetailGeometryMaterial::darkenVCMeshes;
std::vector<std::string> DetailGeometryMaterial::darkenVCMeshesDone;

void GeometryManager::forgetPagedGeometry(Forests::PagedGeometry *g)
{
    auto it = std::find(pagedGeometries.begin(), pagedGeometries.end(), g);

    if (it != pagedGeometries.end())
        pagedGeometries.erase(it);
}

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

    DetailGeometryMaterial::reset();
}

void GeometryManager::postLoad()
{
    ManualDetailGeometry::finish();
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

DetailGeometry* GeometryManager::getInstance(std::string name)
{
    //if ()
    //   return new PgGeometryPreset();
    if (name == "Rocks" || name == "TreesAspen" || name == "Bush")
        return new BasicDetailGeometry();
    if (name == "Grass")
        return new GrassDetailGeometry();

    return nullptr;
}


void GeometryManager::resetMaskedDetailGeometries()
{
    for (auto g : detailGeometries)
    {
        g->clear();
        delete g;
    }

    detailGeometries.clear();
}

void GeometryManager::addDetailGeometryEntity(int id, Ogre::SceneNode* node, std::string type, bool keepMesh, Vector3 color)
{
    auto pg = ManualDetailGeometry::get(id);
    pg->addObject(node, type, keepMesh, color);
}

void GeometryManager::addDetailGeometryMask(Ogre::Entity* maskEnt, std::vector<DetailGeometryParams>& geometries, OgreNewt::Body* target, float rayDistance)
{
    MaskGrid posGrid;
    GeometryMaskInfo info;
    info.rayDistance = rayDistance;
    info.target = target;
    info.node = maskEnt->getParentSceneNode();
    info.parent = this;

    generateGeometryMask(maskEnt, posGrid, info.size);

    for (auto g : geometries)
    {
        auto p = getInstance(g.name);

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

bool GeometryManager::modifyVertexBuffer(Entity* ent, VCEditFunc editFunc, void* data)
{
    auto node = ent->getParentSceneNode();
    auto m = ent->getMesh().get()->getSubMesh(0);

    const Ogre::VertexElement* posElem = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
    const Ogre::VertexElement* posElemCol = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_DIFFUSE);
    const Ogre::VertexElement* posElemTC = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);

    if (posElem == nullptr)
        return false;

    Ogre::HardwareVertexBufferSharedPtr vbuf = m->vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
    unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_NORMAL));
    Ogre::Real* pReal;
    Ogre::Real* pTCReal = nullptr;
    Ogre::RGBA* pCReal = nullptr;

    for (size_t j = 0; j < m->vertexData->vertexCount; j ++)
    {
        posElem->baseVertexPointerToElement(vertex, &pReal);

        if (posElemCol)
            posElemCol->baseVertexPointerToElement(vertex, &pCReal);

        if (posElemTC)
            posElemTC->baseVertexPointerToElement(vertex, &pTCReal);

		editFunc(ent, pReal, pTCReal, pCReal, data);

        vertex += vbuf->getVertexSize();
    }

    vbuf->unlock();

    return true;
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

        if (m->vertexData->vertexCount == 6)
        {
            posGrid.push_back(firstRow);
        }

        posGrid.push_back(loadedRow);


        size.x = temp[2].pos.x - posGrid[0][0].pos.x;
        size.y = posGrid[0][0].pos.z - temp[2].pos.z;
    }

    vbuf->unlock();
}