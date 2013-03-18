// Ŭnicode please
#ifndef __LASER_PLANE_COMPONENT_H__
#define __LASER_PLANE_COMPONENT_H__

#include "character_component.h"
#include "action_timer.h"

//얘는 그냥 기체가 바라보는 방향으로 레이저 쏘는 놈
class LaserPlaneComponent : public CharacterComponent {
public:
    explicit LaserPlaneComponent(GameObject *obj, cocos2d::CCNode *layer);
    virtual ~LaserPlaneComponent();

public:
    virtual CompType type() const { return kCompLaserPlane; }
    virtual void DerivedUpdate(float dt);
    virtual void InitMsgHandler();

public:
    void OnAttackMessage(AttackMessage *msg);
    void OnMoveToMessage(MoveToMessage *msg);
    void OnMoveByMessage(MoveByMessage *msg);

    void UpdateAttackLogic(float dt);
    void Attack();
    void StopAttack();

public:
    float laser_damage() { return laser_damage_; }
    void set_laser_damage(float laser_damage) { laser_damage_ = laser_damage; }

private:
    class RayCastCallback : public b2RayCastCallback {
    public:
        RayCastCallback(LaserPlaneComponent *owner_comp) : owner_comp_(owner_comp) {}
    
    public:
        void Reset();
        virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
        void AfterCallback();
    private:
        bool is_hit_;
        float32 nearest_fraction_;
        b2Fixture *fixture;
        b2Vec2 point;

        LaserPlaneComponent *owner_comp_;
    };
    std::unique_ptr<RayCastCallback> ray_cast_callback_;

    void AfterDestroy();

private:
    std::unique_ptr< ActionTimer<LaserPlaneComponent> > attack_timer_;
    int laser_sound_id_;
    
    //per sec
    float laser_damage_;
};

#endif