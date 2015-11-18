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
        if (pass_id == 1)
        {
            params1 = mat->getTechnique(0)->getPass(0)->getVertexProgramParameters();
        }
        else if (pass_id == 2)
        {
            params2 = mat->getTechnique(0)->getPass(0)->getVertexProgramParameters();
        }
        else if (pass_id == 3)
        {
            params3 = mat->getTechnique(0)->getPass(0)->getVertexProgramParameters();
        }

    }

    virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
    {
        if (pass_id == 1)
        {
            params1->setNamedConstant("lightPosition", vars->SunScreenSpacePosition);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("den", vars->godrayEdge);
        }
        else if (pass_id == 2)
        {
            params2->setNamedConstant("lightPosition", vars->SunScreenSpacePosition);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("den", vars->godrayEdge);
        }
        else if (pass_id == 3)
        {
            params3->setNamedConstant("lightPosition", vars->SunScreenSpacePosition);
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
        else if (pass_id == 12)
        {
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("ppDistortion", vars->ppDistortionIgnore);
        }
        else if (pass_id == 13)
        {
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("hurtCoef", vars->hurtEffect);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("colourOverlaying", vars->colourOverlaying);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("ContSatuSharpNoise", vars->ContSatuSharpNoise);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("ColouringShift", vars->ColouringShift);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("bloomStrDep", vars->bloomStrDep);
        }
        else if (pass_id == 14)
        {
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("invViewProjMatrix", vars->ivp);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("prevViewProjMatrix", vars->pvp);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("radialHorizBlurShade", &(vars->radialHorizBlurVignette.x), 3);
        }
    }

private:

    PostProcessVariables* vars;

    Ogre::GpuProgramParametersSharedPtr params1;
    Ogre::GpuProgramParametersSharedPtr params2;
    Ogre::GpuProgramParametersSharedPtr params3;
};