// Ŭnicode please
#ifndef __BULLET_COMPONENT_H__
#define __BULLET_COMPONENT_H__

#include "logic_component.h"

class BulletComponent : public LogicComponent {
public:
    BulletComponent(GameObject *obj);
    virtual ~BulletComponent();

public:
    //temp 이 컴포넌트의 하위 컴포넌트는 없나?
    virtual CompType type() const { return kCompBullet; }
    virtual void Update(float dt);
    virtual void InitMsgHandler();

    //메세지 처리 함수들
public:
    void OnBulletDamageObjectMessage(BulletDamageObjectMessage *msg);
    void Destroy();
    void OnOutOfBoundMessage(OutOfBoundMessage *msg);

    //get/setter
public:
    //데미지 설정 함수. 일단 둔다.
    void set_damage(float damage) { damage_ = damage; }

    //속력 설정 함수.
    void set_speed(float speed) { speed_ = speed; }
    
    void set_from_enemy(bool from_enemy) { from_enemy_ = from_enemy; }
    bool from_enemy() { return from_enemy_; }

private:
    //데미지
    float damage_;
    float speed_;
    bool from_enemy_;
};

#endif