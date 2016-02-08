#include "stdafx.h"
#include "TriggerPlayerContactCallback.h"
#include "TriggerObjectContactCallback.h"
#include "DefaultObjectContactCallback.h"

void WorldMaterials::init()
{
    auto mWorld = Global::mWorld;

    plNoMove_mat = new OgreNewt::MaterialID(mWorld);
    plMove_mat = new OgreNewt::MaterialID(mWorld);
    noCollide_mat = new OgreNewt::MaterialID(mWorld);
    playerIgnore_mat = new OgreNewt::MaterialID(mWorld);
    trig_mat = new OgreNewt::MaterialID(mWorld);
    actionMaker_mat = new OgreNewt::MaterialID(mWorld);
    selfIgnore_mat = new OgreNewt::MaterialID(mWorld);
    plBlock_mat = new OgreNewt::MaterialID(mWorld);

    initCollisions(mWorld);
}

void WorldMaterials::initCollisions(OgreNewt::World* mWorld)
{

    OgreNewt::MaterialPair* material_pair = new OgreNewt::MaterialPair(mWorld, plMove_mat, mWorld->getDefaultMaterialID());
    material_pair->setDefaultFriction(0.0, 0.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setDefaultSurfaceThickness(0.01);

    material_pair = new OgreNewt::MaterialPair(mWorld, plNoMove_mat, mWorld->getDefaultMaterialID());
    material_pair->setDefaultFriction(4.0, 4.0f);
    material_pair->setDefaultElasticity(0);


    material_pair = new OgreNewt::MaterialPair(mWorld, plMove_mat, selfIgnore_mat);
    material_pair->setDefaultFriction(0.0, 0.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setDefaultSurfaceThickness(0.01);

    material_pair = new OgreNewt::MaterialPair(mWorld, plNoMove_mat, selfIgnore_mat);
    material_pair->setDefaultFriction(4.0, 4.0f);
    material_pair->setDefaultElasticity(0);


    material_pair = new OgreNewt::MaterialPair(mWorld, plMove_mat, actionMaker_mat);
    material_pair->setDefaultFriction(0.0, 0.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setDefaultSurfaceThickness(0.01);

    material_pair = new OgreNewt::MaterialPair(mWorld, plNoMove_mat, actionMaker_mat);
    material_pair->setDefaultFriction(4.0, 4.0f);
    material_pair->setDefaultElasticity(0);


    material_pair = new OgreNewt::MaterialPair(mWorld, plMove_mat, playerIgnore_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, plNoMove_mat, playerIgnore_mat);
    material_pair->setDefaultCollidable(0);

    //TRIGGER ********************************************************
    //WITH PLAYER
    TriggerPlayerContactCallback* callback_p = new TriggerPlayerContactCallback(Global::mEventsMgr);

    material_pair = new OgreNewt::MaterialPair(mWorld, plMove_mat, trig_mat);
    material_pair->setDefaultFriction(0.0, 0.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setDefaultSurfaceThickness(0.01);
    material_pair->setContactCallback(callback_p);

    material_pair = new OgreNewt::MaterialPair(mWorld, plNoMove_mat, trig_mat);
    material_pair->setDefaultFriction(4.0, 4.0f);
    material_pair->setDefaultElasticity(0);
    material_pair->setContactCallback(callback_p);

    //WITH OBJECTS
    TriggerObjectContactCallback* callback = new TriggerObjectContactCallback(Global::mEventsMgr);

    material_pair = new OgreNewt::MaterialPair(mWorld, trig_mat, actionMaker_mat);
    material_pair->setContactCallback(callback);

    material_pair = new OgreNewt::MaterialPair(mWorld, trig_mat, playerIgnore_mat);
    material_pair->setContactCallback(callback);

    material_pair = new OgreNewt::MaterialPair(mWorld, trig_mat, trig_mat);
    material_pair->setContactCallback(callback);
    //****************************************************************


    //FLAGS FOR RAYS
    material_pair = new OgreNewt::MaterialPair(mWorld, plMove_mat, noCollide_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, plNoMove_mat, noCollide_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, mWorld->getDefaultMaterialID(), noCollide_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, noCollide_mat, noCollide_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, noCollide_mat, playerIgnore_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, noCollide_mat, selfIgnore_mat);
    material_pair->setDefaultCollidable(0);

    material_pair = new OgreNewt::MaterialPair(mWorld, noCollide_mat, actionMaker_mat);
    material_pair->setDefaultCollidable(0);
    //****************************************************************

    material_pair = new OgreNewt::MaterialPair(mWorld, mWorld->getDefaultMaterialID(), plBlock_mat);
    material_pair->setDefaultCollidable(0);

    DefaultObjectContactCallback* callb = new DefaultObjectContactCallback();

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
    if (plMove_mat!=NULL)
    {
        delete plNoMove_mat;
        delete plMove_mat;
        delete noCollide_mat;
        delete playerIgnore_mat;
        delete trig_mat;
        delete actionMaker_mat;
        delete selfIgnore_mat;
    }
}