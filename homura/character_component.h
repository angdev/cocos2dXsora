// Ŭnicode please
#ifndef __CHARACTER_COMPONENT_H__
#define __CHARACTER_COMPONENT_H__

#include "logic_component.h"
#include "game_globals.h"

struct DestroyMessage;

//캐릭터가 가지는 수치를 header가지고 만들어줄 수 있게 하려고 함.
class CharacterComponent : public LogicComponent {
public:
    CharacterComponent(GameObject *obj, cocos2d::CCNode *layer = 0);
    virtual ~CharacterComponent();

public:
    //Character 공통의 Update 처리
    //하위 녀석들은 이 함수를 호출한다.
    virtual void Update(float dt);

    //데미지 처리
    //TODO
    //Need implementation
    void ApplyDamage(float damage);
    virtual void InitMsgHandler() { RegisterMsgFunc(this, &CharacterComponent::OnDestroy); }
    void OnDestroy(DestroyMessage *msg);

public:
    cocos2d::CCNode *layer() { return layer_; }
    float hit_point() const { return hit_point_; }
    void set_hit_point(float hit_point) { hit_point_ = hit_point; }

private:
    float hit_point_;
    cocos2d::CCNode *layer_;

};


#endif