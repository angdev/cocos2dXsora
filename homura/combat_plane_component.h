// Ŭnicode please
#ifndef __COMBAT_PLANE_COMPONENT_H__
#define __COMBAT_PLANE_COMPONENT_H__

#include "character_component.h"

//적 기체(전투기) 클래스

class CombatPlaneComponent : public CharacterComponent {
public:
    explicit CombatPlaneComponent(GameObject *obj, cocos2d::CCNode *layer);
    virtual ~CombatPlaneComponent();

public:
    virtual CompType type() const { return kCompCombatPlane; }
	virtual void DerivedUpdate(float dt);
	virtual void InitMsgHandler();

public:
    void OnAttackMessage(AttackMessage *msg);
    void Attack();
    void SuicideAttack(const b2Vec2 &object_pos);
    
public:
    void set_bullet_damage(float bullet_damage) { bullet_damage_ = bullet_damage; }
    bool suicide_flag() { return suicide_flag_; }
    void set_available_suicide(bool available_suicide) { cocos2d::CCLog("!"); available_suicide_ = available_suicide; }

private:
    void AfterDestroy();

private:
    //설정할 수 있도록 하자.
    const float attack_cool_down_;
    float attack_timer_;
    int attack_count_;//공격 횟수를 적절히 세서 패턴 변경에 이용

    float bullet_damage_;

    bool available_suicide_;
    //자폭 플래그
    bool suicide_flag_;
    //날아가는 속도 벡터
    b2Vec2 suicide_vec;
};

#endif