#pragma once

#include "stdafx.h"
#include "BridgeMaker.h"
#include "GeometryManager.h"
#include "PostProcessMgr.h"
#include "lvl_update.h"
#include "SceneParser.h"
#include "CrowWatch.h"

using namespace Ogre;

void createMenuLevel()
{
    PostProcessMgr* postProcMgr = Global::mPPMgr;
    postProcMgr->radialHorizBlurVignette.z = 1.0;
    postProcMgr->setGodraySunPositionFar(Vector3(300, 300, 400) * Vector3(400, -300, -400));

    Camera* mCamera=Global::mSceneMgr->getCamera("Camera");
    SceneNode* camRotator=Global::mSceneMgr->getRootSceneNode()->createChildSceneNode("camRotator");
    camRotator->attachObject(mCamera);
    camRotator->setPosition(Vector3(0,15,0));
    mCamera->setPosition(Vector3(-15,-9,15));
    mCamera->lookAt(Vector3(0,15,0));
    camRotator->yaw(Ogre::Degree(-65));

    auto lvlUp= new LvlMenuUpdate(camRotator,NULL);
    Global::mEventsMgr->addTask(lvlUp);
}

void createTestLevel()
{
	PostProcessMgr* ppMgr = Global::mPPMgr;
	ppMgr->radialHorizBlurVignette = 0.0;

	ppMgr->setAutoGodraySunDirection();

	new CrowWatch();
}

void createTestLevel2()
{
	PostProcessMgr* ppMgr = Global::mPPMgr;
	ppMgr->radialHorizBlurVignette = 0.0;

	ppMgr->setAutoGodraySunDirection();

	new CrowWatch();
}

