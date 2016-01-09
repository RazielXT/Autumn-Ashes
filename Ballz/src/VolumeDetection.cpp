#include "stdafx.h"
#include "VolumeDetection.h"
#include "Player.h"

VolumeDetectionManager VolumeDetectionManager::get;

void VolumeDetectionManager::registerVolume(VolumeDetection* volume)
{
    registeredVolumes.push_back(volume);
}

bool VolumeDetectionManager::update(Ogre::Real tslf)
{
    auto pos = Global::player->bodyPosition;

    for (auto v : registeredVolumes)
    {
        if (v->detectionRegistered)
        {
            if (v->volume.isInside(pos))
                v->playerInside();
        }
    }

    return true;
}

void VolumeDetectionManager::reset()
{
    Global::mEventsMgr->addCachedTask(this);
    registeredVolumes.clear();
}
