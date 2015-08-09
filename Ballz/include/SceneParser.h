#pragma once

#include "stdafx.h"
#include <tinyxml2.h>
#include "GrassHeightFunction.h"
#include "BridgeMaker.h"
#include "Tasks.h"
#include "Reflection.h"
#include "Slide.h"
#include "TopSlide.h"
#include "ZipLineSlide.h"
#include "player.h"
#include "Crows.h"
#include "MUtils.h"
#include "WaterCurrent.h"
#include "Player.h"
#include "PlayerSliding.h"
#include "SceneCubeMap.h"

using namespace Ogre;
using namespace tinyxml2;

class SceneParser
{
public:
    static SceneParser instance;

private:

    struct TriggerAndIDs
    {
        TriggerInfo* trigger;
        std::vector<int> ids;
    };

    struct EventTaskAndID
    {
        EventTask* task;
        int id;
    };

    struct LoadedJointInfo
    {
        Ogre::String body0, body1;
        Ogre::Vector3 position;
        char type;
        Ogre::Vector3 pin;
    };

    struct CompoundBodyInfo
    {
        Ogre::Entity* ent;
        Ogre::SceneNode* node;
        bool visual;
        bool physical;
    };

    struct LoadedInstanceForests
    {
        int gm;
        int im;
        int tr;
        Forests::PagedGeometry * g;
    };

    struct LoadedFilteredGrassArea
    {
        Ogre::String targetBody;
        TerrainHeightQueryData* data;
    };

    std::vector<LoadedFilteredGrassArea> loadedGrassAreas;
    std::vector<TriggerAndIDs> loadedTriggers;
    std::vector<EventTaskAndID> loadedTasks;
    std::vector<LoadedJointInfo> loadedJoints;
    std::map<Ogre::String, OgreNewt::Body*> loadedBodies;
    std::map<Ogre::String, Slide*> loadedSlides;
    std::map<int, Ogre::BillboardSet*> loadedBillboardSets;
    std::map<Ogre::String, std::vector<CompoundBodyInfo>> compoundBodiesParts;
    std::vector<LoadedInstanceForests> loadedForests;

    const float staticEntitiesGridSize = 30;
    const float maxOptimizableEntitySize = staticEntitiesGridSize;
    std::map<Ogre::String, std::vector<Ogre::Entity*>> loadedOptimizableEntities;

    void addOptimizableEntity(Ogre::Entity* e)
    {
        auto sizeVec = e->getBoundingBox().getSize();
        sizeVec *= e->getParentSceneNode()->_getDerivedScale();
        if (std::max(sizeVec.x, std::max(sizeVec.y, sizeVec.z)) > maxOptimizableEntitySize)
            return;

        auto m = e->getSubEntity(0)->getMaterialName();

        if (loadedOptimizableEntities.find(m) == loadedOptimizableEntities.end())
            loadedOptimizableEntities[m] = std::vector<Ogre::Entity*>();

        loadedOptimizableEntities[m].push_back(e);
    }

    struct mvec
    {
        int x;
        int y;
        int z;

        mvec::mvec(int xi, int yi, int zi) : x(xi), y(yi), z(zi) {}

        std::string str()
        {
            return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
        }
    };

    class comparator
    {
    public:
        bool operator()(const mvec& lhs, const mvec& rhs) const
        {
            if (lhs.x < rhs.x)
                return true;
            else if (lhs.y < rhs.y)
                return true;
            else if (lhs.z < rhs.z)
                return true;
            else
                return false;
        }
    };

    void optimizeEntities()
    {
        int sgCount = 0;

        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
        myLog->logMessage("Start of optimizing entities", LML_NORMAL);

        for (auto it : loadedOptimizableEntities)
        {
            auto entities = it.second;

            std::map < mvec, std::vector<Entity*>, comparator> grid;

            for (auto e : entities)
            {
                Ogre::Quaternion eQ = e->getParentNode()->_getDerivedOrientation();
                Ogre::Vector3 bbCenterOffset = eQ*e->getBoundingBox().getHalfSize() - eQ*e->getBoundingBox().getCenter();
                Ogre::Vector3 gridPos = (e->getParentNode()->_getDerivedPosition() - bbCenterOffset) / staticEntitiesGridSize;
                //Ogre::Vector3 gridPos = e->getParentNode()->_getDerivedPosition() / staticEntitiesGridSize;

                mvec mgridPos = mvec((int)gridPos.x, (int)gridPos.y, (int)gridPos.z);

                myLog->logMessage("Fitting entity with pos: " + Ogre::StringConverter::toString(e->getParentNode()->_getDerivedPosition()) + " and id: " + mgridPos.str() + " and m: " + it.first, LML_NORMAL);

                grid[mgridPos].push_back(e);
            }

            Ogre::StaticGeometry* sg = nullptr;

            for (auto gIt : grid)
            {
                auto& v = gIt.second;

                if (v.size()<2)
                    continue;

                if (!sg)
                {
                    sg = Global::mSceneMgr->createStaticGeometry("sg" + std::to_string(sgCount++));
                    sg->setRegionDimensions(Ogre::Vector3(staticEntitiesGridSize, staticEntitiesGridSize, staticEntitiesGridSize));
                    sg->setOrigin(Ogre::Vector3(0, 0, 0));
                    sg->setCastShadows(true);
                }

                myLog->logMessage("Creating geometry with number of entities: " + Ogre::StringConverter::toString(v.size()), LML_NORMAL);

                for (auto e : v)
                {
                    auto sn = e->getParentSceneNode();
                    auto pos = sn->_getDerivedPosition();
                    auto quat = sn->_getDerivedOrientation();
                    auto scale = sn->_getDerivedScale();

                    sn->detachAllObjects();
                    sg->addEntity(e, pos, quat, scale);
                }

            }

            if (sg)
                sg->build();

        }

        myLog->logMessage("End of optimizing entities", LML_NORMAL);

        loadedOptimizableEntities.clear();
    }

    String getElementValue(const XMLElement* rootElement, String elementName, String defaultValue = "")
    {
        auto e = rootElement->FirstChildElement(elementName.c_str());

        if (e == nullptr || e->GetText() == nullptr)
            return defaultValue;
        else
            return e->GetText();
    }

    bool getElementBoolValue(const XMLElement* rootElement, String elementName)
    {
        auto e = rootElement->FirstChildElement(elementName.c_str());

        if (e == nullptr || e->GetText() == nullptr)
            return false;
        else
            return getBool(e->GetText());
    }

    int getElementIntValue(const XMLElement* rootElement, String elementName, int defaultValue = 0)
    {
        auto e = rootElement->FirstChildElement(elementName.c_str());

        if (e == nullptr || e->GetText() == nullptr)
            return defaultValue;
        else
            return Ogre::StringConverter::parseInt(String(e->GetText()), defaultValue);
    }

    float getElementFloatValue(const XMLElement* rootElement, String elementName, float defaultValue = 0)
    {
        auto e = rootElement->FirstChildElement(elementName.c_str());

        if (e == nullptr || e->GetText() == nullptr)
            return defaultValue;
        else
            return Ogre::StringConverter::parseReal(String(e->GetText()), defaultValue);
    }

    Vector2 getElementV2Value(const XMLElement* rootElement, String elementName, Vector2 defaultValue = Vector2::ZERO)
    {
        auto e = rootElement->FirstChildElement(elementName.c_str());

        if (e == nullptr || e->GetText() == nullptr)
            return defaultValue;
        else
        {
            Ogre::String txt(e->GetText());

            Ogre::String f1 = txt.substr(0, txt.find_first_of(','));
            Ogre::String f2 = txt.substr(txt.find_last_of(',')+1);

            return Vector2(Ogre::StringConverter::parseReal(f1), Ogre::StringConverter::parseReal(f2));
        }
    }

    Vector3 getElementV3Value(const XMLElement* rootElement, String elementName, Vector3 defaultValue = Vector3::ZERO)
    {
        auto e = rootElement->FirstChildElement(elementName.c_str());

        if (e == nullptr || e->GetText() == nullptr)
            return defaultValue;
        else
        {
            Ogre::String txt(e->GetText());

            Ogre::String f1 = txt.substr(0, txt.find_first_of(','));
            Ogre::String f2 = txt.substr(txt.find_first_of(',')+1, txt.find_last_of(','));
            Ogre::String f3 = txt.substr(txt.find_last_of(',')+1);

            return Vector3(Ogre::StringConverter::parseReal(f1), Ogre::StringConverter::parseReal(f2), Ogre::StringConverter::parseReal(f3));
        }
    }

    const XMLElement* IterateChildElements(const XMLElement* xmlElement, const XMLElement* childElement)
    {
        if (xmlElement != 0)
        {
            if (childElement == 0)
                childElement = xmlElement->FirstChildElement();
            else
                childElement = childElement->NextSiblingElement();

            return childElement;
        }

        return 0;
    }

    bool getBool(const String& value)
    {
        String valueLower = value;
        StringUtil::toLowerCase(valueLower);
        if (valueLower == "false" || value == "no" || value == "0")
            return false;
        else
            return true;
    }

    Real GetRealAttribute(const XMLElement* xmlElement, const char* name, Real defaultValue = 0)
    {
        String value = xmlElement->Attribute(name);
        return value.empty() ? defaultValue : StringConverter::parseReal(value);
    }

    int GetIntAttribute(const XMLElement* xmlElement, const char* name, int defaultValue = 0)
    {
        String value = xmlElement->Attribute(name);
        return value.empty() ? defaultValue : StringConverter::parseInt(value);
    }

    Vector3 LoadXYZ(const XMLElement* objectElement)
    {
        Vector3 xyz;
        xyz.x = GetRealAttribute(objectElement, "x");
        xyz.y = GetRealAttribute(objectElement, "y");
        xyz.z = GetRealAttribute(objectElement, "z");
        return xyz;
    }

    Light::LightTypes ParseLightType(const String& type)
    {
        String typeLower = type;
        StringUtil::toLowerCase(typeLower);

        if (typeLower == "point")
            return Light::LT_POINT;
        else if (typeLower == "directional")
            return Light::LT_DIRECTIONAL;
        else if (typeLower == "spot")
            return Light::LT_SPOTLIGHT;

        StringUtil::StrStreamType errorMessage;
        errorMessage << "Invalid light type specified: " << type;

        OGRE_EXCEPT
        (
            Exception::ERR_INVALIDPARAMS,
            errorMessage.str(),
            "OgreMaxUtilities::ParseLightType"
        );
    }

    Quaternion LoadRotation(const XMLElement* objectElement)
    {
        Quaternion rotation = Quaternion::IDENTITY;

        rotation.x = GetRealAttribute(objectElement, "qx", 1);
        rotation.y = GetRealAttribute(objectElement, "qy", 0);
        rotation.z = GetRealAttribute(objectElement, "qz", 1);
        rotation.w = GetRealAttribute(objectElement, "qw", 0);

        return rotation;
    }

    ColourValue LoadColor(const XMLElement* objectElement)
    {
        ColourValue color;
        color.r = GetRealAttribute(objectElement, "r", 0);
        color.g = GetRealAttribute(objectElement, "g", 0);
        color.b = GetRealAttribute(objectElement, "b", 0);
        color.a = 1;
        return color;
    }

