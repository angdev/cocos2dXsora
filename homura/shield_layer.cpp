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

    return true;
}

void ShieldLayer::draw() {
    if(player_reflect_state_)
        DrawReflectionShield();
}

void ShieldLayer::DrawReflectionShield() {
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
    ccDrawCircle(ccp(Unit::ToUnitFromMeter(pos.x), Unit::ToUnitFromMeter(pos.y)), 80, 0, 50, false);
}
