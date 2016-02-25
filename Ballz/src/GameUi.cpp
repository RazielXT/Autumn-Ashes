#include "stdafx.h"
#include "GameUi.h"
#include "Energy.h"

GameUi::GameUi(Gorilla::Layer* mLayer)
{
	infoTextTimer = 0;
	shownInfoText = false;
	shownUseGui = false;
	wantUseGui = false;

	infoTextCaption = mLayer->createCaption(48, 50, 900, "");
	infoTextCaption->size(1600, 50);
	infoTextCaption->align(Gorilla::TextAlign_Centre);
	infoTextCaption->vertical_align(Gorilla::VerticalAlign_Bottom);

	useTextCaption = mLayer->createCaption(48, 50, 500, "");
	useTextCaption->size(1580, 50);
	useTextCaption->align(Gorilla::TextAlign_Centre);
	useTextCaption->vertical_align(Gorilla::VerticalAlign_Middle);

	gameCaption = mLayer->createCaption(48, 50, 900, "");
	gameCaption->size(1500, 50);
	gameCaption->setScale(1.0f);
	gameCaption->align(Gorilla::TextAlign_Right);
}

void GameUi::update(float time)
{
	if (shownUseGui && !wantUseGui)
	{
		useTextCaption->text("");
		shownUseGui = false;
	}

	wantUseGui = false;

	if (shownInfoText)
	{
		infoTextTimer -= time;
		infoTextCaption->colour(Ogre::ColourValue(1, 1, 1, Ogre::Math::Clamp(infoTextTimer * 2, 0.0f, 1.0f)));

		if (infoTextTimer <= 0)
		{
			infoTextCaption->text("");
			shownInfoText = false;
		}
	}
}

void GameUi::clear()
{
	gameCaption->text("");
}

void GameUi::showEnergyGui(const std::vector<Energy*>& energies)
{
	int types[4] = { 0 };
	for (auto e : energies)
	{
		types[e->getUsageState()]++;
	}

	std::string txt = std::to_string(types[0]) + " " + std::to_string(types[1]) + " " + std::to_string(types[2]) + " " + std::to_string(types[3]);
	gameCaption->text(txt);
}

void GameUi::showIngameText(Ogre::String text)
{
	infoTextCaption->text(text);

	infoTextTimer = Ogre::Math::Clamp(0.1f*text.length(), 2.0f, 8.0f);
	shownInfoText = true;
}

void GameUi::showUseGui(UiHintInfo id)
{
	wantUseGui = true;

	if (!shownUseGui || id != currUseGui)
	{
		shownUseGui = true;
		currUseGui = id;

		switch (id)
		{
		case Ui_Pickup:
			useTextCaption->text("pickup");
			break;
		case Ui_Use:
			useTextCaption->text("use");
			break;
		case Ui_Climb:
			useTextCaption->text("climb");
			break;
		case Ui_Target:
			useTextCaption->text("(   )");
			break;
		case Ui_GrabEnergy:
			useTextCaption->text("<*>");
			break;
		case Ui_GrabEnergyUnavailable:
			useTextCaption->text("!*!");
			break;
		case Ui_UseEnergy:
			useTextCaption->text(">*<");
			break;
		case Ui_UseEnergyUnavailable:
			useTextCaption->text("-*-");
			break;
		default:
			useTextCaption->text("??");
		}
	}
}