    String GetStringAttribute(const XMLElement* xmlElement, const char* name)
    {
        const char* value = xmlElement->Attribute(name);
        if (value != 0)
            return value;
        else
            return StringUtil::BLANK;
    }

    std::string getElementText(const XMLElement* xmlElement)
    {
        auto txt = xmlElement->GetText();

        if (txt == NULL)
            return "";
        else
            return txt;
    }

    void LoadLightAttenuation(const XMLElement* objectElement, Light* light)
    {

        String value;

        value = GetStringAttribute(objectElement, "range");
        Real range = value.empty() ? light->getAttenuationRange() : StringConverter::parseReal(value);

        value = GetStringAttribute(objectElement, "constant");
        Real constant = value.empty() ? light->getAttenuationConstant() : StringConverter::parseReal(value);

        value = GetStringAttribute(objectElement, "linear");
        Real linear = value.empty() ? light->getAttenuationLinear() : StringConverter::parseReal(value);

        value = GetStringAttribute(objectElement, "quadric");
        Real quadric = value.empty() ? light->getAttenuationQuadric() : StringConverter::parseReal(value);

        light->setAttenuation(range, constant, linear, quadric);
    }

    void LoadLightRange(const XMLElement* objectElement, Light* light)
    {

        if (light->getType() == Light::LT_SPOTLIGHT)
        {
            String value;

            value = GetStringAttribute(objectElement, "inner");
            if (!value.empty())
                light->setSpotlightInnerAngle(Radian(StringConverter::parseReal(value)));

            value = GetStringAttribute(objectElement, "outer");
            if (!value.empty())
                light->setSpotlightOuterAngle(Radian(StringConverter::parseReal(value)));

            value = GetStringAttribute(objectElement, "falloff");
            if (!value.empty())
                light->setSpotlightFalloff(StringConverter::parseReal(value));
        }
    }

    bool AllDigits(const String& text)
    {
        for (size_t i = 0; i < text.length(); i++)
        {
            if (!isdigit(text[i]))
                return false;
        }

        return true;
    }

    uint8 ParseRenderQueue(const String& renderQueue)
    {
        static std::map<String, uint8> nameToNumber;
        if (nameToNumber.empty())
        {
            nameToNumber["background"] = RENDER_QUEUE_BACKGROUND;
            nameToNumber["skiesearly"] = RENDER_QUEUE_SKIES_EARLY;
            nameToNumber["queue1"] = RENDER_QUEUE_1;
            nameToNumber["queue2"] = RENDER_QUEUE_2;
            nameToNumber["worldgeometry1"] = RENDER_QUEUE_WORLD_GEOMETRY_1;
            nameToNumber["queue3"] = RENDER_QUEUE_3;
            nameToNumber["queue4"] = RENDER_QUEUE_4;
            nameToNumber["main"] = RENDER_QUEUE_MAIN;
            nameToNumber["queue6"] = RENDER_QUEUE_6;
            nameToNumber["queue7"] = RENDER_QUEUE_7;
            nameToNumber["worldgeometry2"] = RENDER_QUEUE_WORLD_GEOMETRY_2;
            nameToNumber["queue8"] = RENDER_QUEUE_8;
            nameToNumber["queue9"] = RENDER_QUEUE_9;
            nameToNumber["skieslate"] = RENDER_QUEUE_SKIES_LATE;
            nameToNumber["overlay"] = RENDER_QUEUE_OVERLAY;
            nameToNumber["max"] = RENDER_QUEUE_MAX;
        }

        if (renderQueue.empty())
            return RENDER_QUEUE_MAIN;
        else if (AllDigits(renderQueue))
            return (uint8)StringConverter::parseUnsignedInt(renderQueue);
        else
        {
            //The render queue name, lowercase
            String renderQueueLower;

            //Get the offset that comes after the +, if any
            uint8 offset = 0;
            size_t plusFoundAt = renderQueue.find('+');
            if (plusFoundAt != String::npos)
            {
                //Parse the number
                String offsetText = renderQueue.substr(plusFoundAt + 1);
                StringUtil::trim(offsetText);

                offset = (uint8)StringConverter::parseUnsignedInt(offsetText);

                //Remove the "+offset" substring from the render queue name
                renderQueueLower = renderQueue.substr(0, plusFoundAt);
                StringUtil::trim(renderQueueLower);
            }
            else
                renderQueueLower = renderQueue;
            StringUtil::toLowerCase(renderQueueLower);

            //Look up the render queue and return it
            std::map<String, uint8>::iterator item = nameToNumber.find(renderQueueLower);
            if (item != nameToNumber.end())
            {
                //Don't let the render queue exceed the maximum
                return std::min((uint8)(item->second + offset), (uint8)RENDER_QUEUE_MAX);
            }
            else
            {
                StringUtil::StrStreamType errorMessage;
                errorMessage << "Invalid render queue specified: " << renderQueue;

                OGRE_EXCEPT
                (
                    Exception::ERR_INVALIDPARAMS,
                    errorMessage.str(),
                    "OgreMaxUtilities::ParseRenderQueue"
                );
            }
        }
    }

    void GetChildText(const XMLElement* xmlElement, String& text)
    {
        //Get the first element
        const XMLNode* childNode = xmlElement->FirstChild();
        while (childNode != 0)
        {
            //if (childNode->Type() == XMLNode::TEXT)
            {
                const XMLText* textNode = childNode->ToText();
                if (textNode != 0)
                {
                    text = textNode->Value();
                    break;
                }
            }
            childNode = xmlElement->NextSibling();
        }
    }

    void loadCrowFlight(const XMLElement* rootElement, Entity* ent, SceneNode* node)
    {
        int mID = getElementIntValue(rootElement, "ID");
        int crows = getElementIntValue(rootElement, "InitCrows");

        float randomYaw = getElementFloatValue(rootElement, "RandomYaw");
        float flightTime = getElementFloatValue(rootElement, "Flighttime");
        float switchTime = getElementFloatValue(rootElement, "Switchtime");

        CrowsManager::get()->addCrowFlight(crows, randomYaw, flightTime, switchTime, node, mID);

        node->detachAllObjects();
        Global::mSceneMgr->destroyEntity(ent);
    }

    void loadCrowLanding(const XMLElement* rootElement, Entity* ent, SceneNode* node)
    {
        int mID = getElementIntValue(rootElement, "ID");
        int crows = getElementIntValue(rootElement, "InitCrows");

        int maxCrows = getElementIntValue(rootElement, "MaxCrows");
        float groundTime = getElementFloatValue(rootElement, "Groundtime");
        bool walking = getElementBoolValue(rootElement, "AllowWalk");

        CrowsManager::get()->addCrowLanding(crows, maxCrows, groundTime, walking, node, mID);

        node->detachAllObjects();
        Global::mSceneMgr->destroyEntity(ent);
        Global::mSceneMgr->destroySceneNode(node);
    }

    void loadParticle(const XMLElement* rootElement, Entity* ent, SceneNode* node)
    {
        static int partID = 0;
        Ogre::String name = getElementValue(rootElement, "Name");
        int rGroup = getElementIntValue(rootElement, "RenderQroup", RenderQueue_Particles);

        Ogre::ParticleSystem* ps = Global::mSceneMgr->createParticleSystem("Particle" + std::to_string(partID++), name);
        ps->setRenderQueueGroup(rGroup);
        ps->setVisibilityFlags(VisibilityFlag_SoftParticles);
        if (getElementBoolValue(rootElement, "EditParams"))
        {
            auto mat = getElementValue(rootElement, "Material");
            if (!mat.empty()) ps->setMaterialName(mat);

            auto psize = getElementV2Value(rootElement, "Size");
            if (psize.x != 0) ps->setDefaultDimensions(psize.x, psize.y);
        }

        node->detachAllObjects();
        Global::mSceneMgr->destroyEntity(ent);
        node->attachObject(ps);
    }

    void loadPlane(const XMLElement* planeElement, SceneNode* node, Ogre::SceneManager *mSceneMgr)
    {
        Ogre::Vector3 normal;
        Ogre::Vector3 upVector;

        const XMLElement* childElement = 0;
        while (childElement = IterateChildElements(planeElement, childElement))
        {


            String elementName = childElement->Value();

            if (elementName == "normal")
                normal = LoadXYZ(childElement);
            else if (elementName == "upVector")
                upVector = LoadXYZ(childElement);

        }

        Ogre::Plane plane(normal, GetRealAttribute(planeElement, "distance"));
        MeshManager::getSingleton().createPlane
        (
            planeElement->Attribute("name"),
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            plane,
            GetRealAttribute(planeElement, "width"), GetRealAttribute(planeElement, "height"),
            GetIntAttribute(planeElement, "xSegments"), GetIntAttribute(planeElement, "ySegments"),
            getBool(planeElement->Attribute("normals")), GetIntAttribute(planeElement, "numTexCoordSets"),
            GetRealAttribute(planeElement, "uTile"), GetRealAttribute(planeElement, "vTile"),
            upVector
        );

        Ogre::Entity* ent;
        ent = mSceneMgr->createEntity(planeElement->Attribute("name"), planeElement->Attribute("name"));
        ent->getMesh()->buildTangentVectors();
        node->attachObject(ent);
        ent->setCastShadows(getBool(planeElement->Attribute("castShadows")));
        ent->setMaterialName(planeElement->Attribute("material"));
        ent->setVisibilityFlags(1);

        Ogre::uint8 renderQueue = ParseRenderQueue(GetStringAttribute(planeElement, "renderQueue"));
        ent->setRenderQueueGroup(renderQueue);
    }

    void loadLight(const XMLElement* lightElement, SceneNode* node, Ogre::SceneManager *mSceneMgr)
    {
        auto type = ParseLightType(GetStringAttribute(lightElement, "type"));
        Ogre::String name = lightElement->Attribute("name");

        if (type == Light::LT_DIRECTIONAL && !mSceneMgr->hasLight("Sun"))
            name = "Sun";

        //Create the light
        Light* light = mSceneMgr->createLight(name);
        light->setType(type);
        light->setCastShadows(getBool(lightElement->Attribute("castShadows")));
        light->setPowerScale(GetRealAttribute(lightElement, "power"));
        node->attachObject(light);

        const XMLElement* childElement = 0;
        while (childElement = IterateChildElements(lightElement, childElement))
        {


            String elementName = childElement->Value();

            if (elementName == "colourDiffuse")
                light->setDiffuseColour(LoadColor(childElement));
            else if (elementName == "colourSpecular")
                light->setSpecularColour(LoadColor(childElement));
            else if (elementName == "lightRange")
                LoadLightRange(childElement, light);
            else if (elementName == "lightAttenuation")
                LoadLightAttenuation(childElement, light);
            else if (elementName == "position")
                light->setPosition(LoadXYZ(childElement));
            else if (elementName == "normal")
                light->setDirection(LoadXYZ(childElement));
            else if (elementName == "rotation")
                node->rotate(LoadRotation(childElement));
        }
    }

