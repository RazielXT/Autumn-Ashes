#include "stdafx.h"

//Ogre::Real du=0.2;
class GodRaysListener : public Ogre::CompositorInstance::Listener
{
public:
    GodRaysListener() : SunScreenSpacePosition(Ogre::Vector3::ZERO)
    {
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
        if (pass_id == 3)
        {

            params3 = mat->getTechnique(0)->getPass(0)->getVertexProgramParameters();
        }


    }

    virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
    {
        //mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("den",du);
        if (pass_id == 1)
            params1->setNamedConstant("lightPosition", SunScreenSpacePosition);
        else if (pass_id == 2)
            params2->setNamedConstant("lightPosition", SunScreenSpacePosition);
        if (pass_id == 3)
            params3->setNamedConstant("lightPosition", SunScreenSpacePosition);


    }

    Ogre::Vector4 SunScreenSpacePosition;
private:
    Ogre::GpuProgramParametersSharedPtr params1;
    Ogre::GpuProgramParametersSharedPtr params2;
    Ogre::GpuProgramParametersSharedPtr params3;

};



GodRaysListener *godRaysListener;






