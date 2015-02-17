#include "stdafx.h"
#include "bird.h"

using namespace Ogre;

int Bird::counter=0;

Bird::Bird(Ogre::SceneManager *mSceneM, OgreNewt::World *W, Ogre::Vector3 position,Ogre::SceneNode* tr,OgreNewt::MaterialID* m)
{

    tracker=tr;

    Entity* ent = mSceneM->createEntity(  "bird"+Ogre::StringConverter::toString(counter++), "Bird.mesh" );
    OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(W,ent,10));
    ent->setMaterialName("birdMat");
    OgreNewt::Body* mBody = new OgreNewt::Body( W, col );
    SceneNode* bNode2 = mSceneM->getRootSceneNode()->createChildSceneNode();
    SceneNode* bNode = bNode2->createChildSceneNode();
    bNode->attachObject( ent );
    bNode->setScale(1,1,1);
    bNode->_updateBounds();
    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);

    mBody->setMaterialGroupID(m);
    bN=bNode;
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    //no longer need the collision shape object
    delete col;
#endif
    mBody->setMassMatrix( 0.3, inertia );
    mBody->setCenterOfMass(offset);
    mBody->setPositionOrientation( position, Ogre::Quaternion::IDENTITY );
    mBody->attachNode(bNode2 );
    mBody->setCustomForceAndTorqueCallback<Bird>(&Bird::move_callback, this);
    mBody->setAngularDamping(Vector3(1,1,1)*300);
    mBody->setLinearDamping(300);


}



void Bird::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
    if(tracker==NULL)
        me->setForce(Vector3(0,1,0));
    else
    {
        Vector3 smer=(tracker->getPosition()-me->getOgreNode()->_getDerivedPosition())*2;

        Vector3 o=me->getVelocity();
        Radian r=smer.angleBetween(o);

        Real rad=r.valueDegrees();

        MaterialPtr mat=MaterialManager::getSingleton().getByName("birdMat");
        mat->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant("turnSpeed",rad/4 );

        if(rad>45)
        {
            me->setVelocity(me->getVelocity()/2);
            smer*=rad/20;
        }

        Ogre::Quaternion q=Ogre::Vector3(1,0,0).getRotationTo(o);

        bN->setOrientation(q);

        me->setForce(smer);

    }
}


