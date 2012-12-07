// Ŭnicode please
#include "stdafx.h"
#include "phy_component.h"
#include "player_component.h"
#include "game_object.h"
#include "game_world.h"

//temp
#include "bullet_component.h"

PlayerComponent::PlayerComponent(GameObject *obj, cocos2d::CCNode *layer)
    : CharacterComponent(obj, layer), reflect_timer_(0), reflecting_(false) {

}

PlayerComponent::~PlayerComponent() {

}

void PlayerComponent::Update(float dt) {
    CharacterComponent::Update(dt);
    
    //반사 로직
    //일단 하드 코딩으로 때려박음
    if(reflecting_ && IsMoved()) {
        reflecting_ = false;
        reflect_timer_ = 0;
    }
    else {
        reflect_timer_ += dt;
        if(reflect_timer_ > 1.0f) {
            reflecting_ = true;
        }
    }
}

void PlayerComponent::InitMsgHandler() {
    CharacterComponent::InitMsgHandler();
}

void PlayerComponent::Destroy() {
    obj()->Disable();
}

//반사 로직

bool PlayerComponent::IsMoved() {
    PhyBodyInfo body_info;
    GetPhyBodyInfoMessage body_info_msg = GetPhyBodyInfoMessage::Create(&body_info);
    obj()->OnMessage(&body_info_msg);

    assert(body_info_msg.is_ret && "body_info is not returned.");

    if(glm::abs(prev_body_pos.x - body_info.x) < 0.001 && glm::abs(prev_body_pos.y - body_info.y) < 0.001) {
        return false;
    }
    else {
        prev_body_pos = b2Vec2(body_info.x, body_info.y);
        return true;
    }
}


void PlayerComponent::CollideBullet( CollideBulletMessage *msg ) {
    if(reflecting_) {
        GameObject *bullet = msg->bullet;
        PhyBodyInfo player_body_info;
        GetPhyBodyInfoMessage player_body_msg = GetPhyBodyInfoMessage::Create(&player_body_info);
        obj()->OnMessage(&player_body_msg);
        assert(player_body_msg.is_ret && "get player's body info failed.");
        
        PhyBodyInfo bullet_body_info;
        GetPhyBodyInfoMessage bullet_body_msg = GetPhyBodyInfoMessage::Create(&bullet_body_info);
        bullet->OnMessage(&bullet_body_msg);
        assert(bullet_body_msg.is_ret && "get bullet's body info failed.");

        float reflect_angle = (2 * M_PI - bullet_body_info.angle_rad + (player_body_info.angle_rad - M_PI_2));
        cocos2d::CCLog("%f / %f : %f", player_body_info.angle_rad, bullet_body_info.angle_rad, reflect_angle);

        SetAngleMessage angle_msg = SetAngleMessage::Create(reflect_angle);
        bullet->OnMessage(&angle_msg);
        static_cast<BulletComponent*>(bullet->logic_comp())->set_from_enemy(false);
    }
    else {
        CharacterComponent::CollideBullet(msg);
    }
}

//반사 로직