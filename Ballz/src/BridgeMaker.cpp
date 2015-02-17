#include "stdafx.h"
#include "BridgeMaker.h"
#include "OgreNewt_Joint.h"

using namespace Ogre;

Ogre::String BridgeMaker::defaultMat = Ogre::String("woodPlankMat");

BridgeMaker::BridgeMaker(Ogre::SceneManager *mSceneM, OgreNewt::World *W)
{

    mSceneMgr=mSceneM;
    mWorld=W;
}


/*looseness- 1=very loose, 0=very tight                */
void BridgeMaker::makeBridge(Ogre::Vector3 position, Ogre::Vector3 target, Ogre::Real scale, Ogre::Real looseness, uint32 visibilityFlag, Ogre::String material)
{
    OgreNewt::Body* parent;

    Ogre::Vector3 flatDir;
    int parts;
    Ogre::Quaternion planksOr;
    float pinAngle;


    flatDir = target - position;
    flatDir.y = 0;
    flatDir.normalise();
    flatDir*=1.8f*scale;

    float length = (target - position).length();
    parts = (int)Ogre::Math::Floor(length/(1.8f*scale) + 0.5f);
    parts += (int)(looseness*parts / 4.0f);

    planksOr = Ogre::Vector3(0,0,1).getRotationTo(flatDir);

    pinAngle = planksOr.getYaw().valueRadians();

    if(pinAngle==0)
        pinAngle = 0.001f;


    Entity* bridgeEnd = mSceneMgr->createEntity("bridgePlank.mesh" );
    OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,bridgeEnd,10));
    bridgeEnd->setMaterialName(material);
    bridgeEnd->setVisibilityFlags(visibilityFlag);
    OgreNewt::Body* bridgeB = new OgreNewt::Body( mWorld, col );
    SceneNode* bNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    bNode->attachObject( bridgeEnd );
    bNode->setScale(Vector3(scale));

    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);

    bridgeB->setMassMatrix( 0, inertia );
    bridgeB->setCenterOfMass(offset);
    bridgeB->setPositionOrientation( position, planksOr );
    bridgeB->attachNode(bNode );
	bridgeB->setType(Bridge);

    parent=bridgeB;

    for(int i=0; i<parts; i++)
    {

        Entity* bridgeE = mSceneMgr->createEntity("bridgePlank.mesh" );
        bridgeE->setMaterialName(material);
        SceneNode* bNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        bNode->attachObject( bridgeE );
        bNode->setScale(Vector3(scale));
        bridgeE->setVisibilityFlags(visibilityFlag);
        //rNode->_updateBounds();//showBoundingBox(true);
        col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,bridgeE,10));

        bridgeB = new OgreNewt::Body( mWorld, col );

        Ogre::Vector3 inertia, offset;
        col->calculateInertialMatrix(inertia, offset);

        if(i==parts-1)
            bridgeB->setMassMatrix( 0, inertia );
        else
            bridgeB->setMassMatrix( 0.1, inertia );

        bridgeB->setCenterOfMass(offset);
        bridgeB->setPositionOrientation( position+float(i+1)*flatDir, planksOr ); //Vector3(0,0,scale*(1.8f+i*1.8f))
        bridgeB->attachNode(bNode );

        bridgeB->setCustomForceAndTorqueCallback<BridgeMaker>(&BridgeMaker::move_callback, this);
        bridgeB->setAngularDamping(Vector3(1,1,1)*0.6);
        bridgeB->setLinearDamping(0.6);
		bridgeB->setType(Bridge);

        OgreNewt::BallAndSocket* b=new OgreNewt::BallAndSocket(parent,bridgeB,position+flatDir/2.0f+(float)i*flatDir,pinAngle);//+Vector3(0,0,scale*(0.9f+i*1.8f))
        b->setCollisionState(0);


        parent=bridgeB;

        if(i==parts-1)
            parent->setPositionOrientation( target, planksOr );

    }

}

void BridgeMaker::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
    Ogre::Quaternion q;
    q.FromAngleAxis(me->getOrientation().getYaw(),Ogre::Vector3(0,1,0));

    Ogre::Vector3 om = me->getOmega() + q*Ogre::Vector3(0,0,-0.005f*me->getOrientation().getRoll().valueRadians());

    me->addForce(Ogre::Vector3(0,-1,0));
    me->setOmega(om);
}

BridgeMaker::~BridgeMaker ()
{

}
