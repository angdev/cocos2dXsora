// Ŭnicode please
#ifndef __PLAYER_COMPONENT_H__
#define __PLAYER_COMPONENT_H__

#include "character_component.h"

class PlayerComponent : public CharacterComponent {
public:
    PlayerComponent(GameObject *obj, cocos2d::CCNode *layer);
    ~PlayerComponent();

public:
    virtual CompType type() const { return kCompPlayer; }
	virtual void Update(float dt);
    virtual void InitMsgHandler();
    
public:
    //메시지 처리
    virtual void CollideBullet(CollideBulletMessage *msg);
    void OnRequestPlayerPositionMessage(RequestPlayerPositionMessage *msg);
    void OnRequestRecoveryMessage(RequestRecoveryMessage *msg); //일단 플레이어는 항상 아군이라고 가정.


public:
    virtual bool is_enemy() { return false; }
private:
    void AfterDestroy();
    void HandleOutOfBound(OutOfBoundMessage *msg);

private:
    //반사 로직
    b2Vec2 prev_body_pos;

    bool IsMoved();
    
    //회복 로직
    void RecoverAlly();

    //TODO: 필살기 2 로직 구현하기
    //그냥 쿨다운 로직이랑 메시지 브로드캐스팅만 하면 끝.
    
private:
    //세부 로직을 아직은 분리하지 않는다.
    float reflect_timer_;
    bool reflecting_;
};

#endif