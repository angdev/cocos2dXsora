﻿// Ŭnicode please
#include "stdafx.h"
#include "character_component.h"
#include "game_object.h"
#include "phy_component.h"
#include "game_message.h"
#include "game_world.h"

#include <random>
#include "sora/unit.h"

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

CharacterComponent::CharacterComponent(GameObject *obj, cocos2d::CCNode *layer) 
    : LogicComponent(obj), layer_(layer), is_enemy_(false) {

}

CharacterComponent::~CharacterComponent() {

}

void CharacterComponent::Update(float dt) {
    //체력을 가지고 있으므로 이걸 처리
    if(hit_point_ <= 0) {
        //Destroy
        DestroyMessage msg = DestroyMessage::Create(obj()->id());
        obj()->OnMessage(&msg);
        //OnDestroy -> drawable comp에서 터짐 처리 -> logic comp에서 world에 삭제 요청 보냄.
        //우선순위는 drawable > logic
    }
}

void CharacterComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &CharacterComponent::OnDestroyMessage);
    RegisterMsgFunc(this, &CharacterComponent::OnCollideBulletMessage);
}

void CharacterComponent::OnDestroyMessage(DestroyMessage *msg) {
    cocos2d::CCLog("%d destroied", msg->obj_id);
    Destroy();
}

void CharacterComponent::OnCollideBulletMessage(CollideBulletMessage *msg) {
    CollideBullet(msg);
}

void CharacterComponent::CollideBullet(CollideBulletMessage *msg) {
    if(msg->from_enemy == is_enemy())
        return;
    hit_point_ -= msg->damage;
    msg->applied = true;
    cocos2d::CCLog("%f", hit_point_);

}

//CharacterComponent
