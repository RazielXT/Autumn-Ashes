#include "stdafx.h"
#include "PagingManager.h"

void PagingManager::addPagedGeometry(Forests::PagedGeometry *g)
{
    pagedGeometries.push_back(g);
}

void PagingManager::clear()
{
    auto it = pagedGeometries.begin();

    while(it != pagedGeometries.end())
    {
        delete (*it)->getPageLoader();
        delete (*it);
        it++;
    }

    pagedGeometries.clear();
}

void PagingManager::update()
{
	auto it = pagedGeometries.begin();

    while(it != pagedGeometries.end())
    {
        (*it)->update();
        it++;
    }
}