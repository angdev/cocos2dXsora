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

    virtual bool is_enemy() { return false; }
private:
    void Destroy();

};

#endif