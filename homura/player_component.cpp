// Ŭnicode please
#include "stdafx.h"
#include "phy_component.h"
#include "player_component.h"
#include "game_object.h"
#include "game_world.h"
#include "game_info_layer.h"
#include "shield_layer.h"
#include "aura_layer.h"

//temp
#include "bullet_component.h"

#include "sora/unit.h"

USING_NS_CC;
using namespace sora;

PlayerComponent::PlayerComponent(GameObject *obj, cocos2d::CCNode *layer)
    : CharacterComponent(obj, layer), reflect_timer_(0), reflecting_(false), tokamak_timer_(0), using_tokamak_(false), can_use_tokamak_(true) {

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

    //토카막 필드
    if(using_tokamak_) {
        obj()->world()->shield_layer->RequestRenderTokamakField(obj()->id(), 
            Unit::ToUnitFromMeter(obj()->phy_comp()->main_body()->GetPosition()));
        tokamak_timer_ += dt;
        if(tokamak_timer_ > 5) {
            EndTokamakField();
        }
    }
    else {
        tokamak_timer_ += dt;
        if(tokamak_timer_ > 60) {
            can_use_tokamak_ = true;
        }
    }

    //파워 쉴드
    if(using_power_shield_) {
        power_shield_timer_ += dt;
        if(power_shield_timer_ > 15) {
            EndPowerShield();
        }
    }
    else {
        power_shield_timer_ += dt;
        if(power_shield_timer_ > 5) {
            can_use_power_shield_ = true;
        }
    }

    //이 부분을 레이어 관련으로 묶자
    //반사 쉴드 그리기
    //크기는 어떻게 할까
    obj()->world()->shield_layer->set_player_reflect_state(reflecting_);
    //체력을 info_layer에 넘겨줌
    obj()->world()->game_info_layer->set_player_hit_point(hit_point());
}

void PlayerComponent::InitMsgHandler() {
    CharacterComponent::InitMsgHandler();
    RegisterMsgFunc(this, &PlayerComponent::OnRequestPlayerPositionMessage);
    RegisterMsgFunc(this, &PlayerComponent::OnRequestRecoveryMessage);
    RegisterMsgFunc(this, &PlayerComponent::OnCollidePlaneMessage);
    RegisterMsgFunc(this, &PlayerComponent::OnIsEnemyMessage);
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
        /*
        PhyBodyInfo player_body_info;
        RequestPhyBodyInfoMessage player_body_msg = RequestPhyBodyInfoMessage::Create(&player_body_info);
        obj()->OnMessage(&player_body_msg);
        assert(player_body_msg.is_ret && "get player's body info failed.");
        
        PhyBodyInfo bullet_body_info;
        RequestPhyBodyInfoMessage bullet_body_msg = RequestPhyBodyInfoMessage::Create(&bullet_body_info);
        bullet->OnMessage(&bullet_body_msg);
        assert(bullet_body_msg.is_ret && "get bullet's body info failed.");
        */

        b2Body *bullet_body = bullet->phy_comp()->main_body();

        //TODO
        //총돌한 위치에서의 충돌 박스 면의 각도를 가져올 수 있나?
        // - world manifold 이용해서 충돌점과 normal vector 가져올 수 있음
        b2Vec2 bullet_dir_vec = bullet_body->GetLinearVelocity();
        bullet_dir_vec.Normalize();
        float cross_value = std::abs(b2Cross(msg->manifold.normal, -bullet_dir_vec));
        float normal_vec_angle = std::atan2(msg->manifold.normal.y, msg->manifold.normal.x);
        float between_angle = std::asin(cross_value);
        if(normal_vec_angle < M_PI_2)
            between_angle = -between_angle;
        float reflect_angle = between_angle + normal_vec_angle;

        
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
    if(msg->char_comp->is_enemy() == is_enemy() && msg->char_comp->obj()->IsEnabled()) {
        float current_hit_point = msg->char_comp->hit_point();
        //회복량 때려박음
        //CCLOG("[recover]%f", current_hit_point + 0.2);
        msg->char_comp->set_hit_point(current_hit_point + 0.2);

        glm::vec2 obj_pos = Unit::ToUnitFromMeter(msg->char_comp->obj()->phy_comp()->main_body()->GetPosition());
        obj()->world()->aura_layer->RequestRenderAura(msg->char_comp->obj()->id(), obj_pos);
    }
}

void PlayerComponent::HandleOutOfBound(OutOfBoundMessage *msg) {
    //이전 위치로 되돌림
    /*
    b2Vec2 pos_diff = msg->current_pos - msg->prev_pos;
    pos_diff.x = Unit::ToUnitFromMeter(pos_diff.x);
    pos_diff.y = Unit::ToUnitFromMeter(pos_diff.y);
    MoveMessage move_msg = MoveMessage::Create(-(pos_diff));
    obj()->OnMessage(&move_msg);
    */
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


        //미는거 다시 제대로 구현하자
        /*
        b2Vec2 player_body_pos = obj()->phy_comp()->main_body()->GetPosition();
        b2Vec2 counter_body_pos = msg->counter_obj->phy_comp()->main_body()->GetPosition();
        b2Vec2 push_vec(counter_body_pos.x - player_body_pos.x, counter_body_pos.y - player_body_pos.y);
        push_vec.Normalize();
        push_vec *= 10;
        MoveByMessage move_msg = MoveByMessage::Create(Unit::ToUnitFromMeter(push_vec), 0.016);
        msg->counter_obj->OnMessage(&move_msg);
        push_vec *= -1;
        move_msg.vec = Unit::ToUnitFromMeter(push_vec);
        obj()->OnMessage(&move_msg);
        */
    }
}

void PlayerComponent::OnResetMessage(ResetMessage *msg) {

}

void PlayerComponent::Destroy() {
    //플레이어는 여기서 안 없애고 비활성화 시킴
    obj()->Disable();

    AfterDestroy();
}

//player는 ai comp를 안 가져서 만들어줘야함
void PlayerComponent::OnIsEnemyMessage( IsEnemyMessage *msg ) {
    msg->is_ret = true;
    msg->is_enemy = false;
}

void PlayerComponent::UseTokamakField() {
    set_unbeatable(true);
    using_tokamak_ = true;
    can_use_tokamak_ = false;
    tokamak_timer_ = 0;
}

void PlayerComponent::EndTokamakField() {
    using_tokamak_ = false;
    tokamak_timer_ = 0;
    obj()->world()->shield_layer->StopRenderTokamakField(obj()->id());
    set_unbeatable(false);
}

void PlayerComponent::UsePowerShield() {
    if(!can_use_power_shield_) {
        return;
    }

    power_shield_timer_ = 0;
    can_use_power_shield_ = false;
    using_power_shield_ = true;

    CreateShieldMessage msg = CreateShieldMessage::Create(false);
    obj()->world()->OnMessage(&msg);
}

void PlayerComponent::EndPowerShield() {
    using_power_shield_ = false;
    power_shield_timer_ = 0;
}