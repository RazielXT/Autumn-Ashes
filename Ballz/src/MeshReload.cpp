#include "stdafx.h"

#include "GameScene.h"
#include <filesystem>
#include "SUtils.h"
#include "GUtils.h"

namespace filesystem = std::experimental::filesystem;

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

void reloadMeshes(std::string directory, std::string meshPrefix)
{
	std::vector<std::string> newMeshes;

	for (auto& p : filesystem::directory_iterator(directory))
	{
		if (isMesh(p))
		{
			auto ftime = filesystem::last_write_time(p.path());
			auto time = filesystem::file_time_type::clock::to_time_t(ftime);

			if (time > lastLoadTime)
			{
				newMeshes.push_back(p.path().filename().string());
			}
		}
	}

	int reloaded = 0;

	for (auto& meshName : newMeshes)
	{
		if (SUtils::startsWith(meshName, meshPrefix))
		{
			auto name = meshName.substr(0, meshName.find_last_of('.'));
			name = name.substr(meshPrefix.length());

			if (Global::mSceneMgr->hasEntity(name))
			{
				auto e = Global::mSceneMgr->getEntity(name);

				Ogre::MaterialPtr mat = e->getSubEntity(0)->getMaterial();
				e->getMesh()->reload();
				e->setMaterial(mat);
				e->getMesh()->getSubMesh(0)->setMaterialName(mat->getName());

				reloaded++;
			}
		}
	}

	lastLoadTime = std::time(nullptr);
	GUtils::DebugPrint(std::to_string(reloaded) + " meshes reloaded");
}

}