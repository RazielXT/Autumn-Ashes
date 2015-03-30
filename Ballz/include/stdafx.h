// stdafx.h

// Ogre headers you need
#include <Ogre.h>
#include <OIS.h>
#include <OgreException.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <OgreNewt.h>
#include <irrKlang.h>
#include "PagedGeometry.h"
#include "BatchPage.h"
#include "ImpostorPage.h"
#include "TreeLoader3D.h"
#include "GrassLoader.h"
#include "EventsManager.h"
#include "globals.h"
#include "GameConfig.h"
#include "GuiOverlay.h"
#include "WorldMaterials.h"
#include "AudioLibrary.h"

// any other header can be included, as usual
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif