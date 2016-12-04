#include "stdafx.h"
#include "Gorilla.h"
#include "GuiOverlay.h"
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "PostProcessMgr.h"
#include "MUtils.h"
#include "SUtils.h"
#include "GameStateManager.h"
#include "GameUi.h"
#include "GuiSceneEdit.h"
#include "GUtils.h"

using namespace Ogre;

#define OPEN_OPTIONS_SOUND "../../media/oMopen.wav"
#define CLOSE_OPTIONS_SOUND "../../media/oMopen.wav"
#define OPEN_LEVELS_SOUND "../../media/oMopen.wav"
#define CLOSE_LEVELS_SOUND "../../media/oMopen.wav"
#define SWITCH_OPTIONS_SOUND "../../media/oMclick.wav"
#define OVER_OPTIONS_SOUND "../../media/oMmove.wav"
#define SWITCH_MENU_SOUND "../../media/mMchange.wav"
#define LEVEL_START_SOUND "../../media/mMchange.wav"
#define LEVEL_LOCKED_SOUND "../../media/mMchange.wav"
#define LEVEL_SWITCH_SOUND "../../media/mMchange.wav"

void GuiOverlay::showOptions()
{
	soundEngine->play2D(OPEN_OPTIONS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
	atm=380;
	currentMenu=TOOM;
	oMenuState=RESOLUTION;
	movingDir=-1;

	for(auto myIterator = oMenuButtons.begin(); myIterator != oMenuButtons.end(); myIterator++)
	{
		Gorilla::Caption* c=(*myIterator);
		c->colour(Ogre::ColourValue(1,1,1,0.3));
	}

	cButton=firstOptionButton;
	cButton->value->colour(Ogre::ColourValue(1, 1, 1, 1));
	cOptionButtonA=firstOptionButtonA;
	cOptionButtonA->value->colour(Ogre::ColourValue(1, 1, 1, 1));
}

void GuiOverlay::showLevels()
{
	soundEngine->play2D(OPEN_LEVELS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
	atm=340;
	currentMenu=TOSM;
	movingDir=-1;

	for(auto myIterator = lMenuButtons.begin(); myIterator != lMenuButtons.end(); myIterator++)
	{
		Gorilla::Rectangle* c=(*myIterator)->r;
		if(!(*myIterator)->unlocked)
			c->background_colour(Gorilla::Colours::Red);
		Real alpha=1-Math::Abs((c->left()-690)/999.0f);
		c->yes_background(alpha);
	}
}

void GuiOverlay::closeOptions()
{
	soundEngine->play2D(CLOSE_OPTIONS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);

	atm=380;
	currentMenu=FROM;
	movingDir=1;

	for(auto myIterator = oMenuButtons.begin(); myIterator != oMenuButtons.end(); myIterator++)
	{
		Gorilla::Caption* c=(*myIterator);
		c->colour(Ogre::ColourValue(1,1,1,0.3));
	}

	gConfig->saveCfg();

	Global::window->setFullscreen(gConfig->fs, gConfig->width, gConfig->height);
}

void GuiOverlay::closeLevels()
{
	soundEngine->play2D(CLOSE_LEVELS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);

	atm=340;
	currentMenu=FRSM;
	movingDir=1;

	for(auto myIterator = lMenuButtons.begin(); myIterator != lMenuButtons.end(); myIterator++)
	{
		Gorilla::Rectangle* c=(*myIterator)->r;
		c->yes_background(0.3);
	}
}

void GuiOverlay::clear()
{
	delete mMenuState;
	delete gMenuState;
	delete cButton;
	delete cOptionButtonA;
	delete resolutionsLoop;
}

bool GuiOverlay::pressedKey(const OIS::KeyEvent &arg)
{
	if (editUi->pressedKey(arg))
		return true;

	if (currentMenu == MAINM && !moving)
	{
		switch (arg.key)
		{
		case OIS::KC_LEFT:
			if (!moving)
			{
				movingDir = 1;
				moving = true;
				mMenuState = mMenuState->previous;
			}
			break;
		case OIS::KC_RIGHT:
			if (!moving)
			{
				movingDir = -1;
				moving = true;
				mMenuState = mMenuState->next;
			}
			break;
		case OIS::KC_RETURN:
			if (mMenuState->value == QUIT)
			{
				gConfig->saveCfg();
			}
			if (mMenuState->value == OPTIONS)
			{
				showOptions();
			}
			break;
		default:
			return false;
		}
	}
	else
		return false;

	return true;
}

GuiOverlay::GuiOverlay(GameConfig* gameConfig, Ogre::Camera* mCam, Ogre::RenderWindow* mWin, Ogre::RenderSystem* rs)
{
	editUi = new GuiSceneEdit();

	soundEngine = Global::soundEngine;

	gConfig = gameConfig;

	Ogre::ConfigOptionMap& CurrentRendererOptions = rs->getConfigOptions();
	Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin();
	Ogre::StringVector mFoundResolutions;
	while( configItr != CurrentRendererOptions.end() )
	{
		if( (configItr)->first == "Video Mode" )
		{
			// Store Available Resolutions
			mFoundResolutions = ((configItr)->second.possibleValues);
		}
		configItr++;
	}

	int i=mFoundResolutions.size();
	std::string currentR=Ogre::StringConverter::toString(gConfig->width)+"x"+Ogre::StringConverter::toString(gConfig->height);

	cButton = cOptionButtonA = nullptr;

	resolutionsLoop= new ListLoop<resolution>();
	std::string s=mFoundResolutions.at(0);

	std::string s1=SUtils::strtok_str(s,' ');
	SUtils::strtok_str(s, ' ');
	std::string s2 = SUtils::strtok_str(s, ' ');
	std::string res=s1+"x"+s2;
	resolutionsLoop->value.txt = res;
	resolutionsLoop->value.w = Ogre::StringConverter::parseInt(s1);
	resolutionsLoop->value.h = Ogre::StringConverter::parseInt(s2);

	for(int o=1; o<i; o++)
	{
		std::string s=mFoundResolutions.at(o);

		std::string s1 = SUtils::strtok_str(s, ' ');
		SUtils::strtok_str(s, ' ');
		std::string s2 = SUtils::strtok_str(s, ' ');
		std::string res=s1+"x"+s2;
		resolution trLoopN;
		trLoopN.w=Ogre::StringConverter::parseInt(s1);
		trLoopN.h=Ogre::StringConverter::parseInt(s2);
		trLoopN.txt=res;

		auto added = resolutionsLoop->addToEnd(trLoopN);

		if (Ogre::StringUtil::match(currentR, res))
			resolutionsLoop = added;
	}

	Ogre::LogManager::getSingleton().getDefaultLog()->logMessage("Possible resolutions: ",Ogre::LML_NORMAL);
	for(int o=0; o<i; o++)
	{
		Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(resolutionsLoop->value.txt,Ogre::LML_NORMAL);
		resolutionsLoop=resolutionsLoop->next;
	}

	mMenuState = new ListLoop<mMenuEnum>(START);
	mMenuState->addToEnd(OPTIONS);
	mMenuState->addToEnd(QUIT);

	oMenuState= RESOLUTION;

	gMenuState = new ListLoop<mMenuEnum>(RESUME);
	gMenuState->addToEnd(RESTART);
	gMenuState->addToEnd(OPTIONS);
	gMenuState->addToEnd(QUIT);
	gMenuState->addToEnd(START);

	currentMenu=MAINM;

	moving=false;
	atm=0;
	movingDir=1;
	mSceneMgr = Global::sceneMgr;
	lvlMoving=false;

	mSilverback = new Gorilla::Silverback();
	mSilverback->loadAtlas("dejavu");
	mSilverback->loadAtlas("lvlsThumb");

	mScreenLvls = mSilverback->createScreen(mCam->getViewport(), "lvlsThumb");
	mScreenLvls->setOrientation(Ogre::OrientationMode::OR_DEGREE_0);
	mScreen = mSilverback->createScreen(mCam->getViewport(), "dejavu");
	mScreen->setOrientation(Ogre::OrientationMode::OR_DEGREE_0);

	Ogre::Real vpW = mScreen->getWidth(), vpH = mScreen->getHeight();

	mLayer = mScreen->createLayer(1);
	mLvlsLayer = mScreenLvls->createLayer(1);
	mouseLayer = mScreen->createLayer(2);
	dbgLayer = mScreen->createLayer(3);

	gameLayer = mScreen->createLayer(0);
	gameUi = new GameUi(gameLayer);

	mousePointer = mouseLayer->createRectangle(0,0,41,42);
	mousePointer->background_image("mousepointer");

	fpsCaption = dbgLayer->createCaption(48, 100, 5, "0");
	fpsCaption->size(1500,50);
	fpsCaption->colour(Ogre::ColourValue(0,255,0));
	fpsCaption->align(Gorilla::TextAlign_Right);

	for (size_t i = 0; i < MAX_DEBUG_LINES; i++)
	{
		debugCaption[i] = dbgLayer->createCaption(48, 50, 5 + 20 * (float)i, "");
		debugCaption[i]->size(1500, 50);
		debugCaption[i]->setScale(0.3f);
		debugCaption[i]->align(Gorilla::TextAlign_Left);
	}

	editUi->initUi(dbgLayer);
}


void GuiOverlay::createOptionMenuButtons()
{
	cButton = firstOptionButton = new ListLoop<Gorilla::Caption*>();

	Gorilla::Caption* caption = mLayer->createCaption(48, 350, 1060 , "Resolution");
	caption->size(600,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Left);
	oMenuButtons.push_back(caption);
	cButton->value = caption;

	caption = mLayer->createCaption(48, 350, 1150 , "Fullscreen");
	caption->size(601,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Left);
	oMenuButtons.push_back(caption);
	cButton->addToEnd(caption);

	caption = mLayer->createCaption(48, 350, 1240 , "Shadows");
	caption->size(602,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Left);
	oMenuButtons.push_back(caption);
	cButton->addToEnd(caption);

	caption = mLayer->createCaption(48, 350, 1330 , "SSAO");
	caption->size(603,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Left);
	oMenuButtons.push_back(caption);
	cButton->addToEnd(caption);

	cOptionButtonA = firstOptionButtonA = new ListLoop<Gorilla::Caption*>();

	std::string resolutionStr=Ogre::StringConverter::toString(gConfig->width)+"x"+Ogre::StringConverter::toString(gConfig->height);
	caption = mLayer->createCaption(48, 1000, 1060 , resolutionStr);
	caption->size(350,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Left);
	oMenuButtons.push_back(caption);
	cOptionButtonA->value=caption;

	caption = mLayer->createCaption(48, 1000, 1150, gConfig->fs ? "On" : "Off");
	caption->size(351,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Left);
	oMenuButtons.push_back(caption);
	cOptionButtonA->addToEnd(caption);

	caption = mLayer->createCaption(48, 1000, 1240, gConfig->shadow == 0 ? "Low" : (gConfig->shadow == 1 ? "Medium" : "High"));
	caption->size(352,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Left);
	oMenuButtons.push_back(caption);
	cOptionButtonA->addToEnd(caption);

	caption = mLayer->createCaption(48, 1000, 1330, gConfig->ssao ? "On" : "Off");
	caption->size(353,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Left);
	oMenuButtons.push_back(caption);
	cOptionButtonA->addToEnd(caption);
}


void GuiOverlay::createMainMenuButtons()
{
	Gorilla::Caption* caption = mLayer->createCaption(48, 670, 900 , "Start");
	caption->size(300,50);
	caption->colour(Ogre::ColourValue(1,1,1,1));
	caption->align(Gorilla::TextAlign_Centre);
	mMenuButtons.push_back(caption);

	caption = mLayer->createCaption(48, 1339.5, 900 , "Options");
	caption->size(300,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Centre);
	mMenuButtons.push_back(caption);

	caption = mLayer->createCaption(48, 20.5, 900 , "Quit");
	caption->size(300,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Centre);
	mMenuButtons.push_back(caption);
}

void GuiOverlay::createIngameMenuButtons()
{
	Gorilla::Caption* caption = mLayer->createCaption(48, -629, 900 , "Quit");
	caption->size(300,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Centre);
	mMenuButtons.push_back(caption);

	caption = mLayer->createCaption(48, 20.5, 900 , "Start");
	caption->size(300,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Centre);
	mMenuButtons.push_back(caption);

	caption = mLayer->createCaption(48, 690, 900 , "Resume");
	caption->size(300,50);
	caption->colour(Ogre::ColourValue(1,1,1,1));
	caption->align(Gorilla::TextAlign_Centre);
	mMenuButtons.push_back(caption);

	caption = mLayer->createCaption(48, 1339.5, 900 , "Restart");
	caption->size(300,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Centre);
	mMenuButtons.push_back(caption);

	caption = mLayer->createCaption(48, 1989, 900 , "Options");
	caption->size(300,50);
	caption->colour(Ogre::ColourValue(1,1,1,0.3));
	caption->align(Gorilla::TextAlign_Centre);
	mMenuButtons.push_back(caption);
}

void GuiOverlay::createLevelsMenuButtons()
{
	int lvlImgs = 6;
	float pos = 1680/2.0f;
	int lvlId = 0;
	auto lvlInfo = Global::gameMgr->getLvlInfo(lvlId++);
	Ogre::Vector4 mainButtonSize(210, 1050, 420, 270);
	Ogre::Vector4 sideButtonSize(100, 1050, 230, 160);

	while (lvlInfo && lvlId <= lvlImgs)
	{
		auto& buttonSize = lvlId == 1 ? mainButtonSize : sideButtonSize;
		Gorilla::Rectangle* r = mLvlsLayer->createRectangle(pos - buttonSize.x, buttonSize.y, buttonSize.z, buttonSize.w);
		auto caption = mLayer->createCaption(48, pos - buttonSize.x, buttonSize.y, lvlInfo->name);
		caption->size(buttonSize.z, buttonSize.w);
		caption->align(Gorilla::TextAlign_Centre);

		r->background_image("lvl" + std::to_string(lvlId));
		lvlButton* b = new lvlButton{ r,caption, lvlId, true, pos };
		lMenuButtons.push_back(b);
		pos += 400;

		if (lvlId==1)
			firstLevelButton = curLvlButton = b;

		lvlInfo = Global::gameMgr->getLvlInfo(lvlId++);
	}

	while (lvlId < lvlImgs)
	{
		Gorilla::Rectangle* r = mLayer->createRectangle(pos - sideButtonSize.x, sideButtonSize.y, sideButtonSize.z, sideButtonSize.w);
		auto caption = mLvlsLayer->createCaption(48, pos - sideButtonSize.x, sideButtonSize.y, "TBD");
		caption->size(sideButtonSize.z, sideButtonSize.w);
		r->background_image("lvl" + std::to_string(lvlId));
		lvlButton* b = new lvlButton{ r, caption, lvlId, false, pos };
		lMenuButtons.push_back(b);
		pos += 400;
		lvlId++;
	}
}

void GuiOverlay::setMainMenu()
{
	mLayer->show();
	mLvlsLayer->show();
	mouseLayer->show();
	gameLayer->hide();

	createMainMenuButtons();
	createOptionMenuButtons();
	createLevelsMenuButtons();
	currentMenu=MAINM;

	ingamemenu = false;
}

void GuiOverlay::setIngameMenu()
{
	mLayer->show();
	mLvlsLayer->show();
	mouseLayer->show();
	gameLayer->hide();

	createIngameMenuButtons();
	createOptionMenuButtons();
	createLevelsMenuButtons();
	currentMenu=MAINM;

	while(gMenuState->value!=RESUME)
		gMenuState = gMenuState->next;

	ingamemenu = true;
}

void GuiOverlay::clearMenu()
{
	mMenuButtons.clear();
	mLayer->hide();
	mLayer->destroyAllCaptions();
	mLvlsLayer->hide();
	mLvlsLayer->destroyAllRectangles();
	mouseLayer->hide();
	oMenuButtons.clear();
	for(auto myIterator = lMenuButtons.begin(); myIterator != lMenuButtons.end(); myIterator++)
		delete (*myIterator);
	lMenuButtons.clear();
	ingamemenu = false;
	moving = false;
}

void GuiOverlay::mouseMoveUpdate(int x,int y)
{
	x = 1680 * (x / (float)Global::window->getWidth());
	y = 1050 * (y / (float)Global::window->getHeight());

	mousePointer->position(Math::Clamp<float>(x,0,1680-15),Math::Clamp<float>(y,0,1050-22));
	Vector2 pos(x, y);// = mousePointer->position();

	if(currentMenu==OPTIONSM)
	{
		std::vector<Gorilla::Caption*>::iterator myIterator;
		for(myIterator = oMenuButtons.begin(); myIterator != oMenuButtons.end(); myIterator++)
		{
			Gorilla::Caption* c=(*myIterator);
			if(c->intersects(pos))
			{
				if (!(cButton->value->width() == c->width() || cOptionButtonA->value->width() == c->width()))
				{
					int found1 = 0,found2 = 0;

					if(c->width()==600 || c->width()==350)
					{
						oMenuState=RESOLUTION;
						found1 = 600;
						found2 = 350;
					}

					if(c->width()==601 || c->width()==351)
					{
						oMenuState=FULLSCREEN;
						found1 = 601;
						found2 = 351;
					}

					if(c->width()==602 || c->width()==352)
					{
						oMenuState=SHADOWS;
						found1 = 602;
						found2 = 352;
					}

					if(c->width()==603 || c->width()==353)
					{
						oMenuState=SSAO;
						found1 = 603;
						found2 = 353;
					}

					if(found1)
					{
						cButton->value->colour(Ogre::ColourValue(1, 1, 1, 0.3));
						cOptionButtonA->value->colour(Ogre::ColourValue(1, 1, 1, 0.3));
						while (cButton->value->width() != found1)
							cButton=cButton->previous;
						while (cOptionButtonA->value->width() != found2)
							cOptionButtonA=cOptionButtonA->previous;
						cButton->value->colour(Ogre::ColourValue(1, 1, 1, 1));
						cOptionButtonA->value->colour(Ogre::ColourValue(1, 1, 1, 1));

						soundEngine->play2D(OVER_OPTIONS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
					}
				}
			}
		}
	}

}

int GuiOverlay::mainMenuPressed()
{
	if(moving || lvlMoving)
		return 0;

	Vector2 mPos=mousePointer->position();

	for(auto myIterator = mMenuButtons.begin(); myIterator != mMenuButtons.end(); myIterator++)
	{
		Gorilla::Caption* c=(*myIterator);
		if(c->intersects(mPos))
		{
			if(currentMenu==MAINM)
			{
				if(!ingamemenu)
				{
					if ((mMenuState->value == START && Ogre::StringUtil::match(c->text(), "Quit")) ||
					        (mMenuState->value == OPTIONS && Ogre::StringUtil::match(c->text(), "Start")) ||
					        (mMenuState->value == QUIT && Ogre::StringUtil::match(c->text(), "Options"))
					   )
					{
						movingDir=1;
						moving=true;
						mMenuState=mMenuState->previous;
						soundEngine->play2D(SWITCH_OPTIONS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
					}
					else if ((mMenuState->value == START && Ogre::StringUtil::match(c->text(), "Options")) ||
					         (mMenuState->value == OPTIONS && Ogre::StringUtil::match(c->text(), "Quit")) ||
					         (mMenuState->value == QUIT && Ogre::StringUtil::match(c->text(), "Start"))
					        )
					{
						movingDir=-1;
						moving=true;
						mMenuState=mMenuState->next;
						soundEngine->play2D(SWITCH_MENU_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
					}
					else if(Ogre::StringUtil::match(c->text(),"Quit"))
					{
						return -3;
					}
					else if(Ogre::StringUtil::match(c->text(),"Start"))
					{
						showLevels();
					}
					else if (mMenuState->value == OPTIONS)
					{
						showOptions();
					}
				}
				else
				{
					if ((gMenuState->value == QUIT && Ogre::StringUtil::match(c->text(), "Options")) ||
					        (gMenuState->value == START && Ogre::StringUtil::match(c->text(), "Quit")) ||
					        (gMenuState->value == RESUME && Ogre::StringUtil::match(c->text(), "Start")) ||
					        (gMenuState->value == RESTART && Ogre::StringUtil::match(c->text(), "Resume")) ||
					        (gMenuState->value == OPTIONS && Ogre::StringUtil::match(c->text(), "Restart"))
					   )
					{
						movingDir=1;
						moving=true;
						gMenuState=gMenuState->previous;
						soundEngine->play2D(SWITCH_OPTIONS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
					}
					else if ((gMenuState->value == QUIT && Ogre::StringUtil::match(c->text(), "Start")) ||
					         (gMenuState->value == START && Ogre::StringUtil::match(c->text(), "Resume")) ||
					         (gMenuState->value == RESUME && Ogre::StringUtil::match(c->text(), "Restart")) ||
					         (gMenuState->value == RESTART && Ogre::StringUtil::match(c->text(), "Options")) ||
					         (gMenuState->value == OPTIONS && Ogre::StringUtil::match(c->text(), "Quit"))
					        )
					{
						movingDir=-1;
						moving=true;
						gMenuState=gMenuState->next;
						soundEngine->play2D(SWITCH_MENU_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
					}
					else if(Ogre::StringUtil::match(c->text(),"Quit"))
					{
						clearMenu();
						return -3;
					}
					else if(Ogre::StringUtil::match(c->text(),"Start"))
					{
						showLevels();
					}
					else if(Ogre::StringUtil::match(c->text(),"Restart"))
					{
						clearMenu();
						return -2;
					}
					else if(Ogre::StringUtil::match(c->text(),"Resume"))
					{
						clearMenu();
						return -1;
					}
					else if (Ogre::StringUtil::match(c->text(), "Options") && gMenuState->value == OPTIONS)
					{
						showOptions();
					}
				}
			}
			else if(currentMenu==OPTIONSM)
			{
				if (ingamemenu && gMenuState->value == OPTIONS || !ingamemenu && mMenuState->value == OPTIONS)
				{
					closeOptions();
				}
			}
			else if(currentMenu==STARTM)
			{
				closeLevels();
			}
		}
	}

	if(currentMenu==OPTIONSM)
		for(auto myIterator = oMenuButtons.begin(); myIterator != oMenuButtons.end(); myIterator++)
		{
			Gorilla::Caption* c=(*myIterator);
			if(c->intersects(mPos))
			{
				if(c->width()==600 || c->width()==350)
				{
					resolutionsLoop=resolutionsLoop->next;
					cOptionButtonA->value->text(resolutionsLoop->value.txt);
					gConfig->height = resolutionsLoop->value.h;
					gConfig->width = resolutionsLoop->value.w;

					soundEngine->play2D(SWITCH_OPTIONS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
				}
				if(c->width()==601 || c->width()==351)
				{
					if(gConfig->fs)
					{
						cOptionButtonA->value->text("Off");
						gConfig->fs = false;
					}
					else
					{
						cOptionButtonA->value->text("On");
						gConfig->fs = true;
					}

					soundEngine->play2D(SWITCH_OPTIONS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
					/*if(gConfig->fshadowR)
					{cOptionButtonA->mButton->text("Low"); mSceneMgr->setShadowTextureSettings(512,4, PF_FLOAT32_R);}
					else
					{cOptionButtonA->mButton->text("High"); mSceneMgr->setShadowTextureSettings(1024,4, PF_FLOAT32_R);}
					gConfig->shadowR=!gConfig->shadowR;
					HighLevelGpuProgramPtr gpuProgram = HighLevelGpuProgramManager::getSingleton().getByName("diffuse_ps_bop");
					gpuProgram->setSourceFile("general_bop2.cg");
					gpuProgram->reload();
					engine->play2D("../../media/oMclick.wav", false, false, false, irrklang::ESM_NO_STREAMING, false);*/
				}
				if(c->width()==602 || c->width()==352)
				{
					if(gConfig->shadow == 0)
					{
						cOptionButtonA->value->text("Medium");
						gConfig->shadow = 1;
					}
					else if(gConfig->shadow == 1)
					{
						cOptionButtonA->value->text("High");
						gConfig->shadow = 2;
					}
					else if(gConfig->shadow == 2)
					{
						cOptionButtonA->value->text("Low");
						gConfig->shadow = 0;
					}

					soundEngine->play2D(SWITCH_OPTIONS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
				}
				if(c->width()==603 || c->width()==353)
				{
					if(gConfig->ssao)
					{
						cOptionButtonA->value->text("Off");
						gConfig->ssao = false;
					}
					else
					{
						cOptionButtonA->value->text("On");
						gConfig->ssao = true;
					}

					soundEngine->play2D(SWITCH_OPTIONS_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
				}
			}
		}

	if(currentMenu==STARTM)
		for(auto myIterator = lMenuButtons.begin(); myIterator != lMenuButtons.end(); myIterator++)
		{
			lvlButton* b=(*myIterator);
			if(b->r->intersects(mPos))
			{
				if(b == curLvlButton)
				{
					if(b->unlocked)
					{
						soundEngine->play2D(LEVEL_START_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
						return b->id;
					}
					else
					{
						soundEngine->play2D(LEVEL_LOCKED_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);
						return 0;
					}
				}
				else
				{

					lvlMoving = true;
					atm=400;
					soundEngine->play2D(LEVEL_SWITCH_SOUND, false, false, false, irrklang::ESM_NO_STREAMING, false);

					if(b->id > curLvlButton->id)
					{
						movingDir=-1;
					}
					else
					{
						movingDir=1;
					}

					for (auto lvl : lMenuButtons)
						if (lvl->id == curLvlButton->id - movingDir)
						{
							curLvlButton = lvl;
							break;
						}

					return 0;
				}
			}
		}

	return 0;
}

void GuiOverlay::setDebugValue(Ogre::Real value1, std::vector<std::string>& values)
{
	fpsCaption->text(Ogre::StringConverter::toString(value1));

	for (size_t i = 0; i < values.size() && i < MAX_DEBUG_LINES; i++)
	{
		debugCaption[i]->text(values[i]);
	}

	for (size_t i = values.size(); i < MAX_DEBUG_LINES; i++)
	{
		debugCaption[i]->text("");
	}
}

void GuiOverlay::showLevelDebug()
{
	editUi->queryLevel();
}

void GuiOverlay::showOptGroupDebug()
{
	editUi->queryOptGroup();

}
void GuiOverlay::showMaterialDebug()
{
	editUi->queryMaterial();
}

void GuiOverlay::showDetailGeometryDebug()
{
	editUi->queryDetailGeometry();
}

void GuiOverlay::showParticleDebug()
{
	editUi->queryParticle();
}

void GuiOverlay::updateLevelsMove(Ogre::Real time)
{
	Real rotSpeed=time*1100;
	atm=atm-rotSpeed;

	if(lvlMoving)
	{
		if(atm<=0)
		{
			currentMenu=STARTM;
			rotSpeed=rotSpeed+atm;
			atm=0;
			lvlMoving=false;
		}

		rotSpeed*=movingDir;

		for(auto myIterator = lMenuButtons.begin(); myIterator != lMenuButtons.end(); myIterator++)
		{
			float scaler = 1 - Ogre::Math::Clamp(abs((*myIterator)->pos-840)/500.0f,0.0f,1.0f);

			(*myIterator)->pos += rotSpeed;

			Gorilla::Rectangle* r=(*myIterator)->r;
			r->left((*myIterator)->pos-115-scaler*85);
			r->width(230+scaler*190);
			r->height(160+scaler*110);

			Real alpha=1-Math::Abs((r->left()-690)/999.0f);
			r->yes_background(alpha);

			Gorilla::Caption* c = (*myIterator)->c;
			c->left((*myIterator)->pos - 115 - scaler * 85);
			c->width(230 + scaler * 190);
			c->height(160 + scaler * 110);
			c->yes_background(alpha);
		}
	}
	else
	{
		if(currentMenu==TOSM && atm<=0)
		{
			currentMenu=STARTM;
			rotSpeed=rotSpeed+atm;
			atm=0;
		}
		else if(currentMenu==FRSM && atm<=0)
		{
			currentMenu=MAINM;
			rotSpeed=rotSpeed+atm;
			atm=0;
		}

		rotSpeed*=movingDir;

		for(auto myIterator = mMenuButtons.begin(); myIterator != mMenuButtons.end(); myIterator++)
		{
			Gorilla::Caption* c=(*myIterator);
			c->top(c->top()+rotSpeed);
		}

		for(auto myIterator = lMenuButtons.begin(); myIterator != lMenuButtons.end(); myIterator++)
		{
			Gorilla::Rectangle* c=(*myIterator)->r;
			c->top(c->top()+rotSpeed);
		}

	}
}

void GuiOverlay::updateOptionsMove(Ogre::Real time)
{
	Real rotSpeed=time*1100;
	atm=atm-rotSpeed;

	if(currentMenu==TOOM && atm<=0)
	{
		currentMenu=OPTIONSM;
		rotSpeed=rotSpeed+atm;
		atm=0;
	}
	else if(currentMenu==FROM && atm<=0)
	{
		currentMenu=MAINM;
		rotSpeed=rotSpeed+atm;
		atm=0;
	}

	rotSpeed*=movingDir;

	for(auto myIterator = mMenuButtons.begin(); myIterator != mMenuButtons.end(); myIterator++)
	{
		Gorilla::Caption* c=(*myIterator);
		c->top(c->top()+rotSpeed);
	}

	for(auto myIterator = oMenuButtons.begin(); myIterator != oMenuButtons.end(); myIterator++)
	{
		Gorilla::Caption* c=(*myIterator);
		c->top(c->top()+rotSpeed);
	}
}


void GuiOverlay::updateMainMenu(Ogre::Real time)
{

	if(currentMenu==TOOM || currentMenu==FROM)
		updateOptionsMove(time);
	else if(lvlMoving || currentMenu==TOSM || currentMenu==FRSM)
		updateLevelsMove(time);
	else if(moving)
	{
		Real rotSpeed=time*1400;
		atm=atm+rotSpeed;
		if(atm>=666)
		{
			moving=false;
			rotSpeed=rotSpeed-(atm-666);
			atm=0;
		}

		for(auto myIterator = mMenuButtons.begin(); myIterator != mMenuButtons.end(); myIterator++)
		{
			//1680+2*309, roz 766
			Gorilla::Caption* c=(*myIterator);
			c->left(c->left()+rotSpeed*movingDir);

			if(c->left()>=1689 && movingDir==1)
			{
				c->left(-309+(c->left()-1689));

			}
			if(c->left()<=-309  && movingDir==-1)
				c->left(1689-(-309-c->left()));

			Real alpha=1-Math::Abs((c->left()-690)/999);
			c->colour(Ogre::ColourValue(1,1,1,alpha));
		}
	}

}


void GuiOverlay::updateIngame(Ogre::Real time)
{
	setDebugValue(Global::window->getLastFPS(), GUtils::debug);

	gameLayer->show();

	gameUi->update(time);
}


void GuiOverlay::updateIngameMenu(Ogre::Real time)
{

	if(currentMenu==TOOM || currentMenu==FROM)
		updateOptionsMove(time);
	else if(lvlMoving || currentMenu==TOSM || currentMenu==FRSM)
		updateLevelsMove(time);
	else if(moving)
	{
		Real rotSpeed=time*1400;
		atm=atm+rotSpeed;
		if(atm>=666)
		{
			moving=false;
			rotSpeed=rotSpeed-(atm-666);
			atm=0;
		}

		for(auto myIterator = mMenuButtons.begin(); myIterator != mMenuButtons.end(); myIterator++)
		{
			//1680+2*309, roz 766
			Gorilla::Caption* c=(*myIterator);
			c->left(c->left()+rotSpeed*movingDir);

			if(c->left()>=2355 && movingDir==1)
				c->left(-975+(c->left()-2355));
			if(c->left()<=-975  && movingDir==-1)
				c->left(2355-(-975-c->left()));

			Real alpha=1-Math::Abs((c->left()-690)/999.0f);
			c->colour(Ogre::ColourValue(1,1,1,alpha));
		}
	}

}
