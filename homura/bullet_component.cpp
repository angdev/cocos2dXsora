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
    : LogicComponent(obj), dir_vec_px_(0, 0) {

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
    obj()->OnMessage(&MoveMessage::Create(dir_vec_px_));
}

void BulletComponent::OnDamageObjectMessage(DamageObjectMessage *msg) {
    msg->obj->OnMessage(&ApplyDamageMessage::Create(damage_));
    OnMessage(&DestroyMessage::Create(obj()->id()));
}

void BulletComponent::OnDestroyMessage(DestroyMessage *msg) {
    GameWorld *world = obj()->world();
    world->RequestRemoveObject(world->FindObject(msg->obj_id));
}