// Ŭnicode please
#ifndef __AURA_LAYER_H__
#define __AURA_LAYER_H__

#include "message_handleable.h"

class GameWorld;

struct AuraRenderState {
    cocos2d::CCParticleSystem *particle;
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
    void RequestRenderPlayerAura(int id, const glm::vec2 &pos);
    void RequestRenderAura(int id, const glm::vec2 &pos);
    void StopRenderAura(int id);

private:
    GameWorld *world_;

    cocos2d::CCParticleBatchNode *particle_node_;
    //회복을 받는 대상의 id가 key.
    typedef std::unordered_map<int, AuraRenderState> AuraStateDict;
    AuraStateDict aura_dict_;
};

#endif