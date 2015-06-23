#pragma once

struct WorldMaterials
{
    ~WorldMaterials();
    void init(OgreNewt::World* mWorld);

    OgreNewt::MaterialID* stoji_mat = NULL;
    OgreNewt::MaterialID* ide_mat;
    OgreNewt::MaterialID* flag_mat;
    OgreNewt::MaterialID* playerIgnore_mat;
    OgreNewt::MaterialID* trig_mat;
    OgreNewt::MaterialID* action_mat;
    OgreNewt::MaterialID* selfIgnore_mat;
    OgreNewt::MaterialID* slide_mat;

    void initCollisions(OgreNewt::World* mWorld);
};