#include "stdafx.h"
#include "TriggerPlayerContactCallback.h"
#include "TriggerObjectContactCallback.h"
#include "DefaultObjectContactCallback.h"
#include "SlideContactCallback.h"

void WorldMaterials::init(OgreNewt::World* mWorld)
{
    stoji_mat = new OgreNewt::MaterialID(mWorld);
    ide_mat = new OgreNewt::MaterialID(mWorld);
    flag_mat = new OgreNewt::MaterialID(mWorld);
    playerIgnore_mat = new OgreNewt::MaterialID(mWorld);
    trig_mat = new OgreNewt::MaterialID(mWorld);
    action_mat = new OgreNewt::MaterialID(mWorld);
    selfIgnore_mat = new OgreNewt::MaterialID(mWorld);
    slide_mat = new OgreNewt::MaterialID(mWorld);

    initCollisions(mWorld);
}

void WorldMaterials::initCollisions(OgreNewt::World* mWorld)
{

    OgreNewt::MaterialPair* material_pair = new OgreNewt::MaterialPair(mWorld, ide_mat, mWorld->getDefaultMaterialID());
    material_pair->setDefaultFriction(0.0, 0.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setDefaultSurfaceThickness(0.01);

    material_pair = new OgreNewt::MaterialPair(mWorld, stoji_mat, mWorld->getDefaultMaterialID());
    material_pair->setDefaultFriction(4.0, 4.0f);
    material_pair->setDefaultElasticity(0);


    material_pair = new OgreNewt::MaterialPair(mWorld, ide_mat, selfIgnore_mat);
    material_pair->setDefaultFriction(0.0, 0.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setDefaultSurfaceThickness(0.01);

    material_pair = new OgreNewt::MaterialPair(mWorld, stoji_mat, selfIgnore_mat);
    material_pair->setDefaultFriction(4.0, 4.0f);
    material_pair->setDefaultElasticity(0);


    material_pair = new OgreNewt::MaterialPair(mWorld, ide_mat, action_mat);
    material_pair->setDefaultFriction(0.0, 0.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setDefaultSurfaceThickness(0.01);

    material_pair = new OgreNewt::MaterialPair(mWorld, stoji_mat, action_mat);
    material_pair->setDefaultFriction(4.0, 4.0f);
    material_pair->setDefaultElasticity(0);


    material_pair = new OgreNewt::MaterialPair(mWorld, ide_mat, playerIgnore_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, stoji_mat, playerIgnore_mat);
    material_pair->setDefaultCollidable(0);

    //SLIDE
    SlideContactCallback* callback_s = new SlideContactCallback(Global::mEventsMgr);

    material_pair = new OgreNewt::MaterialPair(mWorld, ide_mat, slide_mat);
    material_pair->setDefaultFriction(0.0, 0.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setDefaultSurfaceThickness(0.01);
    material_pair->setContactCallback(callback_s);

    material_pair = new OgreNewt::MaterialPair(mWorld, stoji_mat, slide_mat);
    material_pair->setDefaultFriction(4.0, 4.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setContactCallback(callback_s);

    //TRIGGER ********************************************************
    //WITH PLAYER
    TriggerPlayerContactCallback* callback_p = new TriggerPlayerContactCallback(Global::mEventsMgr);

    material_pair = new OgreNewt::MaterialPair(mWorld, ide_mat, trig_mat);
    material_pair->setDefaultFriction(0.0, 0.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setDefaultSurfaceThickness(0.01);
    material_pair->setContactCallback(callback_p);

    material_pair = new OgreNewt::MaterialPair(mWorld, stoji_mat, trig_mat);
    material_pair->setDefaultFriction(4.0, 4.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setContactCallback(callback_p);

    //WITH OBJECTS
    TriggerObjectContactCallback* callback = new TriggerObjectContactCallback(Global::mEventsMgr);

    material_pair = new OgreNewt::MaterialPair(mWorld, trig_mat, action_mat);
    material_pair->setContactCallback(callback);

    material_pair = new OgreNewt::MaterialPair(mWorld, trig_mat, playerIgnore_mat);
    material_pair->setContactCallback(callback);

    material_pair = new OgreNewt::MaterialPair(mWorld, trig_mat, trig_mat);
    material_pair->setContactCallback(callback);
    //****************************************************************


    //FLAG
    material_pair = new OgreNewt::MaterialPair(mWorld, ide_mat, flag_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, stoji_mat, flag_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, mWorld->getDefaultMaterialID(), flag_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, flag_mat, flag_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, flag_mat, playerIgnore_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, flag_mat, selfIgnore_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, flag_mat, action_mat);
    material_pair->setDefaultCollidable(0);
    //****************************************************************


    DefaultObjectContactCallback* callb = new DefaultObjectContactCallback(Global::soundEngine);

    //SELF_IGNORE
    material_pair = new OgreNewt::MaterialPair(mWorld, selfIgnore_mat, selfIgnore_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, selfIgnore_mat, mWorld->getDefaultMaterialID());
    material_pair->setDefaultFriction(0.8, 0.8f);
    material_pair->setDefaultSoftness(0.1);
    material_pair->setDefaultElasticity(0.1);
    material_pair->setContactCallback(callb);

    material_pair = new OgreNewt::MaterialPair(mWorld, trig_mat, mWorld->getDefaultMaterialID());
    material_pair->setDefaultFriction(0.8, 0.8f);
    material_pair->setDefaultSoftness(0.1);
    material_pair->setDefaultElasticity(0.1);
    material_pair->setContactCallback(callb);

    material_pair = new OgreNewt::MaterialPair(mWorld, mWorld->getDefaultMaterialID(), mWorld->getDefaultMaterialID());
    material_pair->setDefaultFriction(0.8, 0.8f);
    material_pair->setDefaultSoftness(0.1);
    material_pair->setDefaultElasticity(0.1);
    material_pair->setContactCallback(callb);

}
WorldMaterials::~WorldMaterials()
{
    if (ide_mat!=NULL)
    {
        delete stoji_mat;
        delete ide_mat;
        delete flag_mat;
        delete playerIgnore_mat;
        delete trig_mat;
        delete action_mat;
        delete selfIgnore_mat;
    }
}