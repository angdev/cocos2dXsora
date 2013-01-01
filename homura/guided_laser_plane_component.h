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
    void AfterDestroy();

private:
    int attack_target_id_;
    //이 쯤되면 정형화된 클래스 갖다두는 것도 괜찮겠다
    //스킬 쿨다운 관리? 는 state
    //공격을 멈추고 다음 공격까지 필요한 시간
    const float attack_cool_down_;
    //연속 공격이 가능한 시간
    const float attack_keep_time_;
    float attack_timer_;
    //ai 분리 이름이 잘못되었네
    //fsm이라고 해야 맞는 느낌인데
    //단순한 놈들은 상태 만들지 말고 그냥 플래그로 대충 관리.
    //공격 못하는 단계
    bool is_idle_state_;
    bool now_attacking_;

};

#endif