// Ŭnicode please
#ifndef __LASER_PLANE_COMPONENT_H__
#define __LASER_PLANE_COMPONENT_H__

#include "character_component.h"

class LaserPlaneComponent : public CharacterComponent {
public:
    explicit LaserPlaneComponent(GameObject *obj);
    virtual ~LaserPlaneComponent();

public:
    virtual CompType type() const { return kCompLaserPlane; }
    virtual void Update(float dt);
    virtual void InitMsgHandler();


private:

};

#endif