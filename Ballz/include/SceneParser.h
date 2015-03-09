#pragma once

#include "stdafx.h"
#include <tinyxml.h>
#include "EnvironmentSound.h"
#include "GrassHeightFunction.h"
#include "BridgeMaker.h"
#include "Tasks.h"
#include "ReflectionTask.h"
#include "ZipLine.h"


using namespace Ogre;

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

class SceneParser
{
public:
    static SceneParser instance;

private:

    std::vector<TriggerAndIDs> loadedTriggers;
    std::vector<EventTaskAndID> loadedTasks;
    std::vector<LoadedJointInfo> loadedJoints;
    std::map<Ogre::String, OgreNewt::Body*> loadedBodies;
    std::map<int, Ogre::BillboardSet*> loadedBillboardSets;
    std::map<Ogre::String, std::vector<CompoundBodyInfo>> compoundBodiesParts;
    std::vector<LoadedInstanceForests> loadedForests;


    String getElementValue(const TiXmlElement* rootElement, String elementName, String defaultValue = "")
    {
        auto e = rootElement->FirstChildElement(elementName.c_str());

        if (e == nullptr || e->GetText() == nullptr)
            return defaultValue;
        else
            return e->GetText();
    }

    int getElementIntValue(const TiXmlElement* rootElement, String elementName, int defaultValue = 0)
    {
        auto e = rootElement->FirstChildElement(elementName.c_str());

        if (e == nullptr || e->GetText() == nullptr)
            return defaultValue;
        else
            return Ogre::StringConverter::parseInt(String(e->GetText()), defaultValue);
    }

    const TiXmlElement* IterateChildElements(const TiXmlElement* xmlElement, const TiXmlElement* childElement)
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

    Real GetRealAttribute(const TiXmlElement* xmlElement, const char* name, Real defaultValue = 0)
    {
        String value = xmlElement->Attribute(name);
        return value.empty() ? defaultValue : StringConverter::parseReal(value);
    }

    int GetIntAttribute(const TiXmlElement* xmlElement, const char* name, int defaultValue = 0)
    {
        String value = xmlElement->Attribute(name);
        return value.empty() ? defaultValue : StringConverter::parseInt(value);
    }

    Vector3 LoadXYZ(const TiXmlElement* objectElement)
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

    Quaternion LoadRotation(const TiXmlElement* objectElement)
    {
        Quaternion rotation = Quaternion::IDENTITY;

        rotation.x = GetRealAttribute(objectElement, "qx", 1);
        rotation.y = GetRealAttribute(objectElement, "qy", 0);
        rotation.z = GetRealAttribute(objectElement, "qz", 1);
        rotation.w = GetRealAttribute(objectElement, "qw", 0);

        return rotation;
    }

    ColourValue LoadColor(const TiXmlElement* objectElement)
    {
        ColourValue color;
        color.r = GetRealAttribute(objectElement, "r", 0);
        color.g = GetRealAttribute(objectElement, "g", 0);
        color.b = GetRealAttribute(objectElement, "b", 0);
        color.a = 1;
        return color;
    }

    String GetStringAttribute(const TiXmlElement* xmlElement, const char* name)
    {
        const char* value = xmlElement->Attribute(name);
        if (value != 0)
            return value;
        else
            return StringUtil::BLANK;
    }

    void LoadLightAttenuation(const TiXmlElement* objectElement, Light* light)
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

    void LoadLightRange(const TiXmlElement* objectElement, Light* light)
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

    void GetChildText(const TiXmlElement* xmlElement, String& text)
    {
        //Get the first element
        const TiXmlNode* childNode = xmlElement->FirstChild();
        while (childNode != 0)
        {
            if (childNode->Type() == TiXmlNode::TEXT)
            {
                const TiXmlText* textNode = childNode->ToText();
                if (textNode != 0)
                {
                    text = textNode->Value();
                    break;
                }
            }
            childNode = xmlElement->NextSibling();
        }
    }

