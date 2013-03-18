// Ŭnicode please
#ifndef __PLAYER_COMPONENT_H__
#define __PLAYER_COMPONENT_H__

#include "character_component.h"
#include "action_timer.h"

class PlayerComponent : public CharacterComponent {
public:
    PlayerComponent(GameObject *obj, cocos2d::CCNode *layer);
    ~PlayerComponent();

public:
    virtual CompType type() const { return kCompPlayer; }
	virtual void DerivedUpdate(float dt);
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
    bool IsAvailableTokamak();

    //전부 쉴드 주는 필살기
    void UsePowerShield();
    bool IsAvailablePowerShield();
    
    void TokamakFieldUpdate(float dt);
    void PowerShieldUpdate(float dt);
    void ReflectShieldUpdate(float dt);

private:
    void AfterDestroy();
    void HandleOutOfBound(OutOfBoundMessage *msg);

private:
    void BeginTokamakField();
    void ActiveTokamakField(float dt);
    void EndTokamakField();

    void BeginPowerShield();
    void EndPowerShield();


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
    std::unique_ptr< ActionTimer<PlayerComponent> > tokamak_timer_;
    std::unique_ptr< ActionTimer<PlayerComponent> > power_shield_timer_;
    std::unique_ptr< ActionTimer<PlayerComponent> > reflect_timer_;
};

#endif