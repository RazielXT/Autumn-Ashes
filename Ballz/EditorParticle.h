#pragma once
#include "EditorItem.h"

struct EditorParticle : public EditorItem
{
	virtual void reset();

	virtual void setPosition(Ogre::Vector3&);
	virtual void setScale(Ogre::Vector3&);
	virtual Ogre::Vector3 getPosition();

	virtual std::vector<Ogre::Vector3> getIndividualPositions();
	virtual void setIndividualPositions(std::vector<Ogre::Vector3>&);

	virtual void move(Ogre::Vector3& move);
	virtual void rotate(Ogre::Vector3& axis, Ogre::Radian& angle);
	virtual void addScale(Ogre::Vector3& scale);
	virtual void duplicate();
	virtual void remove();
	virtual void editMouseReleased(SelectionMode mode);

	virtual void add(Ogre::ParticleSystem* ps);
	virtual void select(Ogre::ParticleSystem* ps);
	virtual void deselect();
	virtual bool filter(std::string& name);

	std::vector<Ogre::ParticleSystem*> selected;

	virtual void sendUiInfoMessage(EditorUiHandler* handler);
	virtual void handleSelectionMessage(SelectionInfoChange*);

private:

	Ogre::Quaternion getOrientation();
	void setParticlePosition(Ogre::ParticleSystem* ps, Ogre::Vector3 pos);
	void setParticleOrientation(Ogre::ParticleSystem* ps, Ogre::Quaternion or);

	void updateNode();
	Ogre::SceneNode* boundsNode = nullptr;
};