    void loadActions(const XMLElement* rootElement, EventsManager* mEventMgr, OgreNewt::Body* body, bodyUserData* userD, OgreNewt::MaterialID* action_mat)
    {
        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");

        userD->makeActions = Ogre::StringConverter::parseBool(getElementValue(rootElement, "Amaker"));

        auto actionsStr = getElementValue(rootElement, "Actions");

        if (!actionsStr.empty())
        {
            int i = 0;
            auto aID = MUtils::strtok_str(actionsStr, ';');
            while (!aID.empty() && i < 4)
            {
                //add action
                userD->actions[i] = stoi(aID);
                myLog->logMessage("loaded aID " + Ogre::String(aID), LML_NORMAL);
                i++;
                aID = MUtils::strtok_str(actionsStr, ';');
            }
            body->setMaterialGroupID(action_mat);
        }

        auto startupTasks = getElementValue(rootElement, "Aonstart");

        if (!startupTasks.empty())
        {
            int i = 0;

            char separator = ';';

            bool newForm = false;
            if (startupTasks.find("\n") != std::string::npos || startupTasks.find(')') == startupTasks.length() - 1)
            {
                newForm = true;
                separator = '\n';
            }

            auto react = MUtils::strtok_str(startupTasks, separator);
            while (!react.empty())
            {
                Ogre::String taskStr = Ogre::String(react);

                while (taskStr.at(0) == ' ')
                    taskStr.erase(0, 1);

                float delay = getTaskDelay(taskStr);

                //remove function parentheses characters
                if (newForm)
                {
                    char chars[] = "()";

                    for (unsigned int i = 0; i < strlen(chars); ++i)
                    {
                        taskStr.erase(std::remove(taskStr.begin(), taskStr.end(), chars[i]), taskStr.end());
                    }
                }

                EventTask* r = makeTask(taskStr);

                if (r != NULL)
                {
                    r->setUserData(body);
                    r->taskDelay = delay;

                    mEventMgr->addTask(r);
                }

                react = MUtils::strtok_str(startupTasks, separator);
            }
        }
    }

    Ogre::ColourValue getChannelMask(std::string channel)
    {
        return ColourValue(channel == "R" ? 1.0f : 0.0f, channel == "G" ? 1.0f : 0.0f, channel == "B" ? 1.0f : 0.0f, channel == "A" ? 1.0f : 0.0f);
    }

    bool isEditMask(std::string maskName)
    {
        //"Mask: ..."
        return maskName.size() > 5 && maskName[4] == ':';
    }

    bool isNextChannel(const XMLElement* rootElement, std::string maskChannel, std::string targetChannel)
    {
        std::string channels[] {"R", "G", "B", "A"};

        bool nextChannel = false;
        bool validNextPosTarget = false;
        for (int i = 0; i < 4; i++)
        {
            if (channels[i] == targetChannel)
                validNextPosTarget = true;
            else if (validNextPosTarget && channels[i] == maskChannel)
                nextChannel = true;
            else if (validNextPosTarget)
            {
                auto t = getElementValue(rootElement, "Type" + channels[i]);
                validNextPosTarget = (t.empty() || isEditMask(t));
            }
        }

        return nextChannel;
    }

    void loadCustomGeometryEdit(const XMLElement* rootElement, std::string channel, std::string targetChannel, DetailGeometryCustomEdit& info)
    {
        auto strType = getElementValue(rootElement, "Type" + channel);

        if (strType == "Mask: Scale" || (strType == "Mask: Last scale" && isNextChannel(rootElement, channel, targetChannel)))
        {
            info.customMinmaxScale = getElementV2Value(rootElement, "ScaleMinMax" + channel);
            info.customScaleMask = getChannelMask(channel);
            info.customScaleEnabled = true;
        }
        else if (strType == "Mask: VC set" || strType == "Mask: VC mul")
        {
            info.customVCMode = (strType == "Mask: VC mul") ? MulVC : SetVC;
            info.customVCMask = getChannelMask(channel);
            info.customVCColor = getElementV3Value(rootElement, "Color" + channel);
        }

    }

    void loadDetailGeometryChannel(const XMLElement* rootElement, std::string channel, std::vector<DetailGeometryParams>& geometries)
    {
        bool enabled = getElementBoolValue(rootElement, channel);
        auto strType = getElementValue(rootElement, "Type" + channel);

        bool isEMask = isEditMask(strType);

        if (enabled && !isEMask)
        {
            DetailGeometryParams info;

            //find edit masks
            char* channels[] {"R", "G", "B", "A"};
            for (int i = 0; i < 4; i++)
            {
                std::string c = channels[i];

                if (isEditMask(getElementValue(rootElement, "Type" + c)))
                {
                    loadCustomGeometryEdit(rootElement, c, channel, info.customEdit);
                }
            }

            info.weightMask = getChannelMask(channel);
            info.name = strType;
            info.minmaxScale = getElementV2Value(rootElement, "ScaleMinMax" + channel);
            info.color = getElementV3Value(rootElement, "Color" + channel);
            info.density = getElementFloatValue(rootElement, "Density" + channel);

            geometries.push_back(info);
        }
    }

    void loadDetailGeometryMask(const XMLElement* rootElement, Entity* ent)
    {
        float ray = getElementFloatValue(rootElement, "Ray");

        auto targetName = getElementValue(rootElement, "Target");
        auto targetBodyIt = loadedBodies.find(targetName);
        auto body = targetBodyIt == loadedBodies.end() ? nullptr : targetBodyIt->second;

        std::vector<DetailGeometryParams> geometries;
        loadDetailGeometryChannel(rootElement, "R", geometries);
        loadDetailGeometryChannel(rootElement, "G", geometries);
        loadDetailGeometryChannel(rootElement, "B", geometries);
        loadDetailGeometryChannel(rootElement, "A", geometries);

        Global::gameMgr->geometryMgr->addDetailGeometryMask(ent, geometries, body, ray);

        auto node = ent->getParentSceneNode();

        node->detachObject(ent);
        Global::mSceneMgr->destroyEntity(ent);
        Global::mSceneMgr->destroySceneNode(node);
    }

    void loadDetailGeometry(const XMLElement* rootElement, Entity* ent)
    {
        int id = getElementIntValue(rootElement, "ID");
        auto type = getElementValue(rootElement, "Type");
        auto keepMesh = getElementBoolValue(rootElement, "KeepMesh");
        auto color = getElementV3Value(rootElement, "Color");
        auto node = ent->getParentSceneNode();

        Global::gameMgr->geometryMgr->addDetailGeometryEntity(id, node, type, keepMesh, color);

        node->detachObject(ent);
        Global::mSceneMgr->destroySceneNode(node);

        if (!keepMesh)
            Global::mSceneMgr->destroyEntity(ent);
    }

    void loadGrassArea(const XMLElement* element, Entity* ent, SceneNode* node, Ogre::SceneManager *mSceneMgr)
    {
        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");

        int pageSize = Ogre::StringConverter::parseInt(element->GetText());
        element = element->NextSiblingElement();
        int maxRange = Ogre::StringConverter::parseInt(element->GetText());
        element = element->NextSiblingElement();
        int transLength = Ogre::StringConverter::parseInt(element->GetText());
        element = element->NextSiblingElement();
        int layersNum = Ogre::StringConverter::parseInt(element->GetText());
        element = element->NextSiblingElement();

        std::string materials = element->GetText();
        element = element->NextSiblingElement();
        std::string  minSizesW = element->GetText();
        element = element->NextSiblingElement();
        std::string  minSizesH = element->GetText();
        element = element->NextSiblingElement();
        std::string  maxSizesW = element->GetText();
        element = element->NextSiblingElement();
        std::string  maxSizesH = element->GetText();
        element = element->NextSiblingElement();
        bool anim = Ogre::StringConverter::parseBool(element->GetText());
        element = element->NextSiblingElement();
        std::string  SwayDistributions = element->GetText();
        element = element->NextSiblingElement();
        std::string  SwayLengths = element->GetText();
        element = element->NextSiblingElement();
        std::string  SwaySpeeds = element->GetText();
        element = element->NextSiblingElement();
        int fadeTech = Ogre::StringConverter::parseInt(element->GetText());
        element = element->NextSiblingElement();
        std::string  densities = element->GetText();
        element = element->NextSiblingElement();
        std::string  densityMaps = element->GetText()==NULL ? "" : element->GetText();
        element = element->NextSiblingElement();
        std::string  colorMaps = element->GetText() == NULL ? "" : element->GetText();

        bool usesDensityMap = (!densityMaps.empty());
        bool usesColorMap = (!colorMaps.empty());

        Forests::PagedGeometry *grass = new Forests::PagedGeometry(mSceneMgr->getCamera("Camera"), (float)pageSize);
        grass->setVisibilityFlags(ent->getVisibilityFlags());
        grass->addDetailLevel<Forests::GrassPage>((float)maxRange, (float)transLength);

        TerrainHeightQueryData* offsets = new TerrainHeightQueryData();
        offsets->offset_maxY = ent->getBoundingBox().getMaximum().y + node->getPosition().y;
        offsets->offset_minY = ent->getBoundingBox().getMinimum().y + node->getPosition().y;
        offsets->world = Global::mWorld;

        Forests::GrassLoader *grassLoader = new Forests::GrassLoader(grass);

        auto belement = element->NextSiblingElement("BodyFilter");
        if (belement && belement->GetText())
        {
            LoadedFilteredGrassArea g;
            g.data = offsets;
            g.targetBody = belement->GetText();

            loadedGrassAreas.push_back(g);
            grassLoader->setHeightFunction(&HeightFunction::getTerrainHeightFiltered, offsets);
        }
        else
            grassLoader->setHeightFunction(&HeightFunction::getTerrainHeight, offsets);

        auto startElement = element->NextSiblingElement("PlaneOffsetStart");
        auto endElement = element->NextSiblingElement("PlaneOffsetEnd");
        if (startElement && endElement && startElement->GetText() && endElement->GetText())
        {
            offsets->offset_maxY = offsets->offset_maxY - std::stoi(startElement->GetText());
            offsets->offset_minY = offsets->offset_minY - std::stoi(endElement->GetText());
        }

        grass->setPageLoader(grassLoader);
        char delim = ';';

        for (int i = 0; i < layersNum; i++)
        {
            String curMat = MUtils::strtok_str(materials, delim);
            Real curMinSizeW = Ogre::StringConverter::parseReal(MUtils::strtok_str(minSizesW, delim));
            Real curMinSizeH = Ogre::StringConverter::parseReal(MUtils::strtok_str(minSizesH, delim));
            Real curMaxSizeW = Ogre::StringConverter::parseReal(MUtils::strtok_str(maxSizesW, delim));
            Real curMaxSizeH = Ogre::StringConverter::parseReal(MUtils::strtok_str(maxSizesH, delim));

            Real curSwayDistr = Ogre::StringConverter::parseReal(MUtils::strtok_str(SwayDistributions, delim));
            Real curSwayLen = Ogre::StringConverter::parseReal(MUtils::strtok_str(SwayLengths, delim));
            Real curSwaySpeed = Ogre::StringConverter::parseReal(MUtils::strtok_str(SwaySpeeds, delim));
            Real curDensity = Ogre::StringConverter::parseReal(MUtils::strtok_str(densities, delim));

            Forests::GrassLayer *layer = grassLoader->addLayer(curMat);
            layer->setMaterial(curMat);
            layer->setMaxSlope(Ogre::Radian(Ogre::Degree(50)));
            layer->setMinimumSize(curMinSizeW, curMinSizeH);
            layer->setMaximumSize(curMaxSizeW, curMaxSizeH);
            layer->setAnimationEnabled(anim);
            layer->setSwayDistribution(curSwayDistr);
            layer->setSwayLength(curSwayLen);
            layer->setSwaySpeed(curSwaySpeed);
            layer->setDensity(curDensity);

            if (usesDensityMap)
            {
                String curDenMat = MUtils::strtok_str(densityMaps, delim);
                layer->setDensityMap(curDenMat);
                myLog->logMessage("Grass Area loaded density map " + curDenMat, LML_NORMAL);
            }

            if (usesColorMap)
            {
                String curColMat = MUtils::strtok_str(colorMaps, delim);
                layer->setColorMap(curColMat);
                myLog->logMessage("Grass Area loaded color map " + curColMat, LML_NORMAL);
            }

            if (fadeTech == 0)
                layer->setFadeTechnique(Forests::FADETECH_ALPHA);
            else if (fadeTech == 1)
                layer->setFadeTechnique(Forests::FADETECH_GROW);
            else
                layer->setFadeTechnique(Forests::FADETECH_ALPHAGROW);

            Ogre::Vector4 bounds;

            bounds.x = ent->getBoundingBox().getMinimum().x*node->getScale().x + node->getPosition().x;
            bounds.y = ent->getBoundingBox().getMinimum().z*node->getScale().z + node->getPosition().z;
            bounds.z = ent->getBoundingBox().getMaximum().x*node->getScale().x + node->getPosition().x;
            bounds.w = ent->getBoundingBox().getMaximum().z*node->getScale().z + node->getPosition().z;

            layer->setMapBounds(Forests::TBounds(bounds.x, bounds.y, bounds.z, bounds.w));
            myLog->logMessage("Grass Area ended loading layer number " + Ogre::StringConverter::toString(i), LML_NORMAL);
        }

        node->detachObject(ent);
        mSceneMgr->destroyEntity(ent);
        mSceneMgr->destroySceneNode(node);

        Global::gameMgr->geometryMgr->addPagedGeometry(grass);
        myLog->logMessage("Grass Area Loaded", LML_NORMAL);
    }

