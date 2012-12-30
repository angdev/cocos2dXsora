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

public:
    void OnAttackMessage(AttackMessage *msg);
    void Attack();

private:
    int attack_target_id_;
    float attack_cool_down_;
    //ai 분리 이름이 잘못되었네
    //fsm이라고 해야 맞는 느낌인데
    //단순한 놈들은 상태 만들지 말고 그냥 플래그로 대충 관리.
    //공격 못하는 단계
    bool is_idle_state_;

};

#endif