    void loadPlane(const TiXmlElement* planeElement, SceneNode* node, Ogre::SceneManager *mSceneMgr)
    {


        Ogre::Vector3 normal;
        Ogre::Vector3 upVector;

        const TiXmlElement* childElement = 0;
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

    void loadLight(const TiXmlElement* lightElement, SceneNode* node, Ogre::SceneManager *mSceneMgr)
    {


        //Create the light
        Light* light = mSceneMgr->createLight(lightElement->Attribute("name"));
        light->setType(ParseLightType(GetStringAttribute(lightElement, "type")));
        light->setCastShadows(getBool(lightElement->Attribute("castShadows")));
        light->setPowerScale(GetRealAttribute(lightElement, "power"));
        node->attachObject(light);

        const TiXmlElement* childElement = 0;
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

    void loadActions(const TiXmlElement* rootElement, EventsManager* mEventMgr, OgreNewt::Body* body, bodyUserData* userD, OgreNewt::MaterialID* action_mat)
    {
        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");

        userD->makeActions = Ogre::StringConverter::parseBool(getElementValue(rootElement, "Amaker"));

        auto aStr = getElementValue(rootElement, "Actions");
        const char* actions = aStr.c_str();

        if (actions != 0)
        {
            int i = 0;
            char* aID = strtok(const_cast<char *> (actions), ";");
            while (aID != NULL && i < 4)
            {
                //add action
                userD->actions[i] = atoi(aID);
                myLog->logMessage("loaded aID " + Ogre::String(aID), LML_NORMAL);
                i++;
                aID = strtok(NULL, ";");
            }
            body->setMaterialGroupID(action_mat);
        }

        auto rStr = getElementValue(rootElement, "Aonstart");
        const char* startupTasks = rStr.c_str();

        if (startupTasks != 0)
        {
            int i = 0;

            Ogre::String reactString(startupTasks);
            Ogre::String separator(";");

            bool newForm = false;
            if (reactString.find("\n") != std::string::npos || reactString.find(')') == reactString.length() - 1)
            {
                newForm = true;
                separator = "\n";
            }

            char* react = strtok(const_cast<char *> (startupTasks), separator.c_str());
            while (react != NULL)
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

                react = strtok(NULL, separator.c_str());
            }
        }
    }

    std::string strtok_str(std::string& txt, char delim)
    {
        auto dPos = txt.find_first_of(delim);
        std::string ret = txt;

        if (dPos != std::string::npos)
        {
            ret.erase(dPos, std::string::npos);
            txt.erase(0, dPos+1);
        }
        else
            txt.clear();

        return ret;
    }

    void loadGrassArea(const TiXmlElement* element, Entity* ent, SceneNode* node, Ogre::SceneManager *mSceneMgr, PagingManager* pagingMgr)
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
        element = element->NextSiblingElement();

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
        grassLoader->setHeightFunction(&HeightFunction::getTerrainHeight, offsets);
        grass->setPageLoader(grassLoader);
        char delim = ';';

        for (int i = 0; i < layersNum; i++)
        {
            String curMat = strtok_str(materials, delim);
            Real curMinSizeW = Ogre::StringConverter::parseReal(strtok_str(minSizesW, delim));
            Real curMinSizeH = Ogre::StringConverter::parseReal(strtok_str(minSizesH, delim));
            Real curMaxSizeW = Ogre::StringConverter::parseReal(strtok_str(maxSizesW, delim));
            Real curMaxSizeH = Ogre::StringConverter::parseReal(strtok_str(maxSizesH, delim));

            Real curSwayDistr = Ogre::StringConverter::parseReal(strtok_str(SwayDistributions, delim));
            Real curSwayLen = Ogre::StringConverter::parseReal(strtok_str(SwayLengths, delim));
            Real curSwaySpeed = Ogre::StringConverter::parseReal(strtok_str(SwaySpeeds, delim));
            Real curDensity = Ogre::StringConverter::parseReal(strtok_str(densities, delim));

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
                String curDenMat = strtok_str(densityMaps, delim);
                layer->setDensityMap(curDenMat);
                myLog->logMessage("Grass Area loaded density map " + curDenMat, LML_NORMAL);
            }

            if (usesColorMap)
            {
                String curColMat = strtok_str(colorMaps, delim);
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

        pagingMgr->addPagedGeometry(grass);
        myLog->logMessage("Grass Area Loaded", LML_NORMAL);
    }

    void loadReflection(const TiXmlElement* element, Ogre::Entity* ent, SceneNode* node, Ogre::SceneManager* mSceneMgr)
    {
        Ogre::Vector3 pos = node->getPosition();
        Ogre::Degree yaw = node->getOrientation().getYaw();

        int mask = Ogre::StringConverter::parseInt(element->GetText());

        ReflectionTask* refl = new ReflectionTask();
        refl->init(ent, mask);
        Global::mEventsMgr->addTask(refl);
    }

    void loadInstance(const TiXmlElement* element, Ogre::Entity* ent, SceneNode* node, PagingManager* pagingMgr, Ogre::SceneManager* mSceneMgr)
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
            pagingMgr->addPagedGeometry(trees);
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

    void loadZipLinePart(const TiXmlElement* element, Ogre::Entity* ent, SceneNode* node, WorldMaterials* wMaterials, OgreNewt::World* mWorld)
    {
        String typeName = getElementValue(element, "BodyType");
        OgreNewt::Body* body = nullptr;

        if (typeName == "tree")
        {

            OgreNewt::CollisionPtr col = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision(mWorld, ent, false, 0));
            body = new OgreNewt::Body(mWorld, col);

        }
        else //if (typeName == "conv")
        {
            OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld, ent, 0));
            body = new OgreNewt::Body(mWorld, col);

