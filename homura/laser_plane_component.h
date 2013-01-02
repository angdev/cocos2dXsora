﻿// Ŭnicode please
#ifndef __LASER_PLANE_COMPONENT_H__
#define __LASER_PLANE_COMPONENT_H__

#include "character_component.h"

//얘는 그냥 기체가 바라보는 방향으로 레이저 쏘는 놈
class LaserPlaneComponent : public CharacterComponent {
public:
    explicit LaserPlaneComponent(GameObject *obj, cocos2d::CCNode *layer);
    virtual ~LaserPlaneComponent();

public:
    virtual CompType type() const { return kCompLaserPlane; }
    virtual void Update(float dt);
    virtual void InitMsgHandler();

public:
    void OnAttackMessage(AttackMessage *msg);
    void Attack();

private:
    class RayCastCallback : public b2RayCastCallback {
    public:
        RayCastCallback(CharacterComponent *owner_comp) : owner_comp_(owner_comp) {}
    
    public:
        void Reset();
        virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
        void AfterCallback();
    private:
        bool is_hit_;
        float32 nearest_fraction_;
        b2Fixture *fixture;
        b2Vec2 point;

        CharacterComponent *owner_comp_;
    };
    std::unique_ptr<RayCastCallback> ray_cast_callback_;

    void AfterDestroy();

private:
    const float attack_keep_time_;
    const float attack_cool_down_;
    float attack_timer_;
    bool now_cool_down_;
    bool now_attacking_;
};

#endif