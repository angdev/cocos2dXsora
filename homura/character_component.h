// Ŭnicode please
#ifndef __CHARACTER_COMPONENT_H__
#define __CHARACTER_COMPONENT_H__

#include "logic_component.h"
#include "game_globals.h"

struct DestroyMessage;
struct CollideBulletMessage;

class CharacterFSM;

namespace cocos2d {;
class CCNode;
}

//캐릭터가 가지는 수치를 header가지고 만들어줄 수 있게 하려고 함.
class CharacterComponent : public LogicComponent {
public:
    explicit CharacterComponent(GameObject *obj, cocos2d::CCNode *layer = 0);
    virtual ~CharacterComponent();

public:
    //Character 공통의 Update 처리
    //하위 녀석들은 이 함수를 호출한다.
    virtual void Update(float dt);
    void BaseUpdate(float dt);
    virtual void DerivedUpdate(float dt) = 0;
    virtual void Destroy();

    //데미지 처리
    //TODO
    //Need implementation
    void ApplyDamage(float damage);

    virtual void InitMsgHandler();
    void OnCollideBulletMessage(CollideBulletMessage *msg);
    virtual void CollideBullet(CollideBulletMessage *msg);
    void OnDamageObjectMessage(DamageObjectMessage *msg);
    
    //쉴드는 캐릭터 컴포넌트를 상속받은 녀석이면 다 칠 수 있다고 가정
    //그렇지 않으면 나중에 분리.
    void OnCreateShieldMessage(CreateShieldMessage *msg);
    void OnCheckForcesNumberMessage(CheckForcesNumberMessage *msg);
    //플레이어가 밖에 못 나가도록 막는다
    void OnOutOfBoundMessage(OutOfBoundMessage *msg);
    //거리 계산에 응한다
    void OnFindNearestEnemyMessage(FindNearestEnemyMessage *msg);

public:
    virtual bool is_enemy();

    cocos2d::CCNode *layer() { return layer_; }
    
    float hit_point() const { return hit_point_; }
    void set_hit_point(float hit_point);
    
    float max_hit_point() { return max_hit_point_; }
    void set_max_hit_point(float max_hit_point) { max_hit_point_ = max_hit_point; }
    
    void set_unbeatable(bool unbeatable) { unbeatable_ = unbeatable; }

    //CharacterFSM *fsm() { return fsm_.get(); }

private:
    virtual void HandleOutOfBound(OutOfBoundMessage *msg);
    virtual void AfterDestroy() = 0;
    void RequestRecovery();
    void DrawHitPointBar();

private:
//    std::unique_ptr<CharacterFSM> fsm_;
    
    //무적 상태?
    bool unbeatable_;
    float max_hit_point_;
    float hit_point_;
    cocos2d::CCNode *layer_;

};


#endif