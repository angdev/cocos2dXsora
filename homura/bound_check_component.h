// Ŭnicode please
#ifndef __BOUND_CHECK_COMPONENT_H__
#define __BOUND_CHECK_COMPONENT_H__

#include "logic_component.h"

//경계를 체크하는 로직
//가상의 게임 객체에 포함
class BoundCheckComponent : public LogicComponent {
public:
    BoundCheckComponent(GameObject *obj, const cocos2d::CCSize &window_size) 
        : LogicComponent(obj), window_size_(window_size) {}
    virtual ~BoundCheckComponent() {}

public:
    virtual CompType type() const;
    virtual void Update(float dt);
    virtual void InitMsgHandler() {}

private:
    cocos2d::CCSize window_size_;
};

#endif