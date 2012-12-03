﻿// Ŭnicode please
#ifndef __COMBAT_PLANE_COMPONENT_H__
#define __COMBAT_PLANE_COMPONENT_H__

#include "character_component.h"

//적 기체(전투기) 클래스

class CombatPlaneComponent : public CharacterComponent {
public:
    CombatPlaneComponent(GameObject *obj, cocos2d::CCNode *layer);
    virtual ~CombatPlaneComponent();

public:
    virtual CompType type() const { return kCompEnemy; }
	virtual void Update(float dt);
	virtual void InitMsgHandler();

private:
    void Attack(float dt);
    //설정할 수 있도록 하자.
    const float attack_cool_down_;
    float attack_timer_;

};

#endif