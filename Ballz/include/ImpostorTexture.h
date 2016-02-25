#pragma once

#include "stdafx.h"

class ImpostorRenderEnvironment
{
public:

	Ogre::SceneNode* camNode;
	Ogre::Camera* renderCamera;

	void PreRender();
	void PostRender();
};

class ImpostorTexture
{
public:

	ImpostorTexture() {}
	~ImpostorTexture() {}

	void render(Ogre::Entity* ent = nullptr);
	void clear();

protected:

	class ImpostorTextureResourceLoader : public Ogre::ManualResourceLoader
	{
	public:
		ImpostorTextureResourceLoader(ImpostorTexture& impostorTexture);
		virtual void loadResource(Ogre::Resource *resource);

	protected:
		ImpostorTexture& texture;
	};

	std::auto_ptr<ImpostorTextureResourceLoader> loader;
	Ogre::Entity* ent;
	Ogre::TexturePtr texture;

	ImpostorRenderEnvironment environment;

};