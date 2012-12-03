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
    virtual void InitMsgHandler() { }

    //get/setter
public:
    glm::vec2 dir_vec() { return dir_vec_px_; }
    void set_dir_vec(const glm::vec2& dir_vec_px) { dir_vec_px_ = dir_vec_px; }

private:
    //단위는 px
    glm::vec2 dir_vec_px_;
};

#endif