// Ŭnicode please
#include "stdafx.h"
#include "enemy_ai_component.h"
#include "game_object.h"
#include "phy_component.h"

#include "sora/unit.h"

using namespace sora;

EnemyAIComponent::EnemyAIComponent(GameObject *obj) : AIComponent(obj), state_(kEnemyAppearState) {

}

EnemyAIComponent::~EnemyAIComponent() {

}

void EnemyAIComponent::Update(float dt) {
    if(state_ == kEnemyAppearState) {
        //정해진 위치까지 이동
        
        PhyBodyInfo body_info;
        RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&body_info);
        obj()->OnMessage(&body_info_msg);

        assert(body_info_msg.is_ret && "enemy ai body info error");

        b2Vec2 body_vec(body_info.x, body_info.y);
        b2Vec2 velocity_vec = start_position() - body_vec;
        if(velocity_vec.Length() < Unit::ToMeterFromUnit(30)) {
            CCLOG("state changed");
            state_ = kEnemyNormalState;
        }
        
        else {

            MoveByMessage move_msg = MoveByMessage::Create(Unit::ToUnitFromMeter(velocity_vec), 1);
            obj()->OnMessage(&move_msg);
        }
    }
    else if(state_ == kEnemyNormalState) {
        MoveByMessage move_msg = MoveByMessage::Create(glm::vec2(0, -50), 0.5);
        obj()->OnMessage(&move_msg);
        AttackMessage atk_msg = AttackMessage::Create(0);
        obj()->OnMessage(&atk_msg);
    }
}

void EnemyAIComponent::InitMsgHandler() {
    AIComponent::InitMsgHandler();
}