            body->setMassMatrix(0, Vector3::ZERO);
        }

        body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());

        //if (!physicsOnly)
        body->attachNode(node);

        body->setType(ZipLinePart);
        body->setMaterialGroupID(wMaterials->playerIgnore_mat);

        bodyUserData* userD = new bodyUserData();
        userD->material = 0;

        String zipLineTrackName = getElementValue(element, "Track");
        userD->customData = new std::string("ZipLine"+zipLineTrackName);

        body->setUserData(Ogre::Any(userD));

        loadedBodies[ent->getName()] = body;
    }

    void loadZipLineTrack(const TiXmlElement* element, Ogre::Entity* ent, SceneNode* node, Ogre::SceneManager* mSceneMgr)
    {
        std::vector<Ogre::Vector3> points;

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

        ZipLine* line = new ZipLine(points);
        (*Global::globalData)["ZipLine" + node->getName()] = line;
    }

    void loadBillboard(const TiXmlElement* element, Ogre::Entity* ent, Ogre::SceneManager* mSceneMgr)
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

    OgreNewt::Body* loadPhysics(const TiXmlElement* rootElement, Entity* ent, SceneNode* node, OgreNewt::World* mWorld, EventsManager* mEventMgr, WorldMaterials* wMaterials)
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

            loadActions(rootElement, mEventMgr, body, userD, wMaterials->action_mat);

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
                body->setMaterialGroupID(wMaterials->flag_mat);

            body->setCenterOfMass(offset);
            body->setLinearDamping(0.3);
            body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());
            body->attachNode(node);

            loadedBodies[ent->getName()] = body;

            loadActions(rootElement, mEventMgr, body, userD, wMaterials->action_mat);

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
                body->setMaterialGroupID(wMaterials->flag_mat);

            body->setCenterOfMass(offset);
            body->setLinearDamping(0.3);
            body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());

            if (!physicsOnly)
                body->attachNode(node);

            loadedBodies[ent->getName()] = body;

            loadActions(rootElement, mEventMgr, body, userD, wMaterials->action_mat);

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

        return body;
    }
    /*
    void loadEnvSound(const TiXmlElement* element, SceneNode* node, Ogre::SceneManager *mSceneMgr, EnvironmentSounds* env)
    {
        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
        const TiXmlElement* childElement;
        childElement = element;
        float volume = Ogre::StringConverter::parseReal(childElement->GetText());
        childElement = childElement->NextSiblingElement();
        bool cont = Ogre::StringConverter::parseBool(childElement->GetText());
        childElement = childElement->NextSiblingElement();
        float decay = Ogre::StringConverter::parseReal(childElement->GetText());
        childElement = childElement->NextSiblingElement();
        Ogre::String sound = "../../media/" + Ogre::String(childElement->GetText());
        Vector3 scale = node->getScale();
        node->setScale(1, 1, 1);
        Ogre::Matrix4 trans = node->_getFullTransform().inverse();

        env->addSound(trans, scale, decay, volume, sound, cont);

        node->detachAllObjects();
        mSceneMgr->destroySceneNode(node);
        myLog->logMessage("EnvSound - " + sound, LML_NORMAL);
    }
    */
    void loadJoint(const TiXmlElement* element, SceneNode* node, Ogre::SceneManager *mSceneMgr)
    {
        const TiXmlElement* fElement = element->NextSiblingElement();

        const char* children = element->GetText();
        const char* parents = fElement->GetText();

        char *token, *token2;
        char* child = strtok_s(const_cast<char *> (children), ";", &token);
        element = element->NextSiblingElement();
        char* parent = strtok_s(const_cast<char *> (parents), ";", &token2);

        const TiXmlElement* typeElement = fElement->NextSiblingElement();
        char type = 0;
        if (typeElement != 0)
        {
            type = Ogre::StringConverter::parseInt(typeElement->GetText());
        }

        while (child != NULL && parent != NULL)
        {
            LoadedJointInfo info;

            info.body0 = Ogre::String(child);
            info.body1 = Ogre::String(parent);
            info.position = node->getPosition();
            info.pin = node->_getDerivedOrientation()*Ogre::Vector3(1, 0, 0);
            info.type = type;

            loadedJoints.push_back(info);

            //next
            child = strtok_s(NULL, ";", &token);
            parent = strtok_s(NULL, ";", &token2);

            Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Joint - child:" + info.body0 + " and parent:" + info.body1, LML_NORMAL);
        }

        node->detachAllObjects();
        mSceneMgr->destroySceneNode(node);

    }

    void loadBridge(const TiXmlElement* element, SceneNode* node, Ogre::SceneManager *mSceneMgr, uint32 visibilityFlag)
    {
        Ogre::Vector3 pos = node->getPosition();
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Making BM", LML_NORMAL);
        Ogre::Vector3 target;
        const char* posText = element->GetText();
        char* val = strtok(const_cast<char *> (posText), ",");
        target.x = Ogre::StringConverter::parseReal(val);
        val = strtok(NULL, ",");
        target.z = -Ogre::StringConverter::parseReal(val);
        val = strtok(NULL, ",");
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

        BridgeMaker* bm;
        auto it = Global::globalData->find("BridgeMaker");
        if (it == Global::globalData->end())
        {
            bm = new BridgeMaker(mSceneMgr, Global::mWorld);
            (*Global::globalData)[Ogre::String("BridgeMaker")] = bm;
            Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Making BM", LML_NORMAL);

        }
        else
        {
            Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Loaded BM", LML_NORMAL);
            bm = (BridgeMaker*)it->second;
        }

        if (mat.empty())
            bm->makeBridge(pos, target, scale, loose, matType, visibilityFlag);
        else
            bm->makeBridge(pos, target, scale, loose, matType, visibilityFlag, mat);

        node->detachAllObjects();
        mSceneMgr->destroySceneNode(node);

        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Bridge - to: " + Ogre::StringConverter::toString(target) + " and loose: " + Ogre::StringConverter::toString(loose) + " and scale: " + Ogre::StringConverter::toString(scale), LML_NORMAL);
    }

    void loadSound(const TiXmlElement* element, SceneNode* node, Ogre::SceneManager *mSceneMgr)
    {
        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
        const TiXmlElement* childElement = element;

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

    void loadCompoundBodyPart(const TiXmlElement* element, Entity* ent, SceneNode* node, OgreNewt::World* mWorld, Ogre::SceneManager *mSceneMgr)
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

    TriggerInfo* loadTrigger(const TiXmlElement* element, bool nonphysical = false)
    {

        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");

        TriggerAndIDs trig;

        const char* actions = element->GetText();

        if (actions != 0)
        {
            int i = 0;
            char* aID = strtok(const_cast<char *> (actions), ";");
            while (aID != NULL && i < 4)
            {
                //add action
                myLog->logMessage("Trigger ID " + Ogre::String(aID), LML_NORMAL);
                trig.ids.push_back(atoi(aID));
                i++;
                aID = strtok(NULL, ";");
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

    void loadActions(const TiXmlElement* element, void* data, int id = 0)
    {

        Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");

        const TiXmlElement* fElement = element->NextSiblingElement();

        const char* tasks = element->GetText();
        const char* functions = fElement->GetText();

        if (functions == NULL)
            return;

        int i = 0;
        char *token, *token2;
        char* aID = NULL;
        char* fID = NULL;

        bool newForm = false;
        if (tasks == NULL)
        {
            fID = strtok_s(const_cast<char *> (functions), "\n", &token2);
            newForm = true;
        }
        else
        {
            aID = strtok_s(const_cast<char *> (tasks), ";", &token);
            fID = strtok_s(const_cast<char *> (functions), ";", &token2);
        }

        while (fID != NULL && (aID != NULL || newForm))
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
                myLog->logMessage("No such function available", LML_NORMAL);

            //next
            if (newForm)
            {
                fID = strtok_s(NULL, "\n", &token2);
            }
            else
            {
                aID = strtok_s(NULL, ";", &token);
                fID = strtok_s(NULL, ";", &token2);
            }

        }

    }

    int setModifierStart(const TiXmlElement* rootElement, Entity** ent, SceneNode** node, Ogre::SceneManager *mSceneMgr)
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

    void setModifierEnd(const TiXmlElement* rootElement, Entity** ent, SceneNode** node, Ogre::SceneManager *mSceneMgr)
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

    void loadEntity(const TiXmlElement* entityElement, SceneNode* node, bool visible, Ogre::SceneManager *mSceneMgr, OgreNewt::World* mWorld,
                    EventsManager* mEventMgr, PagingManager* pagingMgr, WorldMaterials* wMaterials)
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

        const TiXmlElement* subentityElement = entityElement->FirstChildElement("subentities");
        const TiXmlElement* childElement = 0;
        while (childElement = IterateChildElements(subentityElement, childElement))
        {
            int index = GetIntAttribute(childElement, "index", 0);
            ent->getSubEntity(index)->setMaterialName(GetStringAttribute(childElement, "materialName"));
        }


        const TiXmlElement* userdataElement = entityElement->FirstChildElement("userData");

        if (userdataElement != NULL)
        {

            Ogre::String userData;
            GetChildText(userdataElement, userData);

            Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
            myLog->logMessage(userData, LML_NORMAL);

            TiXmlDocument document;
            document.Parse(userData.c_str());
            if (!document.Error())
            {
                TiXmlElement *root = document.RootElement();

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
                        loadGrassArea(element, ent, node, mSceneMgr, pagingMgr);
                    }
                    else if (rootTag == "Billboard")
                    {
                        loadBillboard(element, ent, mSceneMgr);
                    }
                    else if (rootTag == "Instanced")
                    {
                        loadInstance(element, ent, node, pagingMgr, mSceneMgr);
                    }
                    else if (rootTag == "Reflection")
                    {
                        loadReflection(element, ent, node, mSceneMgr);
                    }
                    else if (rootTag == "ZipLineTrack")
                    {
                        loadZipLineTrack(root, ent, node, mSceneMgr);
                    }
                    else if (rootTag == "ZipLinePart")
                    {
                        loadZipLinePart(root, ent, node, wMaterials, mWorld);
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
                        TiXmlElement* modifElement = element;
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
            }
        }

    }


    void loadPlayerInfo(const TiXmlElement* nodeElement, Player* pl)
    {
        String elementName;
        const TiXmlElement* childElement = 0;
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
                const TiXmlElement* userdataElement = childElement->FirstChildElement("userData");

                if (userdataElement != NULL)
                {
                    Ogre::String userData;
                    GetChildText(userdataElement, userData);

                    Ogre::Log* myLog = Ogre::LogManager::getSingleton().getLog("Loading.log");
                    myLog->logMessage(userData, LML_NORMAL);

                    TiXmlDocument document;
                    document.Parse(userData.c_str());
                    if (!document.Error())
                    {
                        TiXmlElement *root = document.RootElement();

                        if (root->Value() != NULL)
                        {
                            Ogre::String rootTag(root->Value());
                            TiXmlElement *element = root->FirstChildElement();

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

    void loadAnimations(const TiXmlElement* element, Ogre::SceneNode* node, Ogre::SceneManager* mSceneMgr)
    {

        node = mSceneMgr->getRootSceneNode()->createChildSceneNode(node->getName() + "Anim", node->_getDerivedPosition(), node->_getDerivedOrientation());

        const TiXmlElement* childElement = 0;
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

            const TiXmlElement* keyElement = 0;
            while (keyElement = IterateChildElements(childElement, keyElement))
            {
                float time = GetRealAttribute(keyElement, "time");
                Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(time);

                const TiXmlElement* keyInfoElement = keyElement->FirstChildElement();
                kf->setTranslate(LoadXYZ(keyInfoElement));
                keyInfoElement = keyInfoElement->NextSiblingElement();
                kf->setRotation(LoadRotation(keyInfoElement));
                keyInfoElement = keyInfoElement->NextSiblingElement();
                kf->setScale(LoadXYZ(keyInfoElement));
            }

            Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("Loaded node animation " + animName + " with " + Ogre::StringConverter::toString(track->getNumKeyFrames()) + " keyframes", LML_NORMAL);;
        }

    }

    void loadNode(const TiXmlElement* nodeElement, PagingManager* pagingMgr, WorldMaterials* wMaterials)
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

            node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);

            bool visible = true;
            if (nodeElement->Attribute("visibility") != NULL)
            {
                Ogre::String v = nodeElement->Attribute("visibility");

                if (v == "hidden") visible = false;
            }


            //first properties, then content
            String elementName;
            const TiXmlElement* childElement = 0;


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
                    loadEntity(childElement, node, visible, mSceneMgr, mWorld, mEventMgr, pagingMgr, wMaterials);
                else if (elementName == "light")
                    loadLight(childElement, node, mSceneMgr);
                else if (elementName == "plane")
                    loadPlane(childElement, node, mSceneMgr);
            }

        }
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

        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("\nBEGGINING OF TRIGGERS ::", LML_NORMAL);

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
    }

    bool isCompoundBody(const TiXmlElement* nodeElement)
    {
        const TiXmlElement* childElement = nodeElement->FirstChildElement("entity");
        if (childElement != NULL)
        {
            const TiXmlElement* userdataElement = childElement->FirstChildElement("userData");

            if (userdataElement != NULL)
            {
                Ogre::String userData;
                GetChildText(userdataElement, userData);

                TiXmlDocument document;
                document.Parse(userData.c_str());
                if (!document.Error())
                {
                    TiXmlElement *root = document.RootElement();

                    if (root->Value() != NULL)
                    {
                        Ogre::String rootTag(root->Value());
                        if (rootTag.size() > 12)
                            rootTag.erase(12, rootTag.size() - 12);

                        if (rootTag == "PhysicalBody")
                        {
                            TiXmlElement *element = root->FirstChildElement("BodyType");
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

public:

    void loadScene(Ogre::String filename, PagingManager* pagingMgr, WorldMaterials* wMaterials)
    {

        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("LOADING SCENE :: filename \"" + filename + "\"", LML_NORMAL);

        TiXmlDocument document;
        document.LoadFile(filename.c_str());
        const TiXmlElement* mainElement = document.RootElement();
        const TiXmlElement* nodesElement = mainElement->FirstChildElement("nodes");

        loadedTasks.clear();
        loadedTriggers.clear();
        loadedJoints.clear();
        loadedBodies.clear();
        loadedBillboardSets.clear();
        loadedForests.clear();

        (*Global::globalData)["loadedBodies"] = &loadedBodies;

        if (Global::player != NULL)
            loadedBodies["Player"] = Global::player->body;

        std::vector<const TiXmlElement*> compBodies;
        String elementName;
        const TiXmlElement* childElement = 0;
        while (childElement = IterateChildElements(nodesElement, childElement))
        {
            elementName = childElement->Value();

            if (elementName == "node")
            {
                if (!isCompoundBody(childElement))
                    loadNode(childElement, pagingMgr, wMaterials);
                else
                    compBodies.push_back(childElement);
            }
            /*else if (elementName == "skyBox")
            	LoadSkyBox(childElement);*/
        }

        for (uint i = 0; i < compBodies.size(); i++)
        {
            loadNode(compBodies.at(i), pagingMgr, wMaterials);
        }
        compBodies.clear();

        connectJoints();
        summarizeLevelEvents();
        compoundBodiesParts.clear();
        loadedForests.clear();

        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("LOADING COMPLETED :: filename \"" + filename + "\"", LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("Loading.log")->logMessage("-----------------------------------------------------------", LML_NORMAL);
    }


};