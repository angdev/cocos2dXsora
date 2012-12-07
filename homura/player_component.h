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
    virtual void CollideBullet(CollideBulletMessage *msg);


    virtual bool is_enemy() { return false; }
private:
    void Destroy();

private:
    //반사 로직
    b2Vec2 prev_body_pos;

    bool IsMoved();
    
    //반사 로직

private:
    //세부 로직을 아직은 분리하지 않는다.
    float reflect_timer_;
    bool reflecting_;
};

#endif