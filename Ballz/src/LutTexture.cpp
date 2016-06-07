#include "stdafx.h"
#include "LutTexture.h"
#include <filesystem>
#include "SUtils.h"

LutTexture::LutTexture()
{
	getAvailableTextures();
}

std::vector<std::string> LutTexture::getAvailableTextures()
{
	if (textures.empty())
	{
		for (auto& p : filesystem::directory_iterator(Path::LutTexture))
		{
			std::string name = p.path().string();
			if (SUtils::endsWith(name, "png"))
			{
				name = name.substr(name.find_last_of('/'));
				textures.push_back(name);
			}
		}
	}

	return textures;
}

Ogre::TexturePtr LutTexture::loadTexture(std::string name)
{
	if (loadVolumeTextureFrom2D(Ogre::Tex))
		loadedName = name;

	return loadedTexture;
}

bool LutTexture::loadVolumeTextureFrom2D(Ogre::TexturePtr tex)
{
	const size_t LUTres = 16;

	if (tex && tex.res == LUTres)
	{
		if (!loadedTexture)
		{
			loadedTexture = Ogre::TextureManager::getSingleton().createManual(
				"LUT", // name
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				Ogre::TEX_TYPE_3D,      // type
				LUTres, LUTres, LUTres,       // width, height & depth
				0,                // number of mipmaps
				Ogre::PF_BYTE_BGRA,     // pixel format
				Ogre::TU_DEFAULT);
		}

		// Get the pixel buffer
		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = my3DTexture->getBuffer();

		// Lock the pixel buffer and get a pixel box
		pixelBuffer->lock(HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
		const PixelBox& pixelBox = pixelBuffer->getCurrentLock();

		uint8* pDest = static_cast<uint8*>(pixelBox.data);

		// Fill in some pixel data. This will give a semi-transparent blue,
		// but this is of course dependent on the chosen pixel format.
		for (size_t j = 0; j < LUTres; j++)
			for (size_t i = 0; i < LUTres; i++)
				for (size_t k = 0; k < LUTres; k++)
				{
					*pDest++ = getRoundValue(k / 64); // B
					*pDest++ = getRoundValue(i / 64); // G
					*pDest++ = getRoundValue(j / 64); // R
					*pDest++ = 1;                 // A
				}

		// Unlock the pixel buffer
		pixelBuffer->unlock();

		return true;
	}

	return false;
}

