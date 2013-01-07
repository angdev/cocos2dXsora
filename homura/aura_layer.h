// Ŭnicode please
#ifndef __AURA_LAYER_H__
#define __AURA_LAYER_H__

#include "message_handleable.h"

class GameWorld;

class AuraLayer : public cocos2d::CCLayer, MessageHandleable {
public:
    AuraLayer(GameWorld *world);
    virtual ~AuraLayer();

public:
    bool init();
    void draw();

private:
    GameWorld *world_;
};

#endif