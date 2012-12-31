// Ŭnicode please
#include "stdafx.h"
#include "guided_laser_plane_component.h"

#include "game_object.h"
#include "game_world.h"

GuidedLaserPlaneComponent::GuidedLaserPlaneComponent(GameObject *obj, cocos2d::CCNode *layer) 
    : CharacterComponent(obj, layer), attack_cool_down_(1.0f), attack_keep_time_(3.0f),
    is_idle_state_(true), now_attacking_(false), attack_timer_(0), attack_target_id_(0) {

}

GuidedLaserPlaneComponent::~GuidedLaserPlaneComponent() {

}

void GuidedLaserPlaneComponent::Update(float dt) {
    CharacterComponent::Update(dt);

    if(is_idle_state_) {
        attack_timer_ += dt;
        if(attack_timer_ > attack_cool_down_) {
            is_idle_state_ = false;
            attack_timer_ = 0;
        }
    }

    //Test 필요
    if(now_attacking_) {
        attack_timer_ += dt;
        if(attack_timer_ > attack_keep_time_) {
            is_idle_state_ = true;
            attack_timer_ = 0;
        }
    }
    //공격 도중 끊었을때
    else if(!now_attacking_ && !is_idle_state_){
        attack_timer_ = 0;
        is_idle_state_ = true;
    }
    now_attacking_ = false;
}

void GuidedLaserPlaneComponent::InitMsgHandler() {
    CharacterComponent::InitMsgHandler();
    RegisterMsgFunc(this, &GuidedLaserPlaneComponent::OnAttackMessage);
}

void GuidedLaserPlaneComponent::Attack() {
    if(is_idle_state_)
        return;
    now_attacking_ = true;

    //타겟을 구함
    GameObjectPtr obj_ptr = obj()->world()->FindObject(attack_target_id_);
    if(obj_ptr == NULL)
        return;

    //레이저 그리기

    //데미지 주기
    DamageObjectMessage damage_msg = DamageObjectMessage::Create(0.1f);
    obj_ptr->OnMessage(&damage_msg);
}

void GuidedLaserPlaneComponent::OnAttackMessage(AttackMessage *msg) {
    //TODO
    if(now_attacking_ && msg->target_id != attack_target_id_) {
        is_idle_state_ = true;
        attack_target_id_ = msg->target_id;
        attack_timer_ = 0;
    }
    else if(!is_idle_state_) {
        attack_target_id_ = msg->target_id;
        Attack();
    }
}
