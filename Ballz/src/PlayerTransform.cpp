#include "stdafx.h"
#include "PlayerTransform.h"
#include "Player.h"

void PlayerTransform::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex)
{
    me->addForce(Ogre::Vector3(0, -5, 0));
}

PlayerTransform::PlayerTransform(Player* player)
{
    /* p = player;
     float size = 3;

     Ogre::Entity* ent = Global::mSceneMgr->createEntity("basicSphere.mesh");
     SceneNode* node = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode("transformNode");
     node->attachObject(ent);
     // ent->setCastShadows(false);
     // ent->setVisible(false);

     OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(Global::mWorld, Ogre::Vector3(size, size, size), 10));
     //OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(Global::mWorld, ent, 10));
     OgreNewt::Body* body = new OgreNewt::Body(Global::mWorld, col);

     Ogre::Vector3 inertia, offset;
     col->calculateInertialMatrix(inertia, offset);
    #ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
     //no longer need the collision shape object
     delete col;
    #endif
     body->setMassMatrix(2.5, inertia);
     body->setCenterOfMass(offset);
     body->setContinuousCollisionMode(1);
     body->setPositionOrientation(p->body->getPosition(), Ogre::Quaternion::IDENTITY);
     body->setLinearDamping(0);
     body->setAngularDamping(Vector3::ZERO);
     body->setAutoSleep(0);
     body->attachNode(node);

     body->setCustomForceAndTorqueCallback<PlayerTransform>(&PlayerTransform::move_callback, this);
    */
    //node->addChild(p->detachHead());
    //joint = new OgreNewt::BallAndSocket(p->body, body, p->body->getPosition(), 0);
}

PlayerTransform::~PlayerTransform()
{

}

void PlayerTransform::update(float tslf)
{

}

void PlayerTransform::pressedKey(const OIS::KeyEvent &arg)
{

}

void PlayerTransform::releasedKey(const OIS::KeyEvent &arg)
{

}

void PlayerTransform::movedMouse(const OIS::MouseEvent &e)
{

}