void createLevelTuto()
{
    Global::mSceneMgr->setSkyBox(true, "TCENoonSkyBoxToDark");

    irrklang::ISound* music = Global::soundEngine->play2D(AudioLibrary::getPath("tokyostreetatnight.ogg").c_str(),true , false, true, irrklang::ESM_AUTO_DETECT, true);
    //music->setMinDistance(20);
    //music->setMaxDistance(100);
    music->setVolume(0.4);
    (*Global::globalData)["tokyostreetatnight.ogg"] = music;

    //Global::mSceneMgr->setVisibilityMask(1);

    auto ppMgr = Global::mPPMgr;
    ppMgr->setToScaryBloom();
    ppMgr->ContSatuSharpNoise = 0.0;
    ppMgr->radialHorizBlurVignette = 0.0;
    ppMgr->ColouringShift = Ogre::Vector4(0.95, 0.98, 0.83, 0);
    ppMgr->setGodraySunPositionFar(Vector3(300, 300, 400) * Vector3(400, -300, -400));

    //Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("TCENoonSkyBoxToDark");
    //mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("dark", 1);

    //BridgeMaker* bm=new BridgeMaker(GlobalPointer->mSceneMgr,GlobalPointer->mWorld);
    //bm->makeBridge(Vector3(49.1,15.13,25.6),Vector3(59.1,15.13,35.6),1,0.5);

    /*
    Entity *myTree = mSceneMgr->createEntity("MyTree", "default.mesh");
    myTree->setMaterialName("woody");
    myTree->setCastShadows(false);

    Forests::PagedGeometry *trees = new Forests::PagedGeometry();
    trees->setCamera(mSceneMgr->getCamera("Camera"));	//Set the camera so PagedGeometry knows how to calculate LODs
    trees->setPageSize(80);	//Set the size of each page of geometry
    trees->setInfinite();		//Use infinite paging mode
    trees->addDetailLevel<Forests::BatchPage>(150, 50);		//Use batches up to 150 units away, and fade for 30 more units
    trees->addDetailLevel<Forests::ImpostorPage>(500, 50);	//Use impostors up to 400 units, and for for 50 more units
    Forests::TreeLoader3D *treeLoader = new Forests::TreeLoader3D(trees, Forests::TBounds(0, 0, 1500, 1500));
    trees->setPageLoader(treeLoader);	//Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance
    */

    // Ogre::StaticGeometry *sg = mSceneMgr->createStaticGeometry("GrassArea");
    // sg->setRegionDimensions(Ogre::Vector3(200, 200, 200));
//sg->setOrigin(Ogre::Vector3(0, 0, 0));
    /*
    	for(int i=-5;i<5;i++)
    		for(int j=0;j<10;j++)
    		{
    				//Ogre::MeshPtr m=MeshManager::getSingleton().load( "default.mesh", "General");
    				//Ogre::MeshPtr m2=MeshManager::getSingleton().load( "defaultv2.mesh", "General");
    				//m.get()->createManualLodLevel(50,m2.get()->getName());
    			Entity* e=mSceneMgr->createEntity(Ogre::StringConverter::toString(j)+Ogre::StringConverter::toString(i)+"aaa","default.mesh");

    			OgreNewt::BasicRaycast ray (www,Vector3(i*25,100,j*25),Vector3(i*25,-50,j*25)  ,true);
    		    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);
    			Real d=0;
    			if (info.mBody)
    			{
    				d=(100+(-150)*info.mDistance);
    			}

    			Quaternion q;
    			q.FromAngleAxis(Ogre::Radian(Ogre::Math::RangeRandom(0,3)),Vector3(0.1,1,0.1));
    			if(d!=0){
    			SceneNode* s=mSceneMgr->getRootSceneNode()->createChildSceneNode();
    			s->attachObject(e);
    			s->setPosition(Vector3(i*25,d , j*25));
    			s->setOrientation(q);
    			//e->setCastShadows(false);
    				//treeLoader->addTree(e, Vector3(i*25,d , j*25), q.getYaw(), 1);
    			}

    			//sg->addEntity(e, Vector3(i*25,d , j*25),q , Vector3(1,1,1));
    		}
    */
    //sg->setCastShadows(true);
    //sg->build();

    /*
    	Forests::PagedGeometry *grass = new Forests::PagedGeometry(mSceneMgr->getCamera("Camera"), 50);
    	grass->addDetailLevel<Forests::GrassPage>(200,20);
    	Forests::GrassLoader *grassLoader = new Forests::GrassLoader(grass);
    	grassLoader->setHeightFunction(&HeightFunction::getTerrainHeight);
    	grass->setPageLoader(grassLoader);*/
    /*
    SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    Animation* anim = mSceneMgr->createAnimation("CameraTrack", 40);
    anim->setInterpolationMode(Animation::IM_SPLINE);
    NodeAnimationTrack* track = anim->createNodeTrack(0, node);
    track->createNodeKeyFrame(0)->setTranslate(Vector3(200, 100, 0));
    track->createNodeKeyFrame(10)->setTranslate(Vector3(0, 100, 200));
    track->createNodeKeyFrame(20)->setTranslate(Vector3(-200, 100, 0));
    track->createNodeKeyFrame(30)->setTranslate(Vector3(0, 100, -200));
    track->createNodeKeyFrame(40)->setTranslate(Vector3(200, 100, 0));
    AnimationState* mAnimState = mSceneMgr->createAnimationState("CameraTrack");
    mAnimState->setEnabled(true);*/
    //p->setBaloonTrack(node);

    //Baloon* b=new Baloon(mSceneMgr,mWorld,Vector3(0,20,0));

    /*Forests::GrassLayer *layer = grassLoader->addLayer("grass");
    layer->setMinimumSize(2.0f, 2.0f);
    layer->setMaximumSize(2.5f, 2.5f);
    layer->setAnimationEnabled(true);
    layer->setSwayDistribution(10.0f);
    layer->setSwayLength(0.5f);
    layer->setSwaySpeed(0.5f);
    layer->setDensity(1.5f);
    layer->setFadeTechnique(Forests::FADETECH_ALPHA);
    layer->setMapBounds(Forests::TBounds(22, 318,81,368));
    layer->setDensityMap("teren.png");
    //layer->setColorMap("lm.png");
    layer = grassLoader->addLayer("grass");
    layer->setMinimumSize(2.0f, 2.0f);
    layer->setMaximumSize(2.5f, 2.5f);
    layer->setAnimationEnabled(true);
    layer->setSwayDistribution(10.0f);
    layer->setSwayLength(0.5f);
    layer->setSwaySpeed(0.5f);
    layer->setDensity(0.5f);
    layer->setFadeTechnique(Forests::FADETECH_ALPHA);
    layer->setMapBounds(Forests::TBounds(22, 318,81,368));
    //layer->setColorMap("lm.png");
    layer->setDensityMap("teren.png");*/
    /*
    	Forests::PagedGeometry *grass = new Forests::PagedGeometry(mSceneMgr->getCamera("Camera"), 50);
    	grass->addDetailLevel<Forests::GrassPage>(120,5);
    	//grass->addDetailLevel<Forests::ImpostorPage>(200,25);
    	Forests::GrassLoader *grassLoader = new Forests::GrassLoader(grass);
    	grassLoader->setHeightFunction(&HeightFunction2::getTerrainHeight);
    	grass->setPageLoader(grassLoader);
    	Forests::GrassLayer *layer = grassLoader->addLayer("grassHop");
    	layer->setMinimumSize(1.0f, 1.0f);
    	layer->setMaximumSize(1.5f, 1.5f);
    	layer->setAnimationEnabled(true);
    	layer->setSwayDistribution(3.0f);
    	layer->setSwayLength(0.4f);
    	layer->setSwaySpeed(0.3f);
    	layer->setDensity(1.5f);
    	layer->setFadeTechnique(Forests::FADETECH_ALPHA);
    	layer->setMapBounds(Forests::TBounds(-300, -300,300,300));
    	layer->setMaxSlope(Ogre::Radian(Ogre::Degree(50)));
    	layer = grassLoader->addLayer("grassHop");
    	layer->setMinimumSize(1.0f, 1.0f);
    	layer->setMaximumSize(1.5f, 1.5f);
    	layer->setAnimationEnabled(true);
    	layer->setSwayDistribution(3.0f);
    	layer->setSwayLength(0.3f);
    	layer->setSwaySpeed(0.2f);
    	layer->setDensity(2.5f);
    	layer->setFadeTechnique(Forests::FADETECH_ALPHA);
    	layer->setMapBounds(Forests::TBounds(-300, -300,300,300));
    	layer->setMaxSlope(Ogre::Radian(Ogre::Degree(50)));
    */
    /*
    	int o=0;
    	for(int i=0;i<6;i++)
    		for(int j=0;j<3
    			;j++)
    		{


    		OgreNewt::BasicRaycast ray(www,Vector3(i*15,0,j*15),Vector3(i*15,-50,j*15)  ,true);
    		OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

    			if (info.mBody)
    			{
    			Entity *myTree = mSceneMgr->createEntity("MyTree"+Ogre::StringConverter::toString(o++), "default.mesh");
    			Ogre::SceneNode* nn=mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(i*15,(-50)*info.mDistance+7.5, j*15));
    			nn->attachObject(myTree);
    			nn->yaw(Ogre::Radian(Ogre::Degree(180)));
    			}
    		}*/

    //loadScene("../../media/testxml2.scene",mSceneMgr,mWorld,NULL,flag_mat);

    /*SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    Animation* anim = mSceneMgr->createAnimation("CameraTrack", 15);
    anim->setInterpolationMode(Animation::IM_SPLINE);
    NodeAnimationTrack* track = anim->createNodeTrack(0, node);
    track->createNodeKeyFrame(0)->setTranslate(Vector3(100, 50, 0));
    track->createNodeKeyFrame(5)->setTranslate(Vector3(0, 150, 100));
    track->createNodeKeyFrame(10)->setTranslate(Vector3(-100, 50, 0));
    track->createNodeKeyFrame(12)->setTranslate(Vector3(0, 30, 0));
    track->createNodeKeyFrame(15)->setTranslate(Vector3(100, 50, 0));
    AnimationState* mAnimState = mSceneMgr->createAnimationState("CameraTrack");
    mAnimState->setEnabled(true);

    Bird* b=new Bird(mSceneMgr,mWorld,Vector3(200,100,0),node,flag_mat);

    node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    anim = mSceneMgr->createAnimation("CameraTrack2", 20);
    anim->setInterpolationMode(Animation::IM_SPLINE);
    track = anim->createNodeTrack(0, node);
    track->createNodeKeyFrame(0)->setTranslate(Vector3(50, 200, 0));
    track->createNodeKeyFrame(5)->setTranslate(Vector3(0, 150, 100));
    track->createNodeKeyFrame(10)->setTranslate(Vector3(100, 50, 0));
    track->createNodeKeyFrame(15)->setTranslate(Vector3(-20, 40, 30));
    track->createNodeKeyFrame(20)->setTranslate(Vector3(50, 200, 0));
    mAnimState = mSceneMgr->createAnimationState("CameraTrack2");
    mAnimState->setEnabled(true);

    b=new Bird(mSceneMgr,mWorld,Vector3(50,200,0),node,flag_mat);
    */

}

