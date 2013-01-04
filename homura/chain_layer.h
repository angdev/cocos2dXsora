// Ŭnicode please
#ifndef __CHAIN_LAYER_H__
#define __CHAIN_LAYER_H__

#include "message_handleable.h"

class GameWorld;

class ChainLayer : public cocos2d::CCLayer, public MessageHandleable {
public:
    ChainLayer(GameWorld *world);
    virtual ~ChainLayer();

public:
    bool init();
    void draw();
};

#endif