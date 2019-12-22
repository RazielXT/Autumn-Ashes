#include "stdafx.h"

#include "GameScene.h"
#include "SUtils.h"
#include "GUtils.h"

#include <boost/filesystem.hpp>

namespace filesystem = boost::filesystem;

namespace GameScene
{

bool isMesh(const filesystem::directory_entry& file)
{
	if (!filesystem::is_regular_file(file.status()))
		return false;

	std::string name = file.path().string();
	auto pos = name.find_last_of('.');

	return name.length() > 4 && pos != std::string::npos && name.substr(pos + 1, std::string::npos) == "mesh";
}

int reloadMeshes(std::string directory)
{
	std::vector<std::string> newMeshes;

	for (auto& p : filesystem::directory_iterator(directory))
	{
		if (isMesh(p))
		{
			auto time = filesystem::last_write_time(p.path());

			if (time > lastLoadTime)
			{
				newMeshes.push_back(p.path().filename().string());
			}
		}
	}

	int reloaded = 0;

	for (auto& meshName : newMeshes)
	{
		/*if (SUtils::startsWith(meshName, meshPrefix))
		{
		auto name = meshName.substr(0, meshName.find_last_of('.'));
		name = name.substr(meshPrefix.length());

		if (Global::sceneMgr->hasEntity(name))
		{
		auto e = Global::sceneMgr->getEntity(name);

		Ogre::MaterialPtr mat = e->getSubEntity(0)->getMaterial();
		e->getMesh()->reload();
		e->setMaterial(mat);
		e->getMesh()->getSubMesh(0)->setMaterialName(mat->getName());

		reloaded++;
		}
		else
		{*/

		Ogre::SceneManager::MovableObjectIterator iterator = Global::sceneMgr->getMovableObjectIterator("Entity");
		while (iterator.hasMoreElements())
		{
			Ogre::Entity* e = static_cast<Ogre::Entity*>(iterator.getNext());
			if (e->getMesh()->getName() == meshName)
			{
				Ogre::MaterialPtr mat = e->getSubEntity(0)->getMaterial();
				e->getMesh()->reload();
				e->setMaterial(mat);
				e->getMesh()->getSubMesh(0)->setMaterialName(mat->getName());
				reloaded++;

				break;
			}
		}
	}

	return reloaded;
}

void reloadMeshes(std::vector<std::string> directories)
{
	int reloaded = 0;

	for (auto& d : directories)
		reloaded += reloadMeshes(d);

	lastLoadTime = std::time(0);
	GUtils::DebugPrint(std::to_string(reloaded) + " meshes reloaded");
}

}