#pragma once

#include "Gorilla.h"
#include "GameUiActions.h"

class Energy;

class GameUi
{
public:

	GameUi(Gorilla::Layer* mLayer);
	~GameUi()
	{
		clear();
	}

	void showIngameText(Ogre::String text);
	void showUseGui(UiHintInfo id);

	void showEnergyGui(const std::vector<Energy*>& energies);

	void update(float time);
	void clear();

	void show();
	void hide();

private:

	float infoTextTimer;
	bool shownInfoText;
	bool shownUseGui, wantUseGui;
	char currUseGui;

	Gorilla::Caption* infoTextCaption;
	Gorilla::Caption* useTextCaption;
	Gorilla::Caption* gameCaption;

	Gorilla::Polygon* slideBar;
};
