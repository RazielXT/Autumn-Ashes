#pragma once

#include "stdafx.h"

class LutTexture
{
public:

	LutTexture();

	std::string getCurrentTexture();

	std::vector<std::string> getAvailableTextures();

	Ogre::TexturePtr loadTexture(std::string name);

private:

	bool loadVolumeTextureFrom2D(Ogre::TexturePtr tex);

	Ogre::TexturePtr loadedTexture;
	std::string loadedName;

	std::vector<std::string> textures;
};