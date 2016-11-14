#include "stdafx.h"
#include "LutTexture.h"
#include <filesystem>
#include "SUtils.h"

LutTexture::LutTexture()
{
	getAvailableTextures();
}

std::string LutTexture::getCurrentTexture()
{
	return loadedName;
}

std::vector<std::string> LutTexture::getAvailableTextures()
{
	if (textures.empty())
	{
		for (auto& p : std::experimental::filesystem::directory_iterator(Path::LutTextures))
		{
			std::string name = p.path().string();
			if (SUtils::endsWith(name, "png"))
			{
				name = name.substr(name.find_last_of('\\') + 1);
				textures.push_back(name);
			}
		}
	}

	return textures;
}

Ogre::TexturePtr LutTexture::loadTexture(std::string name)
{
	if (loadVolumeTextureFrom2D(Ogre::TextureManager::getSingleton().load(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TextureType::TEX_TYPE_2D, 0)))
		loadedName = name;

	return loadedTexture;
}

bool LutTexture::loadVolumeTextureFrom2D(Ogre::TexturePtr lutTex)
{
	const size_t LUTres = 16;

	if (!lutTex.isNull() && lutTex->getHeight() == LUTres)
	{
		if (loadedTexture.isNull())
		{
			loadedTexture = Ogre::TextureManager::getSingleton().createManual(
			                    "LUT", // name
			                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			                    Ogre::TEX_TYPE_3D,      // type
			                    LUTres, LUTres, LUTres,       // width, height & depth
			                    0,                // number of mipmaps
			                    Ogre::PF_BYTE_BGRA,     // pixel format
			                    Ogre::TU_STATIC_WRITE_ONLY);
		}

		// Get the pixel buffer
		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = loadedTexture->getBuffer();
		Ogre::HardwarePixelBufferSharedPtr pixelLutBuffer = lutTex->getBuffer();

		// Lock the pixel buffer and get a pixel box
		pixelBuffer->lock(HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
		const PixelBox& pixelBox = pixelBuffer->getCurrentLock();

		pixelLutBuffer->lock(HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
		PixelBox& pixelLutBox = const_cast<PixelBox&>(pixelLutBuffer->getCurrentLock());

		uint8* pDest = static_cast<uint8*>(pixelBox.data);

		// Fill in some pixel data. This will give a semi-transparent blue,
		// but this is of course dependent on the chosen pixel format.
		for (size_t j = 0; j < LUTres; j++)
			for (size_t i = 0; i < LUTres; i++)
				for (size_t k = 0; k < LUTres; k++)
				{
					auto col = pixelLutBox.getColourAt(j + LUTres*k,i,0);

					*pDest++ = col.r*255; // B
					*pDest++ = col.g*255; // G
					*pDest++ = col.b*255; // R
					*pDest++ = 1;                 // A
				}

		// Unlock the pixel buffer
		pixelBuffer->unlock();
		pixelLutBuffer->unlock();

		return true;
	}

	return false;
}

