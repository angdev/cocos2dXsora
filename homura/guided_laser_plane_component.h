// Ŭnicode please
#ifndef __GUIDED_LASER_PLANE_COMPONENT_H__
#define __GUIDED_LASER_PLANE_COMPONENT_H__

#include "character_component.h"

//얘는 타겟이 정해지고 그 타겟쪽으로 유도 레이저를 날림
class GuidedLaserPlaneComponent : public CharacterComponent {
public:
    explicit GuidedLaserPlaneComponent(GameObject *obj, cocos2d::CCNode *layer);
    virtual ~GuidedLaserPlaneComponent();

public:
    virtual CompType type() const { return kCompGuidedLaserPlane; }
    virtual void Update(float dt);
    virtual void InitMsgHandler();

};

#endif