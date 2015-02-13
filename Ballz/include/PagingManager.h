#ifndef PAGINGMGR_H
#define PAGINGMGR_H

#include "stdafx.h"
#include <vector>

class PagingManager
{
public:

    void addPagedGeometry(Forests::PagedGeometry *g);
    void clear();
    void update();

private:

    std::list<Forests::PagedGeometry *> pagedGeometries;
};

#endif
