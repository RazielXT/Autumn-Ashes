#pragma once

#include "stdafx.h"
#include <chrono>

namespace GameScene
{

extern std::time_t lastLoadTime;

extern void reloadScene(std::string filename);

extern void reloadMeshes(std::vector<std::string> directories);

extern void loadScene(std::string filename);

};