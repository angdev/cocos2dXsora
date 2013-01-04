// Ŭnicode please
#ifndef __SHIELD_LAYER_H__
#define __SHIELD_LAYER_H__

#include "message_handleable.h"

class GameWorld;

//여기서 쉴드를 그리고
//플레이어 반사막을 그린다
//이름 때문에 헷갈릴 것 같기도 하고..
class ShieldLayer : public cocos2d::CCLayer, public MessageHandleable {
public:
    ShieldLayer(GameWorld *world);
    virtual ~ShieldLayer();

public:
    bool init();
    void draw();

public:
    void set_player_reflect_state(bool player_reflect_state) { player_reflect_state_ = player_reflect_state; }
    
private:
    void DrawReflectionShield();
    //cocos2d::CCPoint GetPlayerPosition();

private:
    
    GameWorld *world_;

    bool player_reflect_state_;
};

#endif