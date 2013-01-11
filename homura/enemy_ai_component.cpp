// Ŭnicode please
#include "stdafx.h"
#include "enemy_ai_component.h"
#include "game_object.h"
#include "phy_component.h"
#include "game_world.h"

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
        if(static_cast<SinglePhyComponent*>(obj()->phy_comp())->IsArrived()) {
            CCLOG("state changed");
            state_ = kEnemyNormalState;
        }
        
        else {
            MoveByMessage move_msg = MoveByMessage::Create(Unit::ToUnitFromMeter(velocity_vec), 1);
            obj()->OnMessage(&move_msg);
        }
    }
    else if(state_ == kEnemyNormalState) {
        //얘네들은 자기 맘대로 돌아다님
        GameWorld *world = obj()->world();
        b2Body *body = obj()->phy_comp()->main_body();
        b2Vec2 body_pos = body->GetPosition();
        FindNearestEnemyMessage find_msg = FindNearestEnemyMessage::Create(body_pos, true);
        world->OnMessage(&find_msg);
        
        if(find_msg.id == -1) {
            AttackMessage msg = AttackMessage::Create(0);
            obj()->OnMessage(&msg);
            return;
        }

        GameObjectPtr found_enemy = world->FindObject(find_msg.id);
        b2Vec2 enemy_pos = found_enemy->phy_comp()->main_body()->GetPosition();
        
        float x_diff = enemy_pos.x - body_pos.x;
        if(glm::abs(x_diff) > 5.0f) {
            glm::vec2 vec(Unit::ToUnitFromMeter(x_diff), 0);
            MoveByMessage move_msg = MoveByMessage::Create(vec, 1.0f);
            obj()->OnMessage(&move_msg);
        }
        else {
            SetAngleMessage ang_msg = SetAngleMessage::Create(-M_PI_2);
            obj()->OnMessage(&ang_msg);
            MoveByMessage move_msg = MoveByMessage::Create(glm::vec2(0, -0.001), 1.0f/60);
            //obj()->OnMessage(&move_msg);
            AttackMessage msg = AttackMessage::Create(find_msg.id);
            obj()->OnMessage(&msg);
        }
    }
    else if(state_ == kEnemyOutOfControlState) {
        //제어권을 벗어남
    }
}

void EnemyAIComponent::InitMsgHandler() {
    AIComponent::InitMsgHandler();
}

