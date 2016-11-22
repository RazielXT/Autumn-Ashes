#include "stdafx.h"
#include "DebugKeys.h"
#include "PlayerCamera.h"
#include "GameStateManager.h"
#include "PlayerSwimming.h"
#include "Player.h"
#include "MUtils.h"
#include "GUtils.h"

void DebugKeys::pressedKey(const OIS::KeyEvent &arg)
{
	auto postProcMgr = Global::ppMgr;
	static Ogre::Vector3 debugPos;
	static float shake[8] = {0};
	static float shakesign = 0.1f;

	if (Global::gameMgr->myMenu->pressedKey(arg))
		return;

	switch (arg.key)
	{

	case OIS::KC_C:
	{
		debugPos = Global::player->bodyPosition;
	}
	break;

	case OIS::KC_V:
	{
		Global::player->setPosition(debugPos);
	}
	break;

	case OIS::KC_NUMPAD8:
	{
		Global::gameMgr->myMenu->showOptGroupDebug();
	}
	break;

	case OIS::KC_NUMPAD7:
	{
		Global::gameMgr->myMenu->showDetailGeometryDebug();
	}
	break;

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

	case OIS::KC_NUMPAD3:
	{
		Global::gameMgr->reloadMeshes();
	}
	break;

	case OIS::KC_P:
	{
		shakesign *= -1;
	}
	break;

	case OIS::KC_9:
	{
		shake[0] = 1;
		shake[1] = 2;
		shake[2] = 0.5;
		shake[3] = 1;
		shake[4] = 1;
		shake[5] = 0.1;
		shake[6] = 0;
		shake[7] = 1;

		std::string txt;
		for (int i = 0; i < 8; i++)
			txt += std::to_string(shake[i]) + " ";

		GUtils::DebugPrint(txt);
	}
	break;

	case OIS::KC_1:
	{
		shake[0] += shakesign;

		std::string txt;
		for (int i = 0; i < 8; i++)
			txt += std::to_string(shake[i]) + " ";

		GUtils::DebugPrint(txt);
	}
	break;

	case OIS::KC_2:
	{
		shake[1] += shakesign;

		std::string txt;
		for (int i = 0; i < 8; i++)
			txt += std::to_string(shake[i]) + " ";

		GUtils::DebugPrint(txt);
	}
	break;

	case OIS::KC_3:
	{
		shake[2] += shakesign;

		std::string txt;
		for (int i = 0; i < 8; i++)
			txt += std::to_string(shake[i]) + " ";

		GUtils::DebugPrint(txt);
	}
	break;

	case OIS::KC_4:
	{
		shake[3] += shakesign;

		std::string txt;
		for (int i = 0; i < 8; i++)
			txt += std::to_string(shake[i]) + " ";

		GUtils::DebugPrint(txt);
	}
	break;

	case OIS::KC_5:
	{
		shake[4] += shakesign;

		std::string txt;
		for (int i = 0; i < 8; i++)
			txt += std::to_string(shake[i]) + " ";

		GUtils::DebugPrint(txt);
	}
	break;

	case OIS::KC_6:
	{
		shake[5] += shakesign;

		std::string txt;
		for (int i = 0; i < 8; i++)
			txt += std::to_string(shake[i]) + " ";

		GUtils::DebugPrint(txt);
	}
	break;

	case OIS::KC_7:
	{
		shake[6] += shakesign;

		std::string txt;
		for (int i = 0; i < 8; i++)
			txt += std::to_string(shake[i]) + " ";

		GUtils::DebugPrint(txt);
	}
	break;

	case OIS::KC_8:
	{
		shake[7] += shakesign;

		std::string txt;
		for (int i = 0; i < 8; i++)
			txt += std::to_string(shake[i]) + " ";

		GUtils::DebugPrint(txt);
	}
	break;

	case OIS::KC_0:
	{
		Global::player->pCamera->shaker.startShaking(shake[0], shake[1], shake[2], shake[3], shake[4], shake[5], shake[6], shake[7]);
	}
	break;

	case OIS::KC_I:
	{
		static auto imgs = Global::ppMgr->getColorGradingPresets();
		static int id = 0;

		id = (id + 1) % imgs.size();

		Global::ppMgr->setColorGradingPreset(imgs[id]);
		GUtils::DebugPrint(imgs[id]);
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
			Global::eventsMgr->addTask(task);
		break;
	}
	case OIS::KC_N:
	{
		auto task = new SwitchColorSchemeFx("1.0,0.95,0.85,2");
		if (task->start())
			Global::eventsMgr->addTask(task);
		break;
	}

	case OIS::KC_DELETE:
		GUtils::debug.clear();
		break;

	/*case OIS::KC_1:
		Global::player->pCamera->shaker.startShaking(0.3, 1.5, 0.0, 1, 1, 0.7, 0.55, 0.5, true);
		break;

	case OIS::KC_2:
		Global::camera->shaker.startShaking(1.5, 1.0, 0.1, 1, 1, 0.7, 0.15, 0.75, true);
		break;

	case OIS::KC_3:
		Global::camera->shaker.startShaking(1.0, 1.5, 0.5, 1, 1, 0.5, 0.35, 1, true);
		break;

	case OIS::KC_4:
		Global::camera->shaker.startShaking(1.0, 1.5, 5, 1, 1, 0.1, 1.35, 1, true);
		break;

	case OIS::KC_5:
		Global::camera->shaker.startShaking(1.2, 1.5, 10, 1, 1, 0.1, 0.35, 1, true);
		break;*/

	case OIS::KC_NUMPADENTER:
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

	Entity* ent = Global::sceneMgr->createEntity("boxEL.mesh");
	SceneNode* node = Global::sceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(ent);
	node->setScale(size);
	ent->setCastShadows(true);
	ent->setMaterialName("planksVC");
	OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(Global::nWorld, size, 0));
	OgreNewt::Body* body = new OgreNewt::Body(Global::nWorld, col);

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
