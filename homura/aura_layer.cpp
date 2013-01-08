// Ŭnicode please
#include "stdafx.h"
#include "aura_layer.h"

#include "game_world.h"
#include "game_object.h"

USING_NS_CC;

AuraLayer::AuraLayer(GameWorld *world) : world_(world) {

}

AuraLayer::~AuraLayer() {

}

bool AuraLayer::init() {
    if(!CCLayer::init())
        return false;

    RegisterMsgFunc(this, &AuraLayer::OnDestroyMessage);

    aura_sprite_ = CCSprite::create("laser_friend.png");
    SR_ASSERT(aura_sprite_ != nullptr);
    this->addChild(aura_sprite_);
    aura_sprite_->setVisible(false);

    //set shader
    CCGLProgram *prog = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
    this->setShaderProgram(prog);

    return true;
}

void AuraLayer::draw() {
    for(auto value : aura_dict_) {
        const AuraRenderState& s = value.second;
        ccDrawLine(ccp(s.start_pos.x, s.start_pos.y), ccp(s.end_pos.x, s.end_pos.y));
    }
}

void AuraLayer::RequestRenderAura(int id, const glm::vec2 &start_pos, const glm::vec2 &end_pos) {
    auto found = aura_dict_.find(id);
    if(found != aura_dict_.end()) {
        found->second.start_pos = start_pos;
        found->second.end_pos = end_pos;
        return;
    }

    AuraRenderState state;
    state.start_pos = start_pos;
    state.end_pos = end_pos;
    aura_dict_.insert(std::make_pair(id, state));
}

void AuraLayer::StopRenderAura(int id) {
    aura_dict_.erase(id);
}

void AuraLayer::OnDestroyMessage(DestroyMessage *msg) {
    //만약 플레이어가 죽었으면 다 없애야함
    if(world_->FindObject(msg->obj_id)->Type() == kCompPlayer) {
        aura_dict_.clear();
        return;
    }
    StopRenderAura(msg->obj_id);
}