#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"

struct DetailGeometryInfo
{
    float gridSize = 25;
    Ogre::Vector2 stepSize;
    float generalScale = 1;
    float maxSteepY;
    float maxDistance;

    std::vector<std::string> possibleEntities;

    static DetailGeometryInfo get(std::string name)
    {
        DetailGeometryInfo info;

        info.maxSteepY = 0;
        info.maxDistance = 50;

        if (name == "TreesAspen")
        {
            info.stepSize.x = 5;
            info.stepSize.y = 5;
            info.generalScale = 1.05f;
            info.maxDistance = 150;
            info.gridSize = 35;

            //possibleEntities.push_back("treeBunchBigLeafs.mesh;treeBunchBigTrunk.mesh");
            info.possibleEntities.push_back("treeBunchLeafs.mesh;treeBunchTrunk.mesh");
            //possibleEntities.push_back("aspenLeafs.mesh;aspenTrunk.mesh");
            //possibleEntities.push_back("aspen2Leafs.mesh;aspen2Trunk.mesh");
        }
        else if (name == "Bush")
        {
            info.stepSize.x = 1;
            info.stepSize.y = 1;
            info.generalScale = 5.25f;
            info.maxDistance = 100;
            info.gridSize = 35;

            info.possibleEntities.push_back("bush1.mesh");
        }
        else if (name == "Rocks")
        {
            info.stepSize.x = 1;
            info.stepSize.y = 1;

            info.possibleEntities.push_back("Rock1.mesh");
            info.possibleEntities.push_back("Rock2.mesh");
            info.possibleEntities.push_back("Rock3.mesh");
            info.possibleEntities.push_back("Rock4.mesh");
            info.possibleEntities.push_back("Rock5.mesh");
            info.possibleEntities.push_back("Rock6.mesh");
        }
        else if (name == "RocksPG")
        {
            info.stepSize.x = 3;
            info.stepSize.y = 3;
            info.generalScale = 0.05f;

            info.possibleEntities.push_back("fir06_30.mesh");
        }

        return info;
    }
};