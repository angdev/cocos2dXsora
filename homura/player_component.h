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
    virtual void Destroy();
    virtual void InitMsgHandler();
    
public:
    //메시지 처리
    virtual void CollideBullet(CollideBulletMessage *msg);
    void OnRequestPlayerPositionMessage(RequestPlayerPositionMessage *msg);
    void OnRequestRecoveryMessage(RequestRecoveryMessage *msg); //일단 플레이어는 항상 아군이라고 가정.
    void OnCollidePlaneMessage(CollidePlaneMessage *msg);   //박치기했을 때
    //초기화를 새로 해주자
    void OnResetMessage(ResetMessage *msg);
    void OnIsEnemyMessage(IsEnemyMessage *msg);


public:
    virtual bool is_enemy() { return false; }

public:
    //필살기! 토카막 필드
    void UseTokamakField();
    void EndTokamakField();

    //전부 쉴드 주는 필살기
    void UsePowerShield();
    void EndPowerShield();

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
    //대충 구현되어 있음

    
private:
    //세부 로직을 아직은 분리하지 않는다.
    //반사 타이머
    float reflect_timer_;
    bool reflecting_;

    //토카막 타이머
    //타이머 클래스 만들기
    float tokamak_timer_;
    bool using_tokamak_;
    bool can_use_tokamak_;

    //파워 쉴드 타이머
    float power_shield_timer_;
    bool using_power_shield_;
    bool can_use_power_shield_;
};

#endif