    void loadReflection(const XMLElement* element, Ogre::Entity* ent, SceneNode* node, Ogre::SceneManager* mSceneMgr)
    {
        Ogre::Vector3 pos = node->getPosition();
        Ogre::Degree yaw = node->getOrientation().getYaw();

        int mask = Ogre::StringConverter::parseInt(element->GetText());

        ReflectionTask* refl = new ReflectionTask();
        refl->init(ent, mask);
        Global::mEventsMgr->addTask(refl);
    }

    void loadInstance(const XMLElement* element, Ogre::Entity* ent, SceneNode* node, Ogre::SceneManager* mSceneMgr)
    {
        Ogre::Vector3 pos = node->getPosition();
        Ogre::Degree yaw = node->getOrientation().getYaw();

        int gMax = Ogre::StringConverter::parseInt(element->GetText());
        element = element->NextSiblingElement();
        int iMax = Ogre::StringConverter::parseInt(element->GetText());
        element = element->NextSiblingElement();
        int tran = Ogre::StringConverter::parseInt(element->GetText());
        element = element->NextSiblingElement();

        Forests::PagedGeometry *trees = 0;

        for (auto f = loadedForests.cbegin(); f != loadedForests.cend(); f++)
        {
            if (gMax == f->gm && iMax == f->im && tran == f->tr)
            {
                trees = f->g;
                break;
            }
        }
        if (trees == 0)
        {
            trees = new Forests::PagedGeometry();
            trees->setCamera(mSceneMgr->getCamera("Camera"));	//Set the camera so PagedGeometry knows how to calculate LODs
            trees->setPageSize(80);	//Set the size of each page of geometry
            trees->setInfinite();		//Use infinite paging mode
            trees->addDetailLevel<Forests::BatchPage>((float)gMax, (float)tran);		//Use batches up to 150 units away, and fade for 30 more units
            trees->addDetailLevel<Forests::ImpostorPage>((float)iMax, (float)tran);	//Use impostors up to 400 units, and for for 50 more units
            Forests::TreeLoader3D *treeLoader = new Forests::TreeLoader3D(trees, Forests::TBounds(-1500, -1500, 1500, 1500));
            trees->setPageLoader(treeLoader);	//Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance
            Global::gameMgr->geometryMgr->addPagedGeometry(trees);
            LoadedInstanceForests lf;
            lf.gm = gMax;
            lf.im = iMax;
            lf.tr = tran;
            lf.g = trees;
            loadedForests.push_back(lf);
        }

        float scale = node->getScale().x;
        scale = scale < node->getScale().y ? node->getScale().y : scale;
        scale = scale < node->getScale().z ? node->getScale().z : scale;
        node->detachAllObjects();
        mSceneMgr->destroySceneNode(node);
        ((Forests::TreeLoader3D *)trees->getPageLoader())->addTree(ent, pos, yaw, scale);

    }

    void loadSceneCubeMap(const XMLElement* element, Ogre::Entity* ent, SceneNode* node)
    {
        SceneCubeMap* cube = new SceneCubeMap();

        auto res = getElementIntValue(element, "Resolution");
        auto edit = getElementBoolValue(element, "Editable");
        auto minDist = getElementFloatValue(element, "MinRenderDistance");

        cube->init(node->getName(), res, edit, minDist);

        auto offsetW = getElementFloatValue(element, "PositionOffsetW");
        auto radius = getElementFloatValue(element, "ObjectsRadius");

        cube->position = node->_getDerivedPosition();
        cube->posessionRadius = radius;
        cube->materialWPOffset = offsetW;

        node->detachAllObjects();
        Global::mSceneMgr->destroyEntity(ent);
        Global::mSceneMgr->destroySceneNode(node);
    }

    void loadWaterCurrent(const XMLElement* element, Ogre::Entity* ent, SceneNode* node, Ogre::SceneManager* mSceneMgr)
    {
        std::vector<Ogre::Vector3> points;

        auto power = Ogre::StringConverter::parseReal(getElementValue(element, "Power"));
        auto minWidth = Ogre::StringConverter::parseReal(getElementValue(element, "MinWidth"));

        auto m = ent->getMesh().get()->getSubMesh(0);
        const Ogre::VertexElement* posElem = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
        Ogre::HardwareVertexBufferSharedPtr vbuf = m->vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
        unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        Ogre::Real* pReal;

        for (size_t j = 0; j < m->vertexData->vertexCount; ++j, vertex += vbuf->getVertexSize())
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);

            Vector3 pt;

            pt.x = (*pReal++);
            pt.y = (*pReal++);
            pt.z = (*pReal++);

            pt *= node->getScale();
            pt += node->getPosition();
            pt = node->getOrientation()*pt;

