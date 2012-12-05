// Ŭnicode please
#include "stdafx.h"
#include "bullet_component.h"
#include "game_object.h"
#include "phy_component.h"
#include "data_define.h"
#include "sora/unit.h"
#include "game_world.h"

using namespace sora;

BulletComponent::BulletComponent(GameObject *obj)
    : LogicComponent(obj), speed_(0), damage_(0), from_enemy_(false) {

}


BulletComponent::~BulletComponent() {

}

void BulletComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &BulletComponent::OnDamageObjectMessage);
    RegisterMsgFunc(this, &BulletComponent::OnDestroyMessage);
}

void BulletComponent::Update(float dt) {
    //TODO
    //일단 방향 정해진대로 날아가도록만 하였다
    PhyBodyInfo body_info;
    GetPhyBodyInfoMessage body_info_msg = GetPhyBodyInfoMessage::Create(&body_info);
    obj()->OnMessage(&body_info_msg);

    if(!body_info_msg.is_ret)
        return;

    glm::vec2 velocity_vec(glm::cos(body_info.angle_rad), glm::sin(body_info.angle_rad));
    velocity_vec *= speed_;
    MoveMessage move_msg = MoveMessage::Create(velocity_vec);
    obj()->OnMessage(&move_msg);
}

void BulletComponent::OnDamageObjectMessage(DamageObjectMessage *msg) {
    ApplyDamageMessage apply_msg = ApplyDamageMessage::Create(damage_, from_enemy_);
    msg->obj->OnMessage(&apply_msg);
    
    //리턴값. 맞았는가에 대한 체크
    if(apply_msg.applied) {
        DestroyMessage msg = DestroyMessage::Create(obj()->id());
        OnMessage(&msg);
    }
}

void BulletComponent::OnDestroyMessage(DestroyMessage *msg) {
    GameWorld *world = obj()->world();
    world->RequestRemoveObject(world->FindObject(msg->obj_id));
}
