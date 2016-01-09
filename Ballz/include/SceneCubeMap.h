#pragma once

#include "stdafx.h"
#include "OgreTexture.h"

#include "stdafx.h"

class SceneCubeMap
{
public:

    SceneCubeMap();
    ~SceneCubeMap();

    bool update();
    void init(std::string name, int size, bool editable, float minRenderDistance);

    Ogre::Vector3 position;
    float posessionRadius;
    float materialWPOffset;

    static void renderAll()
    {
        for (auto cubemap : cubeMaps)
        {
            cubemap.second->update();
        }

        //double reflection
        for (auto cubemap : cubeMaps)
        {
            cubemap.second->update();
        }
    }

    struct CubemapedMats
    {
        std::string matOriginalName;
        SceneCubeMap* cm;
        std::string matName;
    };

    static std::vector<CubemapedMats> appliedMaterials;

    static Ogre::MaterialPtr applyCubemap(Ogre::MaterialPtr mat, Ogre::Vector3 pos)
    {
        SceneCubeMap* r = nullptr;
        float closestToCenter;
        float closestToRadius;

        for (auto cubemap : cubeMaps)
        {
            auto cm = cubemap.second;

            float distToCenter = cm->position.distance(pos);
            float distToRadius = std::max(0.0f, distToCenter - cm->posessionRadius);

            if (!r || (distToRadius < closestToRadius || (distToRadius == 0 && distToCenter < closestToCenter)))
            {
                r = cm;
                closestToRadius = distToRadius;
                closestToCenter = distToCenter;
            }
        }

        if (!r)
            return mat;


        for (auto appliedMat : appliedMaterials)
        {
            if (appliedMat.matOriginalName == mat->getName() && appliedMat.cm == r)
                return Ogre::MaterialManager::getSingletonPtr()->getByName(appliedMat.matName);
        }

        //create new
        static int mid = 0;
        auto newMat = mat->clone(mat->getName() + std::to_string(mid++));

        auto pass = newMat->getTechnique(0)->getPass(1);
        Ogre::TextureUnitState* t = pass->getTextureUnitState("envCubeMap");
        if (r->detectedEdited)
            t->setCubicTextureName(r->getTextureNamePrefix() + ".png", true);
        else
            t->setTexture(r->texture);

        pass->getFragmentProgramParameters()->setNamedConstant("cubemapWPOffset", Ogre::Vector4(r->position.x, r->position.y, r->position.z, r->materialWPOffset));

        CubemapedMats matInfo;
        matInfo.matName = newMat->getName();
        matInfo.cm = r;
        matInfo.matOriginalName = mat->getName();
        appliedMaterials.push_back(matInfo);

        return newMat;
    }

    static void clearAll()
    {
        for (auto cubemap : cubeMaps)
            delete cubemap.second;

        cubeMaps.clear();
    }

    static std::map<std::string, SceneCubeMap*> cubeMaps;

private:

    std::string getTexturePath(bool edited = false);
    std::string getTextureNamePrefix();

    void loadGpuTexture();

    std::string name;

    Ogre::TexturePtr texture;

    Ogre::SceneNode* mReflectCamNode;
    Ogre::Camera* mReflectCam;

    Ogre::Entity* mEntity;
    int visibilityFlag;
    int size;
    bool editable;
    bool detectedEdited;
    float minRenderDistance;

    static int id;
};