void createLevel1_1()
{

    //Rope* rr=new Rope(mSceneMgr,mWorld,Vector3(13,11,6),10,0.5);

    Global::mSceneMgr->setSkyBox(true, "TCENoonSkyBox");

    irrklang::ISound* music = Global::soundEngine->play2D(AudioLibrary::getPath("anton_wind1.wav").c_str(),true , false, true, irrklang::ESM_AUTO_DETECT, true);
    //music->setMinDistance(20);
    //music->setMaxDistance(100);

    music->setVolume(0.4);
    (*Global::globalData)["anton_wind1.wav"] = music;

    //Global::mSceneMgr->setVisibilityMask(1);

    PostProcessMgr* ppMgr = Global::mPPMgr;
    ppMgr->setToScaryBloom();
    ppMgr->ContSatuSharpNoise = 0.0;
    ppMgr->radialHorizBlurVignette = 0.0;
    ppMgr->ColouringShift = Ogre::Vector4(0.95, 0.89, 1, 0);

    ppMgr->setGodraySunPositionFar(Vector3(300, 300, 400) * Vector3(400, -300, -400));
}

void createLevel2()
{
    Global::mSceneMgr->setSkyBox(true, "TCENoonSkyBox");


    irrklang::ISound* music = Global::soundEngine->play2D(AudioLibrary::getPath("anton_wind1.wav").c_str(), true, false, true, irrklang::ESM_AUTO_DETECT, true);
    //music->setMinDistance(20);
    //music->setMaxDistance(100);

    music->setVolume(0.4);
    (*Global::globalData)["anton_wind1.wav"] = music;

    PostProcessMgr* ppMgr = Global::mPPMgr;
    ppMgr->setToScaryBloom();
    ppMgr->ContSatuSharpNoise = 0.0;
    ppMgr->radialHorizBlurVignette = 0.0;
    ppMgr->ColouringShift = Ogre::Vector4(0.95, 0.89, 1, 0);

    ppMgr->setAutoGodraySunDirection();
}