#pragma once

struct WorldMaterials
{
	~WorldMaterials();
	void init();

	OgreNewt::MaterialID* plNoMove_mat = NULL;
	OgreNewt::MaterialID* plMove_mat;
	OgreNewt::MaterialID* noCollide_mat;
	OgreNewt::MaterialID* playerIgnore_mat;
	OgreNewt::MaterialID* trig_mat;
	OgreNewt::MaterialID* actionMaker_mat;
	OgreNewt::MaterialID* selfIgnore_mat;
	OgreNewt::MaterialID* plBlock_mat;

private:
	void initCollisions(OgreNewt::World* mWorld);
};