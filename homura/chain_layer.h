﻿// Ŭnicode please
#ifndef __CHAIN_LAYER_H__
#define __CHAIN_LAYER_H__

#include "message_handleable.h"

class GameWorld;

struct ChainRenderState {
    //묶인 체인 스프라이트 - slave 위에 그리기
    cocos2d::CCSprite *tie_sprite_;
    glm::vec2 slave_pos;
    glm::vec2 master_pos;
};

class ChainLayer : public cocos2d::CCLayer, public MessageHandleable {
public:
    ChainLayer(GameWorld *world);
    virtual ~ChainLayer();

public:
    bool init();
    void draw();

    void RequestDraw(int id, const glm::vec2 &slave_pos, const glm::vec2 &master_pos);
    void StopDraw(int id);

    void OnDestroyMessage(DestroyMessage *msg);

private:
    GameWorld *world_;

    typedef std::unordered_map<int, ChainRenderState> ChainStateDict;
    ChainStateDict chain_dict_;

    cocos2d::CCSpriteBatchNode *tie_sprite_batch_;
    cocos2d::CCTexture2D *chain_tex_;
};

#endif