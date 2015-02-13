#include "stdafx.h"
#include "lvl1Tasks.h"
#include "PostProcessMgr.h"
#include "player.h"

ParkToDreamReaction::ParkToDreamReaction()
{
}

void ParkToDreamReaction::setUserData(void* data)
{
}

bool ParkToDreamReaction::start()
{
    Global::mPPMgr->colourIn(Ogre::Vector3(1,1,1),1);
    timer=0;

    return true;
}

bool ParkToDreamReaction::update(Ogre::Real tslf)
{
    timer+=tslf;

    if(timer>1.1f)
    {
        PostProcessMgr* mgr = Global::mPPMgr;
        mgr->ColouringShift.x = 0.55;
        mgr->ColouringShift.y = 1.0;
        mgr->ColouringShift.z = 1.7;

        mgr->colourOut(Ogre::Vector3(1, 1, 1), 2);
        Global::mSceneMgr->setVisibilityMask(2);
        return false;
    }

    return true;
}


NearParkEndReaction::NearParkEndReaction()
{
}

void NearParkEndReaction::setUserData(void* data)
{
}

bool NearParkEndReaction::start()
{
    doneEffect=doneEffect0=false;
    mgr = Global::mPPMgr;
    Global::soundEngine->play2D("../../media/oneHearthbeat.ogg",false , false, false, irrklang::ESM_AUTO_DETECT, false);
    noiseSound = Global::soundEngine->play2D("../../media/tvnoise.ogg",true , true, true, irrklang::ESM_AUTO_DETECT, false);
    noiseSound->setVolume(0);
    noiseSound->setIsPaused(false);
    (*Global::globalData)["tvnoise.ogg"] = noiseSound;
    timer=0;

    mgr->ColouringShift.x = 1.2;
    mgr->ColouringShift.y = 1.1;
    mgr->ColouringShift.z = 0;

    return true;
}

bool NearParkEndReaction::update(Ogre::Real tslf)
{
    timer+=tslf;

    if(!doneEffect)
    {
        float val = std::min(timer,0.5f-timer)*20;

        mgr->ContSatuSharpNoise.w = val;
        mgr->ColouringShift.w += tslf*2;
        mgr->radialHorizBlurVignette.y = val;
        mgr->radialHorizBlurVignette.x += tslf*1.5f;

        if(!doneEffect0 && timer>0.25f)
        {
            Global::mSceneMgr->setVisibilityMask(4);
            Global::player->stopMoving();
            Global::player->rotateCamera(180,0);
            doneEffect0=true;
        }

        if(timer>0.5f)
        {
            mgr->ContSatuSharpNoise.w = 0;
            mgr->ColouringShift.w = 1;
            mgr->radialHorizBlurVignette.x = 0.75f;
            mgr->radialHorizBlurVignette.y = 0;
            Global::timestep = 0.4f;
            doneEffect = true;
        }
    }
    else
    {

        if(!noiseSound->isFinished())
        {
            float distance = (Ogre::Vector3(17.6,14.34,151.26)-Global::player->getBody()->getPosition()).length();
            float val = std::max(0.0f,1.0f-distance/50.0f);
            noiseSound->setVolume(val*0.5f);
            mgr->ContSatuSharpNoise.w = val;
            mgr->ContSatuSharpNoise.z = val*0.3f;
        }
        else
        {
            mgr->ContSatuSharpNoise.w = 0;
            mgr->ContSatuSharpNoise.z = 0;
            noiseSound->drop();
            return false;
        }

    }

    return true;
}



ParkEndReaction::ParkEndReaction()
{
}

void ParkEndReaction::setUserData(void* data)
{
}

bool ParkEndReaction::start()
{
    mgr = Global::mPPMgr;
    Global::soundEngine->play2D("../../media/oneHearthbeat.ogg",false , false, false, irrklang::ESM_AUTO_DETECT, false);

    ((irrklang::ISound*)Global::globalData->find("tvnoise.ogg")->second)->stop();
    doneEffect1 = doneEffect2 = doneEffect3 = false;
    timer=0;

    mAnim1 = Global::mSceneMgr->getEntity("Ruka5")->getAnimationState("prsty");
    mAnim1->setLoop(false);
    mAnim1->setEnabled(true);
    mAnim1->setTimePosition(0.3);

    mAnim2 = Global::mSceneMgr->getEntity("Ruka2")->getAnimationState("prsty");
    mAnim2->setLoop(false);
    mAnim2->setEnabled(true);
    mAnim2->setTimePosition(0.35);

    mAnim3 = Global::mSceneMgr->getEntity("Ruka3")->getAnimationState("prsty");
    mAnim3->setLoop(false);
    mAnim3->setEnabled(true);
    mAnim3->setTimePosition(0.2);

    mAnim4 = Global::mSceneMgr->getEntity("Ruka4")->getAnimationState("prsty");
    mAnim4->setLoop(false);
    mAnim4->setEnabled(true);
    mAnim4->setTimePosition(0.15);

    return true;
}

