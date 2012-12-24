// Ŭnicode please
#ifndef __CHARACTER_COMPONENT_H__
#define __CHARACTER_COMPONENT_H__

#include "logic_component.h"
#include "game_globals.h"

struct DestroyMessage;
struct CollideBulletMessage;

namespace cocos2d {;
class CCNode;
}

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

    virtual void InitMsgHandler();
    void OnDestroyMessage(DestroyMessage *msg);
    void OnCollideBulletMessage(CollideBulletMessage *msg);
    virtual void CollideBullet(CollideBulletMessage *msg);
    //캐릭터 컴포넌트를 가진 녀석들은 밖에 못 나가도록 막는다
    void OnOutOfBoundMessage(OutOfBoundMessage *msg);
    void OnDamageObjectMessage(DamageObjectMessage *msg);
    
    //쉴드는 캐릭터 컴포넌트를 상속받은 녀석이면 다 칠 수 있다고 가정
    //그렇지 않으면 나중에 분리.
    void OnCreateShieldMessage(CreateShieldMessage *msg);

    void OnCheckForcesNumberMessage(CheckForcesNumberMessage *msg);

public:
    void set_is_enemy(bool is_enemy) { is_enemy_ = is_enemy; }
    bool is_enemy() { return is_enemy_; }

    cocos2d::CCNode *layer() { return layer_; }
    float hit_point() const { return hit_point_; }
    void set_hit_point(float hit_point) { hit_point_ = hit_point; }

private:
    virtual void Destroy() = 0;
    void RequestRecovery();

private:
    bool is_enemy_;
    float hit_point_;
    cocos2d::CCNode *layer_;

};


#endif