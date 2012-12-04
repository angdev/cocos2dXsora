// Ŭnicode please
#include "stdafx.h"
#include "character_component.h"
#include "game_object.h"
#include "phy_component.h"
#include "game_message.h"
#include "game_world.h"

//
#include <random>
#include "sora/unit.h"

CharacterComponent::CharacterComponent(GameObject *obj, cocos2d::CCNode *layer) 
    : LogicComponent(obj), layer_(layer) {

}

CharacterComponent::~CharacterComponent() {

}

void CharacterComponent::Update(float dt) {
    //체력을 가지고 있으므로 이걸 처리
    if(hit_point_ <= 0) {
        //Destroy
        obj()->OnMessage(&DestroyMessage::Create(obj()->id()));
        //OnDestroy -> drawable comp에서 터짐 처리 -> logic comp에서 world에 삭제 요청 보냄.
        //우선순위는 drawable > logic
    }

    //바라보는 방향에 따라서 스프라이트 회전
}

void CharacterComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &CharacterComponent::OnDestroyMessage);
    RegisterMsgFunc(this, &CharacterComponent::OnApplyDamage);
}

void CharacterComponent::OnDestroyMessage(DestroyMessage *msg) {
    cocos2d::CCLog("%d destroied", msg->obj_id);
    GameWorld *world = obj()->world();
    world->RequestRemoveObject(world->FindObject(msg->obj_id));
}

void CharacterComponent::OnApplyDamage(ApplyDamageMessage *msg) {
    if(msg->from_enemy && IsEnemy())
        return;
    hit_point_ -= msg->damage;
    msg->applied = true;
    cocos2d::CCLog("%f", hit_point_);
}

//CharacterComponent
