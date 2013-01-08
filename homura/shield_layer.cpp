// Ŭnicode please
#include "stdafx.h"
#include "shield_layer.h"

#include "game_world.h"

#include "sora/unit.h"

using namespace sora;
USING_NS_CC;

ShieldLayer::ShieldLayer(GameWorld *world) : world_(world) {

}

ShieldLayer::~ShieldLayer() {

}

bool ShieldLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }

    shield_sprite_batch_ = CCSpriteBatchNode::create("power_shield.png");
    this->addChild(shield_sprite_batch_);

    RegisterMsgFunc(this, &ShieldLayer::OnDestroyMessage);

    return true;
}

void ShieldLayer::draw() {
    if(player_reflect_state_)
        RenderReflectionShield();
}

void ShieldLayer::RenderReflectionShield() {
    //쉴드 크기는 하드코딩으로 박아놓고 이후에 예쁘게 그리기를 시도하면서 고치자

    b2Vec2 pos;
    RequestPlayerPositionMessage msg = RequestPlayerPositionMessage::Create(&pos);
    world_->OnMessage(&msg);

    //여기서는 assert할 필요없이 안 돌아오면 안 그리면 된다.
    if(!msg.is_ret) {
        player_reflect_state_ = false;
        return;
    }

    //둥근 원을 그려보자
    glLineWidth(2.0f);
    ccDrawColor4B(0xff, 0xff, 0xff, 0xff);
    ccDrawCircle(ccp(Unit::ToUnitFromMeter(pos.x), Unit::ToUnitFromMeter(pos.y)), 60, 0, 50, false);
}

void ShieldLayer::RequestRenderTokamakField(int id, const glm::vec2 &pos) {
    RequestRenderShield(id, pos);
}

void ShieldLayer::RequestRenderShield(int id, const glm::vec2 &pos) {
    auto found = shield_dict_.find(id);
    if(found != shield_dict_.end()) {
        found->second.sprite->setPosition(ccp(pos.x, pos.y));
        return;
    }

    ShieldRenderState state;
    state.sprite = CCSprite::createWithTexture(shield_sprite_batch_->getTexture());
    state.sprite->setPosition(ccp(pos.x, pos.y));
    shield_sprite_batch_->addChild(state.sprite);
    shield_dict_.insert(std::make_pair(id, state));
}

void ShieldLayer::StopRenderTokamakField(int id) {
    StopRenderShield(id);
}

void ShieldLayer::StopRenderShield(int id) {
    auto found = shield_dict_.find(id);
    if(found != shield_dict_.end()) {
        found->second.sprite->removeFromParent();
        shield_dict_.erase(id);
    }
}

void ShieldLayer::OnDestroyMessage(DestroyMessage *msg) {
    StopRenderShield(msg->obj_id);
}
