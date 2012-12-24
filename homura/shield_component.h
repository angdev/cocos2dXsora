// Ŭnicode please
#ifndef __SHIELD_COMPONENT_H__
#define __SHIELD_COMPONENT_H__

#include "character_component.h"

//쉴드에 필요한거 - 체력, 지속 시간, 쉴드 대상
//몸빵해주는 캐릭이라고 생각
class ShieldComponent : public CharacterComponent {
public:
    ShieldComponent(GameObject *obj);
    virtual ~ShieldComponent();

public:
    virtual CompType type() const { return kCompShield; }
    virtual void InitMsgHandler();
    virtual void Update(float dt);
    
    //임시 set/getter
public:
    void set_duration(float duration) { duration_ = duration; }
    void set_target_id(int target_id) { target_id_ = target_id; }

private:
    virtual void Destroy();

private:
    //쉴드 지속 시간
    float duration_;
    float elapsed_time_;
    int target_id_;
};

#endif