            if (j == 0 || j % 2 == 1)
                points.push_back(pt);
        }
        vbuf->unlock();

        for (size_t i = 1; i < points.size(); i++)
        {
            WaterCurrent::get()->addCurrent(points[i - 1], points[i], power, minWidth);
        }

        node->detachAllObjects();
        mSceneMgr->destroyEntity(ent);
        mSceneMgr->destroySceneNode(node);
    }

    void loadSlideTrack(const XMLElement* element, Ogre::Entity* ent, SceneNode* node, Ogre::SceneManager* mSceneMgr)
    {
        std::vector<Ogre::Vector3> points;

        auto loop = Ogre::StringConverter::parseBool(getElementValue(element, "Loop"));
        auto bidir = Ogre::StringConverter::parseBool(getElementValue(element, "Bidirectional"));
        auto speed = Ogre::StringConverter::parseReal(getElementValue(element, "Speed"));
        auto top = Ogre::StringConverter::parseBool(getElementValue(element, "Top"));
        auto animTrack = getElementValue(element, "Animation");

        Slide* line;

        if (animTrack.empty())
        {
            auto m = ent->getMesh().get()->getSubMesh(0);
            const Ogre::VertexElement* posElem = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
            Ogre::HardwareVertexBufferSharedPtr vbuf = m->vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
            unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            Ogre::Real* pReal;

            for (size_t j = 0; j < m->vertexData->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);

                Vector3 pt;

                pt.x = (*pReal++);
                pt.y = (*pReal++);
                pt.z = (*pReal++);

                pt *= node->getScale();
                pt += node->getPosition();
                pt = node->getOrientation()*pt;

                if (j == 0 || j % 2 == 1)
                    points.push_back(pt);
            }
            vbuf->unlock();

            if (top)
                line = new TopSlide(points, node->getName(), loop, speed);
            else
                line = new ZipLineSlide(points, node->getName(), loop, speed);

            line->bidirectional = bidir;
        }
        else
            line = new TopSlide(node, node->getName(), animTrack, loop, speed);

        loadedSlides[node->getName()] = line;

        Global::player->pSliding->addLoadedSlide(line);

        node->detachAllObjects();
        mSceneMgr->destroyEntity(ent);
        //mSceneMgr->destroySceneNode(node);
    }

    void loadBillboard(const XMLElement* element, Ogre::Entity* ent, Ogre::SceneManager* mSceneMgr)
    {
        int billboardSet = Ogre::StringConverter::parseInt(element->GetText());
        element = element->NextSiblingElement();
        int orientation = Ogre::StringConverter::parseInt(element->GetText());

        Ogre::BillboardSet* mySet;

        auto it = loadedBillboardSets.find(billboardSet);
        if (it == loadedBillboardSets.end())
        {
            Ogre::SceneNode* myNode = static_cast<Ogre::SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
            mySet = mSceneMgr->createBillboardSet("billboardSet" + Ogre::StringConverter::toString(billboardSet));
            mySet->setMaterialName(ent->getSubEntity(0)->getMaterialName());
            mySet->setVisibilityFlags(ent->getVisibilityFlags());

            if (orientation == 1)
            {
                mySet->setBillboardType(Ogre::BillboardType::BBT_ORIENTED_COMMON);
                mySet->setCommonDirection(Ogre::Vector3(0, 1, 0));
            }
            myNode->attachObject(mySet);
            loadedBillboardSets[billboardSet] = mySet;
        }
        else
        {
            mySet = it->second;
        }

        Ogre::Billboard* myBillboard = mySet->createBillboard(ent->getParentSceneNode()->_getDerivedPosition());

        Ogre::FloatRect uvRect;
        float width, height;

        Ogre::SubMesh* pSubMesh = ent->getMesh()->getSubMesh(0);

        const Ogre::VertexElement*  VertexEle_POS = pSubMesh->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
        Ogre::HardwareVertexBufferSharedPtr VertexBufPOS = pSubMesh->vertexData->vertexBufferBinding->getBuffer(VertexEle_POS->getSource());
        unsigned char* VertexPtrPOS = static_cast<unsigned char*>(VertexBufPOS->lock(Ogre::HardwareBuffer::LockOptions::HBL_NORMAL));
        int VertSizePOS = VertexBufPOS->getVertexSize();
        float * pElementPOS = NULL;
        const Ogre::VertexElement*  VertexEle_UV = pSubMesh->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);
        Ogre::HardwareVertexBufferSharedPtr VertexBufUV = pSubMesh->vertexData->vertexBufferBinding->getBuffer(VertexEle_UV->getSource());
        unsigned char* VertexPtrUV = static_cast<unsigned char*>(VertexBufUV->lock(Ogre::HardwareBuffer::LockOptions::HBL_NORMAL));
        int VertSizeUV = VertexBufUV->getVertexSize();
        float * pElementUV = NULL;

        for (int nVert = 0; nVert < 6; nVert++)
        {
            VertexEle_POS->baseVertexPointerToElement(VertexPtrPOS, &pElementPOS);
            VertexEle_UV->baseVertexPointerToElement(VertexPtrUV, &pElementUV);

            if (nVert == 0)
            {
                uvRect.right = pElementUV[0];
                uvRect.top = pElementUV[1];
                width = -pElementPOS[0];
                height = -pElementPOS[2];
            }
            else if (nVert == 3)
            {
                uvRect.left = pElementUV[0];
                uvRect.bottom = pElementUV[1];
                width += pElementPOS[0];
                height += pElementPOS[2];
            }

            // move on to the next vertex
            VertexPtrPOS += VertSizePOS;
            VertexPtrUV += VertSizeUV;
        }

        VertexBufPOS->unlock();
        VertexBufUV->unlock();


        myBillboard->setTexcoordRect(uvRect);
        myBillboard->setDimensions(width, height);

        Ogre::SceneNode* n = ent->getParentSceneNode();
        n->detachAllObjects();
        mSceneMgr->destroyEntity(ent);
        mSceneMgr->destroySceneNode(n);

    }

    OgreNewt::Body* loadPhysics(const XMLElement* rootElement, Entity* ent, SceneNode* node, OgreNewt::World* mWorld, EventsManager* mEventMgr, WorldMaterials* wMaterials)
    {
        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
        auto eValue = getElementValue(rootElement, "ObjectModifier");
        bool physicsOnly = false;
        if (eValue == "DestroyNode")
        {
            physicsOnly = true;
        }

        String typeName = getElementValue(rootElement, "BodyType");
        myLog->logMessage("Physics type: " + typeName, LML_NORMAL);
        OgreNewt::Body* body = nullptr;
        bodyUserData* userD = nullptr;

        if (typeName == "tree")
        {

            OgreNewt::CollisionPtr col = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision(mWorld, ent, false, 0));
            body = new OgreNewt::Body(mWorld, col);

            myLog->logMessage("matID je: "+ getElementValue(rootElement, "MaterialID", "??"),LML_NORMAL);

            userD = new bodyUserData();
            userD->material = getElementIntValue(rootElement, "MaterialID");
            body->setUserData(Ogre::Any(userD));

            body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());

            if (!physicsOnly)
                body->attachNode(node);

            auto type = getElementValue(rootElement, "Type");
            myLog->logMessage("Body gametype is: " + type, LML_NORMAL);

            int typeID = Ogre::StringConverter::parseInt(type);

            if (typeID == SelfIgnore)
                body->setMaterialGroupID(wMaterials->selfIgnore_mat);
            else if (typeID == PlayerIgnore)
                body->setMaterialGroupID(wMaterials->playerIgnore_mat);
            else
                body->setType(typeID);

            loadedBodies[ent->getName()] = body;

            loadActions(rootElement, mEventMgr, body, userD, wMaterials->actionMaker_mat);

        }
        else if (typeName == "comp" && compoundBodiesParts.find(node->getName()) != compoundBodiesParts.end())
        {

            OgreNewt::CollisionPtr myCol = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld, ent, 0, node->_getDerivedOrientation()));

            std::vector<OgreNewt::CollisionPtr> cols;
            cols.push_back(myCol);

            auto parts = compoundBodiesParts.find(node->getName());
            for (auto i = parts->second.begin(); i < parts->second.end(); i++)
            {
                CompoundBodyInfo info = (*i);

                if (info.physical)
                {
                    OgreNewt::CollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld, info.ent, 0, info.node->_getDerivedOrientation(), info.node->_getDerivedPosition() - node->_getDerivedPosition()));
                    cols.push_back(col);
                }

                if (info.visual)
                {
                    Ogre::SceneNode* childNode = info.node;
                    childNode->getParentSceneNode()->removeChild(childNode);
                    childNode->setPosition(childNode->getPosition() - node->getPosition());
                    childNode->setScale(childNode->getScale() / node->getScale());
                    node->addChild(childNode);
                }
                else
                {
                    info.node->detachAllObjects();
                    Global::mSceneMgr->destroySceneNode(info.node);
                    Global::mSceneMgr->destroyEntity(info.ent);
                }

            }

            OgreNewt::CollisionPtr col = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::CompoundCollision(mWorld, cols, 0));
            body = new OgreNewt::Body(mWorld, col);

            Ogre::Vector3 inertia, offset;
            OgreNewt::ConvexCollisionPtr convCol = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld, ent, 0));
            convCol->calculateInertialMatrix(inertia, offset);

            myLog->logMessage("matID je: " + getElementValue(rootElement, "MaterialID", "??"), LML_NORMAL);

            userD = new bodyUserData();
            userD->material = getElementIntValue(rootElement, "MaterialID");
            body->setUserData(Ogre::Any(userD));

            auto massStr = getElementValue(rootElement, "Mass");
            myLog->logMessage("mass je: " + massStr, LML_NORMAL);
            Real mass = Ogre::StringConverter::parseReal(massStr);
            body->setMassMatrix(mass, inertia);

            auto type = getElementValue(rootElement, "Type");
            myLog->logMessage("Body gametype is: " + type, LML_NORMAL);

            int typeID = Ogre::StringConverter::parseInt(type);

            if (typeID == SelfIgnore)
                body->setMaterialGroupID(wMaterials->selfIgnore_mat);
            else if (typeID == PlayerIgnore)
                body->setMaterialGroupID(wMaterials->playerIgnore_mat);
            else
                body->setType(typeID);

            if (typeID == Pullup_old || typeID == Climb || typeID == Climb_Pullup || typeID == Dynamic_Pullup)
                body->setMaterialGroupID(wMaterials->noCollide_mat);

            body->setCenterOfMass(offset);
            body->setLinearDamping(0.3);
            body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());
            body->attachNode(node);

            loadedBodies[ent->getName()] = body;

            loadActions(rootElement, mEventMgr, body, userD, wMaterials->actionMaker_mat);

            body->setStandardForceCallback();

        }
        else if (typeName == "conv")
        {
            OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld, ent, 0));

            body = new OgreNewt::Body(mWorld, col);
            Ogre::Vector3 inertia, offset;
            col->calculateInertialMatrix(inertia, offset);
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
            //no longer need the collision shape object
            delete col;
