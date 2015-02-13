#include "stdafx.h"
#include "baloon.h"

using namespace Ogre;

int Baloon::counter=0;

Baloon::Baloon(Ogre::SceneManager *mSceneM, OgreNewt::World *W, Ogre::Vector3 position)
{

    tracker=NULL;
    mSceneMgr=mSceneM;
    mWorld=W;

    baloonEnt = mSceneMgr->createEntity(  "balon"+Ogre::StringConverter::toString(counter), "Sphere01.mesh" );
    OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,baloonEnt,1));
    baloonEnt->setMaterialName("testMat");
    baloonB = new OgreNewt::Body( mWorld, col );
    baloonNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    baloonNode->attachObject( baloonEnt );
    baloonNode->setScale(1,1.18717,1);
    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    //no longer need the collision shape object
    delete col;
#endif
    baloonB->setMassMatrix( 0.3, inertia );
    baloonB->setCenterOfMass(offset);
    baloonB->setPositionOrientation( position, Ogre::Quaternion::IDENTITY );
    baloonB->attachNode(baloonNode );
    baloonB->setCustomForceAndTorqueCallback<Baloon>(&Baloon::move_callback, this);
    baloonB->setAngularDamping(Vector3(1,1,1)*300);
    baloonB->setLinearDamping(300);


    ropeEnt = mSceneMgr->createEntity( "lano"+Ogre::StringConverter::toString(counter), "Cone02.mesh" );
    col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,ropeEnt,1));
    ropeB = new OgreNewt::Body( mWorld, col );
    ropeEnt->setMaterialName("lanoMat");
    col->calculateInertialMatrix(inertia, offset);
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    //no longer need the collision shape object
    delete col;
#endif
    ropeB->setMassMatrix( 0.1, inertia );
    ropeB->setCenterOfMass(offset);
    ropeB->setPositionOrientation( position-Ogre::Vector3(0,15.25,0), Ogre::Quaternion::IDENTITY );
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject( ropeEnt );
    ropeB->attachNode( node );
    ropeB->setType(4);
    ropeB->getOgreNode()->setUserAny(Ogre::Any(this));
    ropeB->setStandardForceCallback();

    OgreNewt::BallAndSocket* joint=new OgreNewt::BallAndSocket(ropeB,baloonB,position-Vector3(Ogre::Vector3(0,9.27,0)),0);

    id=counter++;
}


Baloon::~Baloon ()
{
    delete ropeB;
    delete baloonB;
    ropeEnt->detachFromParent();
    baloonEnt->detachFromParent();
    mSceneMgr->destroyEntity(ropeEnt);
    mSceneMgr->destroyEntity(baloonEnt);
    mSceneMgr->destroySceneNode(baloonNode);
    mSceneMgr->destroySceneNode(node);
}

void Baloon::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
    if(tracker==NULL)
        me->setForce(Vector3(0,1,0));
    else
    {
        Vector3 smer=(tracker->getPosition()-me->getOgreNode()->_getDerivedPosition())*2;

        Vector3 o=me->getVelocity();
        Radian r=smer.angleBetween(o);

        Real rad=r.valueDegrees();
        if(rad>45)
        {
            me->setVelocity(me->getVelocity()/2);
            smer*=rad/20;
        }

        me->setForce(smer);
    }
}
