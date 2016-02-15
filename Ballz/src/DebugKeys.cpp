#include "stdafx.h"
#include "DebugKeys.h"
#include "CameraShaker.h"
#include "GameStateManager.h"
#include "PlayerSwimming.h"
#include "Player.h"
#include "MUtils.h"
#include "GUtils.h"

void DebugKeys::pressedKey(const OIS::KeyEvent &arg)
{
    auto postProcMgr = Global::mPPMgr;

    if (Global::gameMgr->myMenu->pressedKey(arg))
        return;

    switch (arg.key)
    {

    case OIS::KC_NUMPAD6:
    {
        Global::gameMgr->myMenu->showMaterialDebug();
    }
    break;

    case OIS::KC_NUMPAD5:
    {
        Global::gameMgr->myMenu->showParticleDebug();
    }
    break;

    case OIS::KC_NUMPAD4:
    {
        Global::gameMgr->myMenu->showLevelDebug();
    }
    break;

    case OIS::KC_NUMPAD1:
    {
        if (Global::gameMgr->gameState == GAME)
            makecube(true);
    }
    break;

    case OIS::KC_NUMPAD2:
    {
        reloadShaders();
    }
    break;

    case OIS::KC_M:
    {
        if (Global::gameMgr->gameState == GAME)
            makecube();
    }
    break;

    case OIS::KC_SLASH:
    {
        Global::gameMgr->reloadLevel();
    }
    break;

    case OIS::KC_B:
    {
        auto task = new SwitchColorSchemeFx("0.8,0.95,1.05,2");
        if (task->start())
            Global::mEventsMgr->addTask(task);
        break;
    }
    case OIS::KC_N:
    {
        auto task = new SwitchColorSchemeFx("1.0,0.95,0.85,2");
        if (task->start())
            Global::mEventsMgr->addTask(task);
        break;
    }

    case OIS::KC_DELETE:
        GUtils::debug.clear();
        break;

    case OIS::KC_1:
        Global::shaker->startShaking(0.3, 1.5, 0.0, 1, 1, 0.7, 0.55, 0.5, true);
        break;

    case OIS::KC_2:
        Global::shaker->startShaking(1.5, 1.0, 0.1, 1, 1, 0.7, 0.15, 0.75, true);
        break;

    case OIS::KC_3:
        Global::shaker->startShaking(1.0, 1.5, 0.5, 1, 1, 0.5, 0.35, 1, true);
        break;

    case OIS::KC_4:
        Global::shaker->startShaking(1.0, 1.5, 5, 1, 1, 0.1, 1.35, 1, true);
        break;

    case OIS::KC_5:
        Global::shaker->startShaking(1.2, 1.5, 10, 1, 1, 0.1, 0.35, 1, true);
        break;

    case OIS::KC_NUMPAD3:
        if (Global::gameMgr->gameState == MENU)
        {
            Global::gameMgr->switchToLevel(5);
        }
        else
            continueExecution = false;

        break;

    case OIS::KC_NUMPAD0:
        postProcMgr->resetValues();
        break;
    }
}

void DebugKeys::reloadShaders()
{
    //std::string matsFile("lvl2.material");

    /*std::vector<std::string> reloadedMats;
    auto mit = MaterialManager::getSingleton().getResourceIterator();

    while (mit.hasMoreElements())
    {
    MaterialPtr mat = mit.getNext();
    mat->reload();

    if (mat->getOrigin() == matsFile)
    reloadedMats.push_back(mat->getName());
    }*/
    /*
    Ogre::SceneManager::MovableObjectIterator mIterator = Global::mSceneMgr->getMovableObjectIterator("Entity");
    std::vector<std::pair<Ogre::Entity*, std::string>> ents;

    while (mIterator.hasMoreElements())
    {
    auto e = (Ogre::Entity*)mIterator.getNext();

    if (e->getNumSubEntities() > 0)
    {
    auto sub = e->getSubEntity(0);
    auto matName = sub->getMaterialName();
    auto mat = sub->getMaterial();

    //if (std::find(reloadedMats.begin(), reloadedMats.end(), mat) != reloadedMats.end())
    if (mat->getOrigin() == matsFile)
    {
    sub->setMaterialName("BaseWhite");
    mat->unload();

    ents.push_back(std::make_pair(e, matName));
    }
    }
    }

    MaterialManager::getSingleton().parseScript(Ogre::ResourceGroupManager::getSingleton().openResource(matsFile), "General");

    for (auto& p : ents)
    {
    p.first->setMaterialName(p.second);
    }
    */
    auto it = HighLevelGpuProgramManager::getSingleton().getResourceIterator();

    while (it.hasMoreElements())
    {
        HighLevelGpuProgramPtr gpuProgram = it.getNext();
        gpuProgram->reload();
    }
}

void DebugKeys::makecube(bool platform)
{
    Ogre::Vector3 size(2, 2, 2);
    Ogre::Real mass = platform ? 0 : 0.3f;

    Entity* ent = Global::mSceneMgr->createEntity("boxEL.mesh");
    SceneNode* node = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
    node->setScale(size);
    ent->setCastShadows(true);
    ent->setMaterialName("murbob");
    OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(Global::mWorld, size, 0));
    OgreNewt::Body* body = new OgreNewt::Body(Global::mWorld, col);

    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);

#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    delete col;
#endif

    body->setMassMatrix(mass, mass*inertia);
    body->setCenterOfMass(offset);
    body->attachNode(node);
    body->setType(Grabbable);
    body->setCustomForceAndTorqueCallback<DebugKeys>(&DebugKeys::default_callback, this);
    body->setPositionOrientation(Global::player->bodyPosition + Vector3(0, platform ? -3.0f : 5.0f, 0), Ogre::Quaternion::IDENTITY);
}
