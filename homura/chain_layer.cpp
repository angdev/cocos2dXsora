// Ŭnicode please
#include "stdafx.h"
#include "chain_layer.h"

USING_NS_CC;

ChainLayer::ChainLayer(GameWorld *world) : world_(world) {
    
}

ChainLayer::~ChainLayer() {
    chain_dict_.clear();
}

bool ChainLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }

    //메시지 핸들러 등록
    RegisterMsgFunc(this, &ChainLayer::OnDestroyMessage);

    return true;
}

void ChainLayer::draw() {
    
    ccDrawColor4B(255, 255, 255, 255);

    for(auto dict_value : chain_dict_) {
        ccDrawLine(ccp(dict_value.second.master_pos.x, dict_value.second.master_pos.y), 
            ccp(dict_value.second.slave_pos.x, dict_value.second.slave_pos.y));
    }
}

void ChainLayer::RequestDraw(int id, const glm::vec2 &slave_pos, const glm::vec2 &master_pos) {
    //dict에 있으면 정보 업데이트
    auto found = chain_dict_.find(id);
    if(found != chain_dict_.end()) {
        found->second.master_pos = master_pos;
        found->second.slave_pos = slave_pos;
        return;
    }

    ChainRenderState state;
    state.master_pos = master_pos;
    state.slave_pos = slave_pos;
    chain_dict_.insert(std::make_pair(id, state));
}

void ChainLayer::StopDraw(int id) {
    chain_dict_.erase(id);
}

void ChainLayer::OnDestroyMessage(DestroyMessage *msg) {
    StopDraw(msg->obj_id);
}
