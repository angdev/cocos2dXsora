// Ŭnicode please
#include "stdafx.h"
#include "enemy_ai_component.h"
#include "game_object.h"
#include "phy_component.h"
#include "game_world.h"

#include "sora/unit.h"

using namespace sora;
USING_NS_CC;

EnemyAIComponent::EnemyAIComponent(GameObject *obj) : AIComponent(obj), state_(kEnemyAppearState), elapsed_time_(0) {

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
        static int seed = 0;
        b2Body *body = obj()->phy_comp()->main_body();
        b2Vec2 body_pos = body->GetPosition();
        
        CCSize win_size = CCDirector::sharedDirector()->getWinSize();
        float x_px = Unit::ToUnitFromMeter(body_pos.x), y_px = Unit::ToUnitFromMeter(body_pos.y);
        if(x_px < 0 || x_px > win_size.width || y_px < 0 || y_px > win_size.height) {
            MoveToMessage msg = MoveToMessage::Create(glm::vec2(std::default_random_engine((unsigned int)time(0) + seed++)() % 720, 
                1000), 1);
            obj()->OnMessage(&msg);
            moved_ = true;
            return;
        }
        
        FindNearestEnemyMessage find_msg = FindNearestEnemyMessage::Create(body_pos, true);
        world->OnMessage(&find_msg);
        

        if(find_msg.id == -1) {
            if(static_cast<SinglePhyComponent*>(obj()->phy_comp())->IsArrived() || !moved_) {
                MoveToMessage msg = MoveToMessage::Create(glm::vec2(std::default_random_engine((unsigned int)time(0) + seed++)() % 720, 
                    Unit::ToUnitFromMeter(obj()->phy_comp()->main_body()->GetPosition().y)), 1);
                obj()->OnMessage(&msg);
                moved_ = true;
            }
            if(moved_) {
                SetAngleMessage ang_msg = SetAngleMessage::Create(-M_PI_2);
                obj()->OnMessage(&ang_msg);
                AttackMessage msg = AttackMessage::Create(0);
                obj()->OnMessage(&msg);
                elapsed_time_ += 1.0/60;
                if(elapsed_time_ > 3) {
                    elapsed_time_ = 0;
                    moved_ = false;
                }
            }
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

