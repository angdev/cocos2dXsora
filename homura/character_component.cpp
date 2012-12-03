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

CharacterComponent::CharacterComponent(GameObject *obj) 
    : LogicComponent(obj) {

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
}

void CharacterComponent::ApplyDamage(float damage) {
    cocos2d::CCLog("%f", hit_point_);
    hit_point_ -= damage;
}

void CharacterComponent::OnDestroy(DestroyMessage *msg) {
    cocos2d::CCLog("%d destroied", msg->obj_id);
    GameWorld *world = obj()->world();
    world->RequestRemoveObject(world->FindObject(msg->obj_id));
}

//CharacterComponent

AICharacterComponent::AICharacterComponent(GameObject *obj)
    : CharacterComponent(obj) {

}

AICharacterComponent::~AICharacterComponent() {

}

void AICharacterComponent::Update(float dt) {
    CharacterComponent::Update(dt);

    AIUpdate(dt);
}

void AICharacterComponent::AIUpdate(float dt) {
    GameObject *o = obj();
    if(NULL == o)
        return;
    
    PhyComponent *phy = o->phy_comp();
    if(NULL == phy)
        return;
    
    b2Body *body = phy->main_body();
    b2Vec2 body_pos = body->GetPosition();

    body_pos.y += sora::Unit::ToMeterFromUnit(1.0f);

    body->SetTransform(body_pos, body->GetAngle());
    body->SetAwake(true);
}