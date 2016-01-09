#pragma once

#include "stdafx.h"

struct PostProcessVariables;

class AaPostProcessListener : public Ogre::CompositorInstance::Listener
{
public:

    AaPostProcessListener(PostProcessVariables* vars)
    {
        this->vars = vars;
    }

    virtual void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
    {
    }

    virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
    {
        if (pass_id == 1 || pass_id == 2 || pass_id == 3)
        {
            mat->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant("lightPosition", vars->SunScreenSpacePosition);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("den", vars->godrayEdge);
        }
        else if(pass_id == 4)
        {
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("invProjMatrix", vars->ipm);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("projMatrix", vars->projm);
        }
        else if (pass_id == 5)
        {
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("invViewProjMatrix", vars->ivp);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("prevViewProjMatrix", vars->pvp);
        }
        else if (pass_id == 6)
        {
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("bloomAdd", vars->bloomStrDepAddSize.z);
        }
        else if (pass_id == 7)
        {
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("bloomSize", vars->bloomStrDepAddSize.w);
        }
        else if (pass_id == 12)
        {
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("ppDistortion", vars->ppDistortionIgnore);
        }
        else if (pass_id == 13)
        {
            auto params = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
            params->setNamedConstant("hurtCoef", vars->hurtEffect);
            params->setNamedConstant("colourOverlaying", vars->colourOverlaying);
            params->setNamedConstant("ContSatuSharpNoise", vars->ContSatuSharpNoise);
            params->setNamedConstant("ColouringShift", vars->ColouringShift);
            params->setNamedConstant("bloomStrDep", vars->bloomStrDepAddSize);
        }
        else if (pass_id == 14)
        {
            auto params = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
            params->setNamedConstant("invViewProjMatrix", vars->ivp);
            params->setNamedConstant("prevViewProjMatrix", vars->pvp);
            params->setNamedConstant("radialHorizBlurShade", &(vars->radialHorizBlurVignette.x), 3);
        }
    }

private:

    PostProcessVariables* vars;

};