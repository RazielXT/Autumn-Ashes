#pragma once

#include "stdafx.h"
#include <chrono>

namespace GameScene
{

extern std::time_t lastLoadTime;

extern void reloadScene(std::string filename);

extern void reloadMeshes(std::string directory, std::string meshPrefix);

extern void loadScene(std::string filename);

};