#endif

            myLog->logMessage("matID je: " + getElementValue(rootElement, "MaterialID", "??"), LML_NORMAL);

            userD = new bodyUserData();
            userD->material = getElementIntValue(rootElement, "MaterialID");
            body->setUserData(Ogre::Any(userD));

            auto massStr = getElementValue(rootElement, "Mass");
            myLog->logMessage("mass je: " + massStr, LML_NORMAL);
            Real mass = Ogre::StringConverter::parseReal(massStr);
            body->setMassMatrix(mass, inertia);

            auto type = getElementValue(rootElement, "Type");
            myLog->logMessage("Body gametype is: " + type, LML_NORMAL);

            int typeID = Ogre::StringConverter::parseInt(type);

            if (typeID == SelfIgnore)
                body->setMaterialGroupID(wMaterials->selfIgnore_mat);
            else if (typeID == PlayerIgnore)
                body->setMaterialGroupID(wMaterials->playerIgnore_mat);
            else
                body->setType(typeID);

            if (typeID == Pullup_old || typeID == Climb || typeID == Climb_Pullup || typeID == Dynamic_Pullup)
                body->setMaterialGroupID(wMaterials->noCollide_mat);

            body->setCenterOfMass(offset);
            body->setLinearDamping(0.3);
            body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());

            if (!physicsOnly)
                body->attachNode(node);

            loadedBodies[ent->getName()] = body;

            loadActions(rootElement, mEventMgr, body, userD, wMaterials->actionMaker_mat);

            body->setStandardForceCallback();

        }

        switch (userD->material)
        {
        case 6:
            userD->sounds = Global::audioLib->dynamicsAudio["Wood"];
            break;
        case 7:
            userD->sounds = Global::audioLib->dynamicsAudio["Metal"];
            break;
        case 8:
            userD->sounds = Global::audioLib->dynamicsAudio["Bush"];
            break;
        }

        if (body && body->getMass() == 0)
            addOptimizableEntity(ent);

        return body;
    }

    void loadJoint(const XMLElement* element, SceneNode* node, Ogre::SceneManager *mSceneMgr)
    {
        const XMLElement* pElement = element->NextSiblingElement();

        auto children = getElementText(element);
        auto parents = getElementText(pElement);

        auto child = MUtils::strtok_str(children,';');
        element = element->NextSiblingElement();
        auto parent = MUtils::strtok_str(parents, ';');

        const XMLElement* typeElement = pElement->NextSiblingElement();
        char type = 0;
        if (typeElement != 0)
        {
            type = Ogre::StringConverter::parseInt(typeElement->GetText());
        }

        while (!child.empty() && !parent.empty())
        {
            LoadedJointInfo info;

            info.body0 = Ogre::String(child);
            info.body1 = Ogre::String(parent);
            info.position = node->getPosition();
            info.pin = node->_getDerivedOrientation()*Ogre::Vector3(1, 0, 0);
            info.type = type;

            loadedJoints.push_back(info);

            //next
            child = MUtils::strtok_str(children, ';');
            parent = MUtils::strtok_str(parents, ';');

            Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Joint - child:" + info.body0 + " and parent:" + info.body1, LML_NORMAL);
        }

        node->detachAllObjects();
        mSceneMgr->destroySceneNode(node);

    }

    void loadBridge(const XMLElement* element, SceneNode* node, Ogre::SceneManager *mSceneMgr, uint32 visibilityFlag)
    {
        Ogre::Vector3 pos = node->getPosition();
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Making BM", LML_NORMAL);
        Ogre::Vector3 target;
        auto posText = getElementText(element);
        auto val = MUtils::strtok_str(posText, ',');
        target.x = Ogre::StringConverter::parseReal(val);
        val = MUtils::strtok_str(posText, ',');
        target.z = -Ogre::StringConverter::parseReal(val);
        val = MUtils::strtok_str(posText, ',');
        target.y = Ogre::StringConverter::parseReal(val);
        element = element->NextSiblingElement();
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Making BM", LML_NORMAL);
        float loose = Ogre::StringConverter::parseReal(element->GetText());
        element = element->NextSiblingElement();
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Making BM", LML_NORMAL);
        float scale = Ogre::StringConverter::parseReal(element->GetText());
        element = element->NextSiblingElement();
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Making BM", LML_NORMAL);

        Ogre::String mat;
        if (element->GetText() != NULL)
            mat = Ogre::String(element->GetText());

        element = element->NextSiblingElement();
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Making BM", LML_NORMAL);
        int matType = Ogre::StringConverter::parseInt(element->GetText());

        static BridgeMaker bm(mSceneMgr, Global::mWorld);

        if (mat.empty())
            bm.makeBridge(pos, target, scale, loose, matType, visibilityFlag);
        else
            bm.makeBridge(pos, target, scale, loose, matType, visibilityFlag, mat);

        node->detachAllObjects();
        mSceneMgr->destroySceneNode(node);

        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Bridge - to: " + Ogre::StringConverter::toString(target) + " and loose: " + Ogre::StringConverter::toString(loose) + " and scale: " + Ogre::StringConverter::toString(scale), LML_NORMAL);
    }

    void loadSound(const XMLElement* element, SceneNode* node, Ogre::SceneManager *mSceneMgr)
    {
        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
        const XMLElement* childElement = element;

        float volume = Ogre::StringConverter::parseReal(childElement->GetText());
        childElement = childElement->NextSiblingElement();
        bool loop = Ogre::StringConverter::parseBool(childElement->GetText());
        childElement = childElement->NextSiblingElement();
        float min = Ogre::StringConverter::parseReal(childElement->GetText());
        childElement = childElement->NextSiblingElement();
        Ogre::String soundName = Global::audioLib->getPath(Ogre::String(childElement->GetText()));

        irrklang::ISound* sound = Global::soundEngine->play3D(soundName.c_str(), irrklang::vec3df(node->getPosition().x, node->getPosition().y, node->getPosition().z), loop, false, true, irrklang::ESM_AUTO_DETECT, false);
        sound->setMinDistance(min);
        sound->setVolume(volume);

        (*Global::globalData)[childElement->GetText()] = sound;

        //sound->getSoundEffectControl()->enableWavesReverbSoundEffect(0,-10*timestep,2600,0.5);

        node->detachAllObjects();
        mSceneMgr->destroySceneNode(node);

        myLog->logMessage("Sound - " + soundName, LML_NORMAL);
    }

    void loadCompoundBodyPart(const XMLElement* element, Entity* ent, SceneNode* node, OgreNewt::World* mWorld, Ogre::SceneManager *mSceneMgr)
    {
        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
        Ogre::String compBody = Ogre::String(element->GetText());
        element = element->NextSiblingElement();
        bool visual = getBool(element->GetText());
        element = element->NextSiblingElement();
        bool physical = getBool(element->GetText());

        myLog->logMessage("Comp - " + compBody, LML_NORMAL);

        auto parts = compoundBodiesParts.find(compBody);

        if (parts == compoundBodiesParts.end())
        {
            std::vector<CompoundBodyInfo> infos;
            CompoundBodyInfo info;
            info.ent = ent;
            info.node = node;
            info.physical = physical;
            info.visual = visual;

            infos.push_back(info);
            compoundBodiesParts[compBody] = infos;
        }
        else
        {
            CompoundBodyInfo info;
            info.ent = ent;
            info.node = node;
            info.physical = physical;
            info.visual = visual;

            parts->second.push_back(info);
        }

    }

    TriggerInfo* loadTrigger(const XMLElement* element, bool nonphysical = false)
    {

        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");

        TriggerAndIDs trig;

        std::string actions = getElementText(element);

        if (!actions.empty())
        {
            int i = 0;
            auto aID = MUtils::strtok_str(actions, ';');
            while (!aID.empty() && i < 4)
            {
                //add action
                myLog->logMessage("Trigger ID " + Ogre::String(aID), LML_NORMAL);
                trig.ids.push_back(stoi(aID));
                i++;
                aID = MUtils::strtok_str(actions, ';');
            }
        }

        if (nonphysical)
        {
            TriggerInfo* trigger = new TriggerInfo();
            trig.trigger = trigger;
            loadedTriggers.push_back(trig);
            return trigger;
        }

        element = element->NextSiblingElement();
        //PLAYER ID
        short playerID = (short)Ogre::StringConverter::parseInt(element->GetText());

        element = element->NextSiblingElement();
        //touch=1,activation=0
        int activation = Ogre::StringConverter::parseInt(element->GetText());

        if (playerID)
            myLog->logMessage("Trigger with player ID " + Ogre::StringConverter::toString(playerID) + " of type " + Ogre::StringConverter::toString(activation) + " (touch=1,activation=0)", LML_NORMAL);

        element = element->NextSiblingElement();
        float cooldown = Ogre::StringConverter::parseReal(element->GetText());

        TriggerInfo* trigger = new TriggerInfo(playerID, cooldown);
        trig.trigger = trigger;

        trigger->activationType = activation;

        loadedTriggers.push_back(trig);

        return trigger;
    }

    void loadActions(const XMLElement* element, void* data, int id = 0)
    {
        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");

        const XMLElement* fElement = element->NextSiblingElement();

        std::string tasks = getElementText(element);
        std::string functions = getElementText(fElement);

        if (functions.empty())
            return;

        std::string aID;
        std::string fID;

        bool newForm = false;
        if (tasks.empty())
        {
            fID = MUtils::strtok_str(functions, '\n');
            newForm = true;
        }
        else
        {
            aID = MUtils::strtok_str(tasks, ';');
            fID = MUtils::strtok_str(functions, ';');
        }

        while (!fID.empty() && (!aID.empty() || newForm))
        {
            Ogre::String rName(fID);
            Ogre::String action(rName);

            if (newForm)
            {
                int ind = action.find_first_of(':');
                action.erase(ind, action.length() - ind);
                rName.erase(0, ind + 1);

                while (rName.at(0) == ' ')
                    rName.erase(0, 1);
            }
            else
            {
                action = Ogre::String(aID);
            }

            //add task
            myLog->logMessage("Task ID " + action + " with function " + rName, LML_NORMAL);

            float delay = getTaskDelay(rName);

            EventTask* r = makeTask(rName);

            if (r != NULL)
            {
                EventTaskAndID er;
                er.task = r;
                r->taskDelay = delay;
                r->setUserData(data);
                //r->setIdentifier(id);
                er.id = Ogre::StringConverter::parseInt(action);
                loadedTasks.push_back(er);
            }
            else
                myLog->logMessage("WARNING: No such function available", LML_NORMAL);

            //next
            if (newForm)
            {
                fID = MUtils::strtok_str(functions, '\n');
            }
            else
            {
                aID = MUtils::strtok_str(tasks, ';');
                fID = MUtils::strtok_str(functions, ';');
            }

        }

    }

    int setModifierStart(const XMLElement* rootElement, Entity** ent, SceneNode** node, Ogre::SceneManager *mSceneMgr)
    {
        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");

        int id = 0;

        auto element = rootElement->FirstChildElement("Identifier");
        if (element != NULL)
        {
            String idName = element->GetText();
            id = Ogre::StringConverter::parseInt(idName);
        }

        element = rootElement->FirstChildElement("ObjectModifier");
        if (element != NULL)
        {
            String elementName = element->GetText();
            myLog->logMessage("Modifier start, type: " + elementName + " with id " + Ogre::StringConverter::toString(id), LML_NORMAL);

            if (elementName == "Regular")
                return id;

            if (elementName == "Hand")
            {
                (*node)->detachObject(*ent);
                Ogre::uint32 vFlags = (*ent)->getVisibilityFlags();
                mSceneMgr->destroyEntity(*ent);
                (*ent) = mSceneMgr->createEntity("Ruka" + Ogre::StringConverter::toString(id), "ruka.mesh");
                (*ent)->setVisibilityFlags(vFlags);
                (*node)->attachObject(*ent);
                (*node)->scale(0.1, 0.1, 0.1);
            }
        }
        return id;
    }

    void setModifierEnd(const XMLElement* rootElement, Entity** ent, SceneNode** node, Ogre::SceneManager *mSceneMgr)
    {
        auto element = rootElement->FirstChildElement("ObjectModifier");
        if (element != NULL)
        {
            String elementName = element->GetText();

            //Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Modifier end, type: "+elementName);

            if (elementName == "DestroyEntity")
            {
                (*node)->detachObject(*ent);
                mSceneMgr->destroyEntity(*ent);
            }
            else if (elementName == "DestroyNode")
            {
                (*node)->detachAllObjects();
                mSceneMgr->destroyEntity(*ent);
                mSceneMgr->destroySceneNode(*node);
            }
        }
    }

    void loadEntity(const XMLElement* entityElement, SceneNode* node, bool visible, Ogre::SceneManager *mSceneMgr, OgreNewt::World* mWorld,
                    EventsManager* mEventMgr, WorldMaterials* wMaterials)
    {

        Entity* ent = mSceneMgr->createEntity(entityElement->Attribute("name"), entityElement->Attribute("meshFile"));
        node->attachObject(ent);
        ent->setVisible(visible);

        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Entity " + ent->getName(), LML_NORMAL);


        String str = GetStringAttribute(entityElement, "castShadows");
        if (!str.empty() && !getBool(str)) ent->setCastShadows(false);
        else ent->setCastShadows(true);

        String str2 = GetStringAttribute(entityElement, "visibilityFlags");
        if (!str2.empty())
        {
            ent->setVisibilityFlags(Ogre::StringConverter::parseLong(str2));
        }
        else
        {
            ent->setVisibilityFlags(1);
        }

        Ogre::uint8 renderQueue = ParseRenderQueue(GetStringAttribute(entityElement, "renderQueue"));
        ent->setRenderQueueGroup(renderQueue);

        bool waterFlag = false;
        const XMLElement* subentityElement = entityElement->FirstChildElement("subentities");
        const XMLElement* childElement = 0;
        while (childElement = IterateChildElements(subentityElement, childElement))
        {
            int index = GetIntAttribute(childElement, "index", 0);
            ent->getSubEntity(index)->setMaterialName(GetStringAttribute(childElement, "materialName"));

            //update dynamic materials
            ent->getSubEntity(index)->setMaterial(getCorrectMaterial(ent->getSubEntity(index)->getMaterial(), node));

            auto matPtr = ent->getSubEntity(index)->getMaterial();

            for (int i = 0; i < matPtr->getNumTechniques() && !waterFlag; i++)
            {
                waterFlag = (matPtr->getTechnique(i)->getSchemeName() == "WaterDepth");
            }
        }

        if (waterFlag)
            ent->setVisibilityFlags(ent->getVisibilityFlags() | VisibilityFlag_Water);

        const XMLElement* userdataElement = entityElement->FirstChildElement("userData");

        if (userdataElement != NULL)
        {

            Ogre::String userData;
            GetChildText(userdataElement, userData);

            Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
            myLog->logMessage(userData, LML_NORMAL);

            XMLDocument document;
            document.Parse(userData.c_str());
            if (!document.Error())
            {
                XMLElement *root = document.RootElement();

                if (root->Value() != NULL)
                {
                    Ogre::String rootTag(root->Value());
                    auto element = root->FirstChildElement();

                    if (rootTag == "PhysicalBody")
                    {
                        setModifierStart(root, &ent, &node, mSceneMgr);

                        loadPhysics(root, ent, node, mWorld, mEventMgr, wMaterials);

                        setModifierEnd(root, &ent, &node, mSceneMgr);
                    }
                    else if (rootTag == "Particle")
                    {
                        loadParticle(root, ent, node);
                    }
                    else if (rootTag == "CrowFlight")
                    {
                        loadCrowFlight(root, ent, node);
                    }
                    else if (rootTag == "CrowLanding")
                    {
                        loadCrowLanding(root, ent, node);
                    }
                    else if (rootTag == "EnvSoundBody")
                    {
                        //loadEnvSound(element, node, mSceneMgr, env);
                        mSceneMgr->destroyEntity(ent);
                    }
                    else if (rootTag == "SoundBody")
                    {
                        loadSound(element, node, mSceneMgr);
                        mSceneMgr->destroyEntity(ent);
                    }
                    else if (rootTag == "RigidJoint")
                    {
                        loadJoint(element, node, mSceneMgr);
                        mSceneMgr->destroyEntity(ent);
                    }
                    else if (rootTag == "RigidBridge")
                    {
                        loadBridge(element, node, mSceneMgr, ent->getVisibilityFlags());
                    }
                    else if (rootTag == "GrassArea")
                    {
                        loadGrassArea(element, ent, node, mSceneMgr);
                    }
                    else if (rootTag == "DetailGeometryMask")
                    {
                        loadDetailGeometryMask(root, ent);
                    }
                    else if (rootTag == "DetailGeometry")
                    {
                        loadDetailGeometry(root, ent);
                    }
                    else if (rootTag == "Billboard")
                    {
                        loadBillboard(element, ent, mSceneMgr);
                    }
                    else if (rootTag == "Instanced")
                    {
                        loadInstance(element, ent, node, mSceneMgr);
                    }
                    else if (rootTag == "Reflection")
                    {
                        loadReflection(element, ent, node, mSceneMgr);
                    }
                    else if (rootTag == "SceneCubeMap")
                    {
                        loadSceneCubeMap(root, ent, node);
                    }
                    else if (rootTag == "SlideTrack")
                    {
                        loadSlideTrack(root, ent, node, mSceneMgr);
                    }
                    else if (rootTag == "WaterCurrent")
                    {
                        loadWaterCurrent(root, ent, node, mSceneMgr);
                    }
                    else if (rootTag == "PhysicalBodyTrigger")
                    {
                        setModifierStart(root, &ent, &node, mSceneMgr);

                        OgreNewt::Body* body = loadPhysics(root, ent, node, mWorld, mEventMgr, wMaterials);
                        element = element->NextSiblingElement("triggerids");
                        TriggerInfo* trigger = loadTrigger(element);

                        Ogre::Any any = body->getUserData();
                        if (!any.isEmpty())
                        {
                            any_cast<bodyUserData*>(any)->trigger = trigger;

                            if (!trigger->activationType) body->setType(Trigger);

                            if (element->GetText() || (trigger->playerAction && trigger->activationType))
                                body->setMaterialGroupID(wMaterials->trig_mat);

                            element = element->NextSiblingElement("onstart");
                            bool onStart = Ogre::StringConverter::parseBool(element->GetText());
                            any_cast<bodyUserData*>(any)->enabledTrigger = onStart;

                        }

                        setModifierEnd(root, &ent, &node, mSceneMgr);

                    }
                    else if (rootTag == "PhysicalBodyTriggerReaction")
                    {
                        int id = setModifierStart(root, &ent, &node, mSceneMgr);

                        OgreNewt::Body* body = loadPhysics(root, ent, node, mWorld, mEventMgr, wMaterials);

                        element = element->NextSiblingElement("reactionids");
                        //parse tasks+functions
                        loadActions(element, body, id);

                        element = element->NextSiblingElement("triggerids");
                        TriggerInfo* trigger = loadTrigger(element);

                        Ogre::Any any = body->getUserData();
                        if (!any.isEmpty())
                        {
                            any_cast<bodyUserData*>(any)->trigger = trigger;

                            if (!trigger->activationType) body->setType(Trigger);

                            if (element->GetText() || (trigger->playerAction && trigger->activationType))
                                body->setMaterialGroupID(wMaterials->trig_mat);

                            element = element->NextSiblingElement("onstart");
                            bool onStart = Ogre::StringConverter::parseBool(element->GetText());
                            any_cast<bodyUserData*>(any)->enabledTrigger = onStart;

                        }

                        setModifierEnd(root, &ent, &node, mSceneMgr);
                    }
                    else if (rootTag == "PhysicalBodyReaction")
                    {
                        XMLElement* modifElement = element;
                        int id = setModifierStart(root, &ent, &node, mSceneMgr);

                        OgreNewt::Body* body = loadPhysics(root, ent, node, mWorld, mEventMgr, wMaterials);
                        element = element->NextSiblingElement("reactionids");

                        //parse tasks+functions
                        loadActions(element, body, id);

                        setModifierEnd(root, &ent, &node, mSceneMgr);
                    }
                    else if (rootTag == "TriggerReaction")
                    {
                        int id = setModifierStart(root, &ent, &node, mSceneMgr);
                        element = element->NextSiblingElement()->NextSiblingElement();

                        if (element->GetText() != 0)
                        {

                            Ogre::String rName = Ogre::String(element->GetText());
                            float delay = getTaskDelay(rName);

                            EventTask* r = makeTask(rName);

                            if (r != NULL)
                            {
                                r->setUserData(ent);
                                //r->setIdentifier(id);
                                r->taskDelay = delay;

                                mEventMgr->addTask(r);
                            }
                        }

                        element = element->NextSiblingElement();

                        //parse tasks+functions
                        loadActions(element, ent, id);

                        element = element->NextSiblingElement("triggerids");
                        TriggerInfo* trigger = loadTrigger(element, true);

                        ent->setUserAny(Ogre::Any(trigger));

                        setModifierEnd(root, &ent, &node, mSceneMgr);
                    }
                    else if (rootTag == "Reaction")
                    {
                        int id = setModifierStart(root, &ent, &node, mSceneMgr);
                        element = element->NextSiblingElement()->NextSiblingElement();

                        if (element->GetText() != 0)
                        {
                            Ogre::String rName = Ogre::String(element->GetText());
                            float delay = getTaskDelay(rName);

                            EventTask* r = makeTask(rName);

                            if (r != NULL)
                            {
                                r->setUserData(ent);
                                //r->setIdentifier(id);
                                r->taskDelay = delay;

                                mEventMgr->addTask(r);
                            }
                        }

                        element = element->NextSiblingElement();

                        //parse tasks+functions
                        loadActions(element, ent, id);

                        setModifierEnd(root, &ent, &node, mSceneMgr);
                    }
                    else if (rootTag == "CompoundBodyPart")
                    {
                        loadCompoundBodyPart(element, ent, node, mWorld, mSceneMgr);

                    }

                }
                else
                    addOptimizableEntity(ent);
            }
        }

    }


    void loadPlayerInfo(const XMLElement* nodeElement, Player* pl)
    {
        String elementName;
        const XMLElement* childElement = 0;
        while (childElement = IterateChildElements(nodeElement, childElement))
        {
            elementName = childElement->Value();

            if (elementName == "position")
                pl->setPosition(LoadXYZ(childElement));
            else if (elementName == "rotation")
            {
                Quaternion q = LoadRotation(childElement);
                pl->rotateCamera(Ogre::Degree(q.getYaw()).valueDegrees(), -Ogre::Degree(q.getPitch()).valueDegrees());
            }
            else if (elementName == "entity")
            {
                const XMLElement* userdataElement = childElement->FirstChildElement("userData");

                if (userdataElement != NULL)
                {
                    Ogre::String userData;
                    GetChildText(userdataElement, userData);

                    Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
                    myLog->logMessage(userData, LML_NORMAL);

                    XMLDocument document;
                    document.Parse(userData.c_str());
                    if (!document.Error())
                    {
                        XMLElement *root = document.RootElement();

                        if (root->Value() != NULL)
                        {
                            Ogre::String rootTag(root->Value());
                            XMLElement *element = root->FirstChildElement();

                            if (rootTag == "Player")
                            {
                                loadActions(element, pl->body);
                            }
                        }
                    }
                }

            }
        }
    }

    inline void fixSpline(Quaternion& rotation, Quaternion previous)
    {
        float fCos = previous.Dot(rotation);
        if (fCos < 0.0f)
            rotation = -rotation;
    }

    void loadAnimations(const XMLElement* element, Ogre::SceneNode* node, Ogre::SceneManager* mSceneMgr)
    {

        node = mSceneMgr->getRootSceneNode()->createChildSceneNode(node->getName() + "Anim", node->_getDerivedPosition(), node->_getDerivedOrientation());

        const XMLElement* childElement = 0;
        while (childElement = IterateChildElements(element, childElement))
        {

            Ogre::String animName = childElement->Attribute("name");
            bool enabled = getBool(childElement->Attribute("enable"));
            bool loop = getBool(childElement->Attribute("loop"));
            Ogre::String interpMode = childElement->Attribute("interpolationMode");
            Ogre::String rotInterpMode = childElement->Attribute("rotationInterpolationMode");
            float length = GetRealAttribute(childElement, "length");

            Animation* anim = mSceneMgr->createAnimation(animName, length);

            if (interpMode == "spline")
                anim->setInterpolationMode(Animation::IM_SPLINE);
            else
                anim->setInterpolationMode(Animation::IM_LINEAR);

            if (rotInterpMode == "spherical")
                anim->setRotationInterpolationMode(Animation::RIM_SPHERICAL);
            else
                anim->setRotationInterpolationMode(Animation::RIM_LINEAR);

            NodeAnimationTrack* track = anim->createNodeTrack(0, node);
            Quaternion previous;

            const XMLElement* keyElement = 0;
            while (keyElement = IterateChildElements(childElement, keyElement))
            {
                float time = GetRealAttribute(keyElement, "time");
                Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(time);

                const XMLElement* keyInfoElement = keyElement->FirstChildElement();
                kf->setTranslate(LoadXYZ(keyInfoElement));
                keyInfoElement = keyInfoElement->NextSiblingElement();
                Quaternion rot = LoadRotation(keyInfoElement);

                fixSpline(rot, previous);

                kf->setRotation(rot);

                previous = rot;

                keyInfoElement = keyInfoElement->NextSiblingElement();
                kf->setScale(LoadXYZ(keyInfoElement));
            }

            Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Loaded node animation " + animName + " with " + Ogre::StringConverter::toString(track->getNumKeyFrames()) + " keyframes", LML_NORMAL);;
        }

    }

    void loadNode(const XMLElement* nodeElement)
    {

        SceneNode* node;
        Ogre::String name;
        name = nodeElement->Attribute("name");

        if (name == "Player" &&  Global::player != NULL)
        {
            loadPlayerInfo(nodeElement, Global::player);
        }
        else
        {

            Ogre::SceneManager *mSceneMgr = Global::mSceneMgr;
            OgreNewt::World* mWorld = Global::mWorld;
            EventsManager* mEventMgr = Global::mEventsMgr;
            WorldMaterials* wMaterials = Global::gameMgr->wMaterials;

            node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);

            bool visible = true;
            if (nodeElement->Attribute("visibility") != NULL)
            {
                Ogre::String v = nodeElement->Attribute("visibility");

                if (v == "hidden") visible = false;
            }


            //first properties, then content
            String elementName;
            const XMLElement* childElement = 0;


            while (childElement = IterateChildElements(nodeElement, childElement))
            {
                elementName = childElement->Value();

                if (elementName == "position")
                    node->setPosition(LoadXYZ(childElement));
                else if (elementName == "rotation")
                    node->setOrientation(LoadRotation(childElement));
                else if (elementName == "scale")
                    node->setScale(LoadXYZ(childElement));
                else if (elementName == "animations")
                    loadAnimations(childElement, node, mSceneMgr);
            }


            childElement = 0;

            while (childElement = IterateChildElements(nodeElement, childElement))
            {
                elementName = childElement->Value();

                if (elementName == "entity")
                    loadEntity(childElement, node, visible, mSceneMgr, mWorld, mEventMgr, wMaterials);
                else if (elementName == "light")
                    loadLight(childElement, node, mSceneMgr);
                else if (elementName == "plane")
                    loadPlane(childElement, node, mSceneMgr);
            }

        }
    }

    Ogre::MaterialPtr getCorrectMaterial(Ogre::MaterialPtr mat, Ogre::SceneNode* node)
    {
        auto retMat = mat;

        if (mat->getTechnique(0)->getNumPasses() == 2)
        {
            auto mainPass = mat->getTechnique(0)->getPass(1);

            //scene cubemap
            Ogre::TextureUnitState* t = mainPass->getTextureUnitState("envCubeMap");
            if (t)
            {
                retMat = SceneCubeMap::applyCubemap(mat, node->getPosition());
            }
        }

        return retMat;
    }

    void connectJoints()
    {

        for (std::vector<LoadedJointInfo>::iterator jointIt = loadedJoints.begin(); jointIt != loadedJoints.end(); jointIt++)
        {
            Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("\nConnecting joints - joint body 0 " + (*jointIt).body0 + " and " + (*jointIt).body1 + " with joint type " + Ogre::StringConverter::toString((*jointIt).type), Ogre::LML_NORMAL);
            OgreNewt::Body* b0, *b1;
            std::map<Ogre::String, OgreNewt::Body*>::iterator it;

            it = loadedBodies.find((*jointIt).body0);
            if (it == loadedBodies.end())
            {
                Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("\nWARNING - joint body 0 " + (*jointIt).body0 + " not found!", Ogre::LML_NORMAL);
                continue;
            }
            b0 = it->second;

            if ((*jointIt).body1 == "NULL")
                b1 = NULL;
            else
            {
                it = loadedBodies.find((*jointIt).body1);
                if (it == loadedBodies.end())
                {
                    Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("\nWARNING - joint body 1 " + (*jointIt).body0 + " not found!", Ogre::LML_NORMAL);
                    continue;
                }
                b1 = it->second;
            }

            Ogre::Vector3 pos = (*jointIt).position;
            Ogre::Vector3 pin = (*jointIt).pin;
            char type = (*jointIt).type;

            if (type == 0)
            {
                OgreNewt::BallAndSocket* j = new OgreNewt::BallAndSocket(b0, b1, pos, 0);

                if ((*jointIt).body1 != "NULL")
                    j->setCollisionState(0);

                j->setStiffness(100);
                j->setRowStiffness(100);
            }
            else
            {
                OgreNewt::Hinge* j = new OgreNewt::Hinge(b0, b1, pos, pin);

                if ((*jointIt).body1 != "NULL")
                    j->setCollisionState(0);

                j->setStiffness(100);
                j->setRowStiffness(100);

                b0->setAngularDamping(Vector3(0.0));
                b0->setLinearDamping(0);

                if (b1 != NULL)
                    b1->setLinearDamping(0);
            }
        }

        loadedJoints.clear();
    }

    void summarizeLevelEvents()
    {
        TriggerAndIDs tri;
        EventTaskAndID eri;
        int countAll = 0;

        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("BEGGINING OF TRIGGERS ::", LML_NORMAL);

        //for all triggers
        for (unsigned int i = 0; i < loadedTriggers.size(); i++)
        {
            tri = loadedTriggers.at(i);

            //initialize empty tasks vectors
            for (unsigned int j = 0; j < tri.ids.size(); j++)
            {
                std::vector<EventTask*> tasks;
                tri.trigger->tasks[tri.ids.at(j)] = tasks;
            }

            int count = 0;

            //iterate through all tasks
            for (unsigned int j = 0; j < loadedTasks.size(); j++)
            {
                eri = loadedTasks.at(j);

                if (std::count(tri.ids.begin(), tri.ids.end(), eri.id) > 0)
                {
                    Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Connecting task with ID " + Ogre::StringConverter::toString(eri.id), LML_NORMAL);
                    tri.trigger->tasks[eri.id].push_back(eri.task);
                    count++;
                }

                if (tri.trigger->playerAction == eri.id)
                {
                    Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Connecting task on Player with ID " + Ogre::StringConverter::toString(eri.id), LML_NORMAL);
                    tri.trigger->tasks[eri.id].push_back(eri.task);
                    count++;
                }
            }

            Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Tasks connected " + Ogre::StringConverter::toString(count) + " times", LML_NORMAL);
            countAll += count;
        }

        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("END OF TRIGGERS :: Totally created " + Ogre::StringConverter::toString(loadedTriggers.size()) + " triggers and " + Ogre::StringConverter::toString(loadedTasks.size()) + " tasks which were connected " + Ogre::StringConverter::toString(countAll) + " times", LML_NORMAL);

        loadedTasks.clear();
        loadedTriggers.clear();

        for (auto g : loadedGrassAreas)
        {
            auto b = loadedBodies[g.targetBody];
            g.data->bodyTarget = b;
        }

        loadedGrassAreas.clear();
    }

    bool isCompoundBody(const XMLElement* nodeElement)
    {
        const XMLElement* childElement = nodeElement->FirstChildElement("entity");
        if (childElement != NULL)
        {
            const XMLElement* userdataElement = childElement->FirstChildElement("userData");

            if (userdataElement != NULL)
            {
                Ogre::String userData;
                GetChildText(userdataElement, userData);

                XMLDocument document;
                document.Parse(userData.c_str());
                if (!document.Error())
                {
                    XMLElement *root = document.RootElement();

                    if (root->Value() != NULL)
                    {
                        Ogre::String rootTag(root->Value());
                        if (rootTag.size() > 12)
                            rootTag.erase(12, rootTag.size() - 12);

                        if (rootTag == "PhysicalBody")
                        {
                            XMLElement *element = root->FirstChildElement("BodyType");
                            Ogre::String phTag(element->GetText());

                            if (phTag == "comp")
                                return true;
                        }
                    }
                }
            }
        }

        return false;
    }


    std::string getUserDataType(const XMLElement* nodeElement)
    {
        const XMLElement* childElement = nodeElement->FirstChildElement("entity");
        if (childElement != NULL)
        {
            const XMLElement* userdataElement = childElement->FirstChildElement("userData");

            if (userdataElement != NULL)
            {
                Ogre::String userData;
                GetChildText(userdataElement, userData);

                XMLDocument document;
                document.Parse(userData.c_str());
                if (!document.Error())
                {
                    XMLElement *root = document.RootElement();

                    if (root->Value() != NULL)
                    {
                        Ogre::String rootTag(root->Value());
                        return rootTag;
                    }
                }
            }
        }

        return "";
    }

    void parseSceneSettings(const XMLElement* sceneElement)
    {
        Ogre::String userData;
        GetChildText(sceneElement, userData);

        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
        myLog->logMessage(userData, LML_NORMAL);

        XMLDocument document;
        document.Parse(userData.c_str());

        if (!document.Error())
        {
            XMLElement *root = document.RootElement();

            if (root->Value() != NULL)
            {
                Ogre::String rootTag(root->Value());

                if (rootTag == "SceneSettings")
                {
                    auto ambient = getElementV3Value(root, "AmbientColor");
                    auto fogColor = getElementV3Value(root, "FogColor");
                    auto fogW = getElementFloatValue(root, "FogWeight");
                    auto fogStart = getElementFloatValue(root, "FogStart");
                    auto fogEnd = getElementFloatValue(root, "FogEnd");
                    auto skybox = getElementValue(root, "Skybox");

                    auto lvl = Global::gameMgr->getCurrentLvlInfo();
                    lvl->ambientColor = Ogre::ColourValue(ambient.x, ambient.y, ambient.z);
                    lvl->fogColor = Ogre::ColourValue(fogColor.x, fogColor.y, fogColor.z, fogW);
                    lvl->fogStartDistance = fogStart;
                    lvl->fogEndDistance = fogEnd;
                    lvl->skyboxName = skybox;
                }
            }
        }
    }

