// Ŭnicode please
#ifndef __AURA_LAYER_H__
#define __AURA_LAYER_H__

#include "message_handleable.h"

class GameWorld;

struct AuraRenderState {
    glm::vec2 start_pos;
    glm::vec2 end_pos;
};

class AuraLayer : public cocos2d::CCLayer, public MessageHandleable {
public:
    AuraLayer(GameWorld *world);
    virtual ~AuraLayer();

public:
    bool init();
    void draw();

public:
    void OnDestroyMessage(DestroyMessage *msg);

public:
    void RequestRenderAura(int id, const glm::vec2 &start_pos, const glm::vec2 &end_pos);
    void StopRenderAura(int id);

private:
    GameWorld *world_;

    cocos2d::CCSprite *aura_sprite_;
    //회복을 받는 대상의 id가 key.
    typedef std::unordered_map<int, AuraRenderState> AuraStateDict;
    AuraStateDict aura_dict_;
};

#endif