// Ŭnicode please
#include "stdafx.h"
#include "laser_layer.h"

#include "game_object.h"
#include "game_world.h"
#include "game_stage.h"

#include "phy_component.h"

#include "sora/unit.h"

USING_NS_CC;
using namespace sora;

LaserLayer::LaserLayer(GameObject *obj, CCNode *parent) : LogicComponent(obj), parent_(parent) {

}

LaserLayer::~LaserLayer() {

}

CompType LaserLayer::type() const {
    return kCompLaserLayer;
}

void LaserLayer::Update(float dt) {
    
    for(auto iter : state_table_) {
        if(!iter.second.now_rendering)
            continue;
        //적당히 end_point에 길이를 맞추고 위치도 적절히 이동
        CCSprite *sprite = iter.second.sprite;
        
        PhyBodyInfo body_info;
        RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&body_info);

        iter.second.obj->OnMessage(&body_info_msg);
        assert(body_info_msg.is_ret && "laser layer body info error");
        glm::vec2 body_pos(Unit::ToUnitFromMeter(body_info.x), Unit::ToUnitFromMeter(body_info.y));
        sprite->setPosition(ccp(body_pos.x, body_pos.y));

        //길이 조절?
        CCSize sprite_size = sprite->getContentSize();
        glm::vec2 diff_vec = (iter.second.end_point - body_pos);
        sprite->setScaleY(glm::length((iter.second.end_point - body_pos)) / sprite_size.height);
    }
    
}

void LaserLayer::InitMsgHandler() {
    RegisterMsgFunc(this, &LaserLayer::OnRequestRenderLaserMessage);
    RegisterMsgFunc(this, &LaserLayer::OnStopRenderLaserMessage);
    RegisterMsgFunc(this, &LaserLayer::OnDestroyMessage);
}

void LaserLayer::OnRequestRenderLaserMessage(RequestRenderLaserMessage *msg) {
    //자기도 그려달라 이런 식으로 요청하는 녀석들을 맵에 넣는다
    auto found = state_table_.find(msg->id);
    if(found != state_table_.end()) {
        
        found->second.now_rendering = true;
        found->second.sprite->setVisible(true);
        found->second.end_point = msg->end_point;
       
    }
    else {
        LaserRenderState state;
        state.now_rendering = true;

        if(state.is_enemy_type) {
            CCSprite *sprite = CCSprite::create("enemy_laser.png");
            CCSize sprite_size = sprite->getContentSize();
            sprite->setAnchorPoint(ccp(0.5, 0));
            sprite->setRotation(180);
            parent_->addChild(sprite);
            
            state.sprite = sprite;
        }
        else {
            CCSprite *sprite = CCSprite::create("enemy_laser.png");
            CCSize sprite_size = sprite->getContentSize();
            sprite->setAnchorPoint(ccp(0.5, 1));
            parent_->addChild(sprite);

            state.sprite = sprite;
        }
        
        state.end_point = msg->end_point;

        state.obj = obj()->world()->FindObject(msg->id);
        state_table_.insert(std::make_pair(msg->id, state));

    }
}

void LaserLayer::OnStopRenderLaserMessage(StopRenderLaserMessage *msg) {
    
    auto found = state_table_.find(msg->id);
    if(found != state_table_.end()) {
        found->second.now_rendering = false;
        found->second.sprite->setVisible(false);
    }
    
}

void LaserLayer::OnDestroyMessage(DestroyMessage *msg) {
    auto found = state_table_.find(msg->obj_id);
    if(found != state_table_.end()) {
        obj()->world()->stage()->layer()->removeChild(found->second.sprite);
        state_table_.erase(found);
    }
}

void LaserLayer::StartDraw() {

}
