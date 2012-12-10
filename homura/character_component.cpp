// Ŭnicode please
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

    //캐릭터 컴포넌트를 가지는 녀석들은 플레이어 객체에 회복을 요청할 수 있다.
    //플레이어 빼고
    if(type() != kCompPlayer) {
        RequestRecovery();
    }
}

void CharacterComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &CharacterComponent::OnDestroyMessage);
    RegisterMsgFunc(this, &CharacterComponent::OnCollideBulletMessage);
    RegisterMsgFunc(this, &CharacterComponent::OnOutOfBoundMessage);
    RegisterMsgFunc(this, &CharacterComponent::OnDamageObjectMessage);
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
    //cocos2d::CCLog("%f", hit_point_);

}

//플레이어에게 회복 요청
void CharacterComponent::RequestRecovery() {
    b2Vec2 player_pos;
    RequestPlayerPositionMessage req_player_pos_msg = RequestPlayerPositionMessage::Create(&player_pos);
    obj()->world()->OnMessage(&req_player_pos_msg);

    if(!req_player_pos_msg.is_ret) {
        return;
    }

    PhyBodyInfo obj_body_info;
    RequestPhyBodyInfoMessage req_body_info_msg = RequestPhyBodyInfoMessage::Create(&obj_body_info);
    obj()->OnMessage(&req_body_info_msg);

    assert(req_body_info_msg.is_ret);

    //거리 계산
    float distance = glm::distance(glm::vec2(player_pos.x, player_pos.y), glm::vec2(obj_body_info.x, obj_body_info.y));
    if(distance > 3.0 /* 플레이어 정보 만들어야 함 */) { //단위는 미터
        return;
    }

    RequestRecoveryMessage req_recovery_msg = RequestRecoveryMessage::Create(this);
    obj()->world()->OnMessage(&req_recovery_msg);
    
    return;
}

void CharacterComponent::OnOutOfBoundMessage( OutOfBoundMessage *msg ) {
    //이전 위치로 되돌림
    b2Vec2 pos_diff = msg->current_pos - msg->prev_pos;
    MoveMessage move_msg = MoveMessage::Create(-(pos_diff));
    obj()->OnMessage(&move_msg);
}

void CharacterComponent::OnDamageObjectMessage(DamageObjectMessage *msg) {
    hit_point_ -= msg->damage;
}

//CharacterComponent
