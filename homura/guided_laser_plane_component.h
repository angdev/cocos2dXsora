// Ŭnicode please
#ifndef __GUIDED_LASER_PLANE_COMPONENT_H__
#define __GUIDED_LASER_PLANE_COMPONENT_H__

#include "character_component.h"

class GuidedLaserPlaneComponent : public CharacterComponent {
public:
    explicit GuidedLaserPlaneComponent(GameObject *obj, cocos2d::CCNode *layer);
    virtual ~GuidedLaserPlaneComponent();

};

#endif