bool ParkEndReaction::update(Ogre::Real tslf)
{
    timer+=tslf;

    if(!doneEffect1)
    {
        float val = std::min(timer,0.5f-timer)*20;

        mgr->ContSatuSharpNoise.w = val;
        mgr->ColouringShift.w += tslf*2;
        mgr->radialHorizBlurVignette.y = val;
        mgr->radialHorizBlurVignette.x += tslf*1.5f;

        if(timer>0.5f)
        {
            mgr->ContSatuSharpNoise.w = 0;
            mgr->ColouringShift.w = 1;
            mgr->radialHorizBlurVignette.x = 0.75f;
            mgr->radialHorizBlurVignette.y = 0;
            Global::timestep = 1.0f;
            Global::player->enableMovement(false);
            Global::player->stopMoving();
            Global::player->getBody()->setPositionOrientation(Ogre::Vector3(17.6,12.5,158.5),Global::player->getBody()->getOrientation());
            doneEffect1=true;
        }
    }
    else
    {
        if(timer>=2.0f)
        {
            if(!doneEffect2)
            {
                Global::mSceneMgr->setVisibilityMask(8);
                Global::soundEngine->play2D("../../media/blaze_explotion.wav",false , false, false, irrklang::ESM_AUTO_DETECT, false);
                sound = Global::soundEngine->play2D("../../media/scaryScreak1.wav",false , false, true, irrklang::ESM_AUTO_DETECT, false);

                doneEffect2=true;
            }

            float relTimer = timer-2.0f;
            float val0 = Ogre::Math::Clamp(std::min(relTimer*10,2.0f-relTimer),0.0f,1.0f);
            float val1 = relTimer/2.0f;

            mgr->ContSatuSharpNoise.x = val1;
            mgr->ContSatuSharpNoise.y = val1;
            mgr->ColouringShift.w = val0;
            mgr->radialHorizBlurVignette.y = val0;
            Global::timestep = 1.0f - val1;

            sound->setVolume(Global::timestep);

            if(Global::timestep==0)
                sound->drop();

            //Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("TCENoonSkyBoxToDark");
            Ogre::Vector4 pp = Ogre::Vector4::ZERO;
            //mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("dark", pp);
            Global::mSceneMgr->setAmbientLight(Ogre::ColourValue(1*Global::timestep,1*Global::timestep,1*Global::timestep,0));
            if(timer>3.0f)
            {
                mAnim1->addTime(tslf*Global::timestep);
                mAnim2->addTime(tslf*Global::timestep);
                mAnim3->addTime(tslf*Global::timestep);
                mAnim4->addTime(tslf*Global::timestep);

                if(!doneEffect3)
                {
                    Global::mSceneMgr->setVisibilityMask(16);
                    doneEffect3=true;
                }
            }

            if(timer>4.0f)
            {

                mgr->ContSatuSharpNoise.x = 1;
                mgr->ContSatuSharpNoise.y = 1;
                mgr->ColouringShift.w = 0;
                mgr->radialHorizBlurVignette.y = 0;

                Global::timestep = 1.0f;
                Global::player->enableMovement(true);
                return false;
            }
        }
    }

    return true;
}

OutOfCave::OutOfCave()
{
}

void OutOfCave::setUserData(void* data)
{
}

bool OutOfCave::start()
{
    auto bodies = (std::map<Ogre::String,OgreNewt::Body*>*) Global::globalData->find("loadedBodies")->second;
    auto it = bodies->find("FCave");
    delete it->second;
    bodies->erase(it);
    it = bodies->find("ORock1");
    it->second->setMaterialGroupID(Global::mWorld->getDefaultMaterialID());

    Global::mSceneMgr->getSceneNode("FCave")->detachAllObjects();
    Global::mSceneMgr->getSceneNode("Plane01")->detachAllObjects();
    Global::mSceneMgr->getSceneNode("Plane02")->detachAllObjects();

    return false;
}