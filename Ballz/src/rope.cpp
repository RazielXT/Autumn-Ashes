#include "stdafx.h"
#include "rope.h"
#include "OgreNewt_Joint.h"

using namespace Ogre;

int Rope::counter=0;

Rope::Rope(Ogre::SceneManager *mSceneM, OgreNewt::World *W, Ogre::Vector3 position, int parts,Real scale)
{

    mSceneMgr=mSceneM;
    mWorld=W;
    OgreNewt::Body* parent;

    Entity* ropeEnd = mSceneMgr->createEntity(  "rope"+Ogre::StringConverter::toString(counter++), "ropeV.mesh" );
    OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,ropeEnd,10));
    ropeEnd->setMaterialName("kamenbob10");
    OgreNewt::Body* ropeB = new OgreNewt::Body( mWorld, col );
    SceneNode* rNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    rNode->attachObject( ropeEnd );
    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);

    ropeB->setMassMatrix( 0, inertia );
    ropeB->setCenterOfMass(offset);
    ropeB->setPositionOrientation( position, Ogre::Quaternion::IDENTITY );
    ropeB->attachNode(rNode );

    parent=ropeB;

    for(int i=0; i<parts; i++)
    {

        Entity* ropeE = mSceneMgr->createEntity(  "rope"+Ogre::StringConverter::toString(counter++), "ropeV.mesh" );
        ropeE->setMaterialName("kamenbob10");
        SceneNode* rNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        rNode->attachObject( ropeE );
        rNode->setScale(Vector3(4,scale,4));
        col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,ropeE,10));
        rNode->setScale(Vector3(2,scale,2));
        //ropeE->setMaterialName("ropeM");
        ropeB = new OgreNewt::Body( mWorld, col );

        Ogre::Vector3 inertia, offset;
        col->calculateInertialMatrix(inertia, offset);

        ropeB->setMassMatrix( 0.05, inertia );
        ropeB->setCenterOfMass(offset);
        ropeB->setPositionOrientation( position-Vector3(0,scale*(2+i*2),0), Ogre::Quaternion::IDENTITY );
        ropeB->attachNode(rNode );
        ropeB->setStandardForceCallback();
        ropeB->setAngularDamping(Vector3(1,1,1)*0.1);
        ropeB->setLinearDamping(0.1);
        ropeB->setType(7);

        OgreNewt::BallAndSocket* b=new OgreNewt::BallAndSocket(parent,ropeB,position-Vector3(0,scale*i*2,0),3);
        b->setCollisionState(0);
        //b->setStiffness(0.5);

        parent=ropeB;

    }


}


Rope::~Rope ()
{

}