public:

    void reloadScene(Ogre::String filename)
    {
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("RELOADING SCENE :: filename \"" + filename + "\"", LML_NORMAL);

        Global::gameMgr->geometryMgr->resetMaskedDetailGeometries();

        XMLDocument document;
        document.LoadFile(filename.c_str());
        auto mainElement = document.RootElement();
        auto nodesElement = mainElement->FirstChildElement("nodes");
        auto sceneElement = mainElement->FirstChildElement("userData");

        if (sceneElement)
            parseSceneSettings(sceneElement);

        Global::gameMgr->reloadSceneSettings();

        String elementName;
        const XMLElement* childElement = 0;
        while (childElement = IterateChildElements(nodesElement, childElement))
        {
            elementName = childElement->Value();

            if (elementName == "node")
            {
                auto type = getUserDataType(childElement);

                if (type == "DetailGeometryMask")
                    loadNode(childElement);
            }
        }

        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("RELOADING COMPLETED :: filename \"" + filename + "\"", LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("-----------------------------------------------------------", LML_NORMAL);
    }

    bool isInPreloadPass(std::string type)
    {
        return (type == "SceneCubeMap");
    }

    void loadScene(Ogre::String filename)
    {
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("LOADING SCENE :: filename \"" + filename + "\"", LML_NORMAL);

        XMLDocument document;
        document.LoadFile(filename.c_str());
        auto mainElement = document.RootElement();
        auto nodesElement = mainElement->FirstChildElement("nodes");
        auto sceneElement = mainElement->FirstChildElement("userData");

        loadedTasks.clear();
        loadedTriggers.clear();
        loadedJoints.clear();
        loadedBodies.clear();
        loadedBillboardSets.clear();
        loadedForests.clear();
        loadedSlides.clear();

        Global::gameMgr->loadedBodies = &loadedBodies;

        if (Global::player != NULL)
            loadedBodies["Player"] = Global::player->body;

        if (sceneElement)
            parseSceneSettings(sceneElement);

        Global::gameMgr->reloadSceneSettings();

        std::vector<const XMLElement*> compBodies;
        String elementName;

        //preload pass
        const XMLElement* childElement = 0;
        while (childElement = IterateChildElements(nodesElement, childElement))
        {
            elementName = childElement->Value();

            if (elementName == "node")
            {
                auto type = getUserDataType(childElement);

                if (isInPreloadPass(type))
                    loadNode(childElement);
            }
        }

        childElement = 0;
        while (childElement = IterateChildElements(nodesElement, childElement))
        {
            elementName = childElement->Value();

            if (elementName == "node")
            {
                auto type = getUserDataType(childElement);
                if (isInPreloadPass(type))
                    continue;

                if (!isCompoundBody(childElement))
                    loadNode(childElement);
                else
                    compBodies.push_back(childElement);
            }
            /*else if (elementName == "skyBox")
            	LoadSkyBox(childElement);*/
        }

        for (uint i = 0; i < compBodies.size(); i++)
        {
            loadNode(compBodies.at(i));
        }
        compBodies.clear();

        connectJoints();
        summarizeLevelEvents();
        compoundBodiesParts.clear();
        loadedForests.clear();

        optimizeEntities();
        Global::gameMgr->geometryMgr->postLoad();

        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("LOADING COMPLETED :: filename \"" + filename + "\"", LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("-----------------------------------------------------------", LML_NORMAL);
    }


};