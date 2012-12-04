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
    void OnDamageObjectMessage(DamageObjectMessage *msg);
    void OnSetDirectionMessage(SetDirectionMessage *msg);
    void OnDestroyMessage(DestroyMessage *msg);

    //get/setter
public:
    glm::vec2 dir_vec() { return dir_vec_px_; }
    void set_dir_vec(const glm::vec2& dir_vec_px) { dir_vec_px_ = dir_vec_px; }

    //데미지 설정 함수. 일단 둔다.
    void set_damage(float damage) { damage_ = damage; }
    
    void set_from_enemy(bool from_enemy) { from_enemy_ = from_enemy; }
    bool from_enemy() { return from_enemy_; }

private:
    //단위는 px
    glm::vec2 dir_vec_px_;
    //데미지
    float damage_;
    bool from_enemy_;
};

#endif