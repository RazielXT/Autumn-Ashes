#include "stdafx.h"

class BasicPostProcessListener : public Ogre::CompositorInstance::Listener
{
public:

    BasicPostProcessListener(Ogre::Vector4* SunScreenSpacePosition, Ogre::Matrix4* ivp,	Ogre::Matrix4* pvp,	Ogre::Real* hurtEffect,	Ogre::Real* godrayEdge, Ogre::Vector4* colourOverlaying )
    {
        this->SunScreenSpacePosition = SunScreenSpacePosition;
        this->ivp = ivp;
        this->pvp = pvp;
        this->hurtEffect = hurtEffect;
        this->godrayEdge = godrayEdge;
        this->colourOverlaying = colourOverlaying;
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
            params1->setNamedConstant("lightPosition", *SunScreenSpacePosition);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("den",*godrayEdge);
        }
        else if (pass_id == 2)
        {
            params2->setNamedConstant("lightPosition", *SunScreenSpacePosition);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("den",*godrayEdge);
        }
        else if (pass_id == 3)
        {
            params3->setNamedConstant("lightPosition", *SunScreenSpacePosition);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("den",*godrayEdge);
        }

        if(pass_id==13)
        {
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("hurtCoef",*hurtEffect);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("colourOverlaying",*colourOverlaying);
        }
        if(pass_id==14)
        {
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("invViewProjMatrix",*ivp);
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("prevViewProjMatrix",*pvp);
        }
    }



private:

    Ogre::Vector4* SunScreenSpacePosition;
    Ogre::Matrix4* ivp;
    Ogre::Matrix4* pvp;
    Ogre::Real* hurtEffect;
    Ogre::Real* godrayEdge;
    Ogre::Vector4* colourOverlaying;

    Ogre::GpuProgramParametersSharedPtr params1;
    Ogre::GpuProgramParametersSharedPtr params2;
    Ogre::GpuProgramParametersSharedPtr params3;

};