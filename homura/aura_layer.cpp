// Ŭnicode please
#include "stdafx.h"
#include "aura_layer.h"

#include "game_world.h"
#include "game_object.h"

#include "sora/unit.h"

USING_NS_CC;
using namespace sora;

AuraLayer::AuraLayer(GameWorld *world) : world_(world) {

}

AuraLayer::~AuraLayer() {

}

bool AuraLayer::init() {
    if(!CCLayer::init())
        return false;

    RegisterMsgFunc(this, &AuraLayer::OnDestroyMessage);

    //파티클 배치 노드 초기화
    /*
    particle_node_ = new CCParticleBatchNode();
    particle_node_->initWithFile("particles/BoilingFoam.plist", 10);
    particle_node_->autorelease();
    this->addChild(particle_node_);
    */

    return true;
}

void AuraLayer::draw() {

}

void AuraLayer::RequestRenderAura(int id, const glm::vec2& pos) {
    auto found = aura_dict_.find(id);
    if(found != aura_dict_.end()) {
        found->second.particle->setPosition(pos.x, pos.y);
        return;
    }

    AuraRenderState state;

    CCParticleSystem *emitter = new CCParticleSystemQuad();
    emitter->initWithFile("particles/BoilingFoam.plist");
    assert(emitter != NULL);
    
    emitter->setPosition(pos.x, pos.y);
    this->addChild(emitter);
    emitter->autorelease();

    state.particle = emitter;

    aura_dict_.insert(std::make_pair(id, state));
}

void AuraLayer::StopRenderAura(int id) {
    auto found = aura_dict_.find(id);
    if(found != aura_dict_.end()) {
        found->second.particle->removeFromParent();
        aura_dict_.erase(id);
    }
}

void AuraLayer::OnDestroyMessage(DestroyMessage *msg) {
    //만약 플레이어가 죽었으면 다 없애야함
    if(world_->FindObject(msg->obj_id)->Type() == kCompPlayer) {
        aura_dict_.clear();
        return;
    }
    StopRenderAura(msg->obj_id);
}