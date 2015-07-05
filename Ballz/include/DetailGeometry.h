#pragma once

#include "stdafx.h"

struct GeometryMaskPoint
{
    Ogre::Vector3 pos;
    Ogre::ColourValue color;
};

using MaskGrid = std::vector < std::vector<GeometryMaskPoint> > ;

struct GeometryMaskInfo
{
    GeometryManager* parent;
    Ogre::Vector2 size;
    Ogre::SceneNode* node;
    OgreNewt::Body* target;
    float rayDistance;
};

enum VCEditMode { Off, SetVC, MulVC };

struct DetailGeometryCustomEdit
{
    VCEditMode customVCMode = Off;
    Ogre::ColourValue customVCMask;
    Ogre::Vector3 customVCColor;

    bool customScaleEnabled = false;
    Ogre::ColourValue customScaleMask;
    Ogre::Vector2 customMinmaxScale;
};

struct DetailGeometryParams
{
    std::string name;
    Ogre::ColourValue weightMask;
    Ogre::Vector3 color;
    Ogre::Vector2 minmaxScale;
    float density;

    DetailGeometryCustomEdit customEdit;
};

class DetailGeometry
{
public:

    virtual ~DetailGeometry() {}

    virtual void addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryParams& info) = 0;
    virtual void clear() = 0;

protected:

    static int matID;

    GeometryMaskPoint getMaskAt(MaskGrid& grid, GeometryMaskInfo& gridInfo, float x, float y)
    {
        x = Ogre::Math::Clamp(x, 0.0f, gridInfo.size.x);
        y = Ogre::Math::Clamp(y, 0.0f, gridInfo.size.y);

        x += grid[0][0].pos.x;
        y = -y - grid[grid.size()-1][0].pos.z;

        GeometryMaskPoint out;
        int rowStart = 0;
        int colStart = 0;

        for (auto& c : grid[0])
        {
            if (c.pos.x > x || colStart + 1 == grid[0].size())
            {
                colStart--;
                break;
            }

            colStart++;
        }

        for (auto& r : grid)
        {
            if (r[0].pos.z < y || rowStart + 1 == grid.size())
            {
                rowStart--;
                break;
            }

            rowStart++;
        }

        rowStart = std::max(rowStart, 0);
        colStart = std::max(colStart, 0);

        float rowDifSize = grid[rowStart][colStart].pos.z - grid[rowStart + 1][colStart].pos.z;
        float colDifSize = grid[rowStart][colStart + 1].pos.x - grid[rowStart][colStart].pos.x;

        out.pos = Ogre::Vector3(x, 0, y);
        out.pos = gridInfo.node->getOrientation()*out.pos;
        out.pos += gridInfo.node->getPosition();

        Ogre::ColourValue p1 = grid[rowStart][colStart].color;
        Ogre::ColourValue p2 = grid[rowStart+1][colStart].color;
        Ogre::ColourValue p3 = grid[rowStart][colStart+1].color;
        Ogre::ColourValue p4 = grid[rowStart+1][colStart+1].color;

        // Calculate the weights for each pixel
        float fy = (x - grid[rowStart][colStart].pos.x) / rowDifSize;
        float fx = (grid[rowStart][colStart].pos.z - y) / colDifSize;
        float fx1 = 1.0f - fx;
        float fy1 = 1.0f - fy;

        float w1 = fx1 * fy1;
        float w2 = fx  * fy1;
        float w3 = fx1 * fy;
        float w4 = fx  * fy;

        // Calculate the weighted sum of pixels (for each color channel)
        float outr = p1.r * w1 + p2.r * w2 + p3.r * w3 + p4.r * w4;
        float outg = p1.g * w1 + p2.g * w2 + p3.g * w3 + p4.g * w4;
        float outb = p1.b * w1 + p2.b * w2 + p3.b * w3 + p4.b * w4;
        float outa = p1.a * w1 + p2.a * w2 + p3.a * w3 + p4.a * w4;

        out.color = Ogre::ColourValue(outr, outg, outb, outa);

        return out;
    }
};