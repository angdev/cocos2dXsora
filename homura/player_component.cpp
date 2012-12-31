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
    RegisterMsgFunc(this, &PlayerComponent::OnRequestPlayerPositionMessage);
    RegisterMsgFunc(this, &PlayerComponent::OnRequestRecoveryMessage);
    RegisterMsgFunc(this, &PlayerComponent::OnCollidePlaneMessage);
}

void PlayerComponent::AfterDestroy() {
    
}

//반사 로직

bool PlayerComponent::IsMoved() {
    PhyBodyInfo body_info;
    RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&body_info);
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


//반사 로직 포함된 충돌 처리 로직
void PlayerComponent::CollideBullet( CollideBulletMessage *msg ) {
    if(reflecting_) {
        GameObject *bullet = msg->bullet;
        if(static_cast<BulletComponent*>(bullet->logic_comp())->from_enemy() == is_enemy())
            return;

        PhyBodyInfo player_body_info;
        RequestPhyBodyInfoMessage player_body_msg = RequestPhyBodyInfoMessage::Create(&player_body_info);
        obj()->OnMessage(&player_body_msg);
        assert(player_body_msg.is_ret && "get player's body info failed.");
        
        PhyBodyInfo bullet_body_info;
        RequestPhyBodyInfoMessage bullet_body_msg = RequestPhyBodyInfoMessage::Create(&bullet_body_info);
        bullet->OnMessage(&bullet_body_msg);
        assert(bullet_body_msg.is_ret && "get bullet's body info failed.");

        //TODO
        //총돌한 위치에서의 충돌 박스 면의 각도를 가져올 수 있나?
        // - world manifold 이용해서 충돌점과 normal vector 가져올 수 있음
        //일단 충돌면이 항상 x축이라고 가정
        float reflect_angle = (2 * M_PI - bullet_body_info.angle_rad + (player_body_info.angle_rad - M_PI_2));
        //cocos2d::CCLog("%f / %f : %f", player_body_info.angle_rad, bullet_body_info.angle_rad, reflect_angle);

        SetAngleMessage angle_msg = SetAngleMessage::Create(reflect_angle);
        bullet->OnMessage(&angle_msg);
        static_cast<BulletComponent*>(bullet->logic_comp())->set_from_enemy(false);
    }
    else {
        CharacterComponent::CollideBullet(msg);
    }
}


void PlayerComponent::OnRequestPlayerPositionMessage( RequestPlayerPositionMessage *msg ) {
    PhyBodyInfo body_info;
    RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&body_info);
    obj()->OnMessage(&body_info_msg);
    
    assert(body_info_msg.is_ret);

    msg->position->x = body_info.x;
    msg->position->y = body_info.y;
    msg->is_ret = true;
    
    return;
}

void PlayerComponent::OnRequestRecoveryMessage( RequestRecoveryMessage *msg ) {
    if(msg->char_comp->is_enemy() == is_enemy()) {
        float current_hit_point = msg->char_comp->hit_point();
        //회복량 때려박음
        //CCLOG("[recover]%f", current_hit_point + 0.2);
        msg->char_comp->set_hit_point(current_hit_point + 0.2);
    }
}

void PlayerComponent::HandleOutOfBound(OutOfBoundMessage *msg) {
    //이전 위치로 되돌림
    b2Vec2 pos_diff = msg->current_pos - msg->prev_pos;
    MoveMessage move_msg = MoveMessage::Create(-(pos_diff));
    obj()->OnMessage(&move_msg);
}

void PlayerComponent::OnCollidePlaneMessage(CollidePlaneMessage *msg) {
    //일단 부딪힌 객체가 캐릭터 컴포넌트를 가지는게 보장되고 있음
    CharacterComponent *counter_char_comp = static_cast<CharacterComponent*>(msg->counter_obj->logic_comp());
    if(counter_char_comp->is_enemy() != is_enemy()) {
        //데미지를 주자
        //set_hit_point가 있으니 써도 됨..
        //왜 메시지로 만들었지?
        DamageObjectMessage damage_msg = DamageObjectMessage::Create(1.0f);
        msg->counter_obj->OnMessage(&damage_msg);
        obj()->OnMessage(&damage_msg);

        //걍 적절히 민다
        //음.. 이걸 터치 핸들러랑 적절히 엮어야할텐데.
        //그리고 이렇게 밀면 너무 별로인듯?
        b2Vec2 player_body_pos = obj()->phy_comp()->main_body()->GetPosition();
        b2Vec2 counter_body_pos = msg->counter_obj->phy_comp()->main_body()->GetPosition();
        b2Vec2 push_vec(counter_body_pos.x - player_body_pos.x, counter_body_pos.y - player_body_pos.y);
        push_vec.Normalize();
        push_vec *= 10;

        MoveMessage move_msg = MoveMessage::Create(push_vec);
        msg->counter_obj->OnMessage(&move_msg);
        push_vec *= -1;
        move_msg.vec = push_vec;
        obj()->OnMessage(&move_msg);
    }
}

void PlayerComponent::OnResetMessage(ResetMessage *msg) {

}

void PlayerComponent::Destroy() {
    //플레이어는 여기서 안 없애고 비활성화 시킴
    obj()->Disable();

    AfterDestroy();
}
