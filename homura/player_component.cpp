// Ŭnicode please
#include "stdafx.h"
#include "phy_component.h"
#include "player_component.h"
#include "game_object.h"
#include "game_world.h"
#include "game_info_layer.h"
#include "shield_layer.h"
#include "aura_layer.h"

#include "action_timer.h"

//temp
#include "bullet_component.h"
#include "drawable_component.h"

#include "sora/unit.h"

USING_NS_CC;
using namespace sora;

PlayerComponent::PlayerComponent(GameObject *obj, cocos2d::CCNode *layer)
    : 
    CharacterComponent(obj, layer) {
    tokamak_timer_.reset(new ActionTimer(30, 5));
    power_shield_timer_.reset(new ActionTimer(15, 5));
    reflect_timer_.reset(new ActionTimer(1, -1));
}

PlayerComponent::~PlayerComponent() {

}

void PlayerComponent::TokamakFieldUpdate(float dt) {
    tokamak_timer_->Update(dt);
    if(tokamak_timer_->IsActive()) {
        set_hit_point(hit_point() + 0.3);

        float time = tokamak_timer_->GetTime();

        if(time < 0.2f) {
            CCNode *sprite = static_cast<NodeDrawableComponent*>(obj()->drawable_comp())->node();
            static_cast<CCSprite*>(sprite)->setColor(ccc3(255, 255-time*500, 255-time*500));
        }
        else if(time > 4.8f) {
            CCNode *sprite = static_cast<NodeDrawableComponent*>(obj()->drawable_comp())->node();
            ccColor3B color = static_cast<CCSprite*>(sprite)->getColor();
            static_cast<CCSprite*>(sprite)->setColor(ccc3(255, color.g+dt*500, color.b+dt*500));
        }

        return;
    }
    else if(tokamak_timer_->IsInactive()) {
        EndTokamakField();
    }
}

bool PlayerComponent::IsAvailableTokamak() {
    return tokamak_timer_->IsAvailable();
}

void PlayerComponent::PowerShieldUpdate(float dt) {
    power_shield_timer_->Update(dt);
    if(power_shield_timer_->IsInactive()) {
        EndPowerShield();
    }
}

bool PlayerComponent::IsAvailablePowerShield() {
    return power_shield_timer_->IsAvailable();
}

void PlayerComponent::ReflectShieldUpdate(float dt) {
    reflect_timer_->Update(dt);
    if(reflect_timer_->IsAvailable()) {
        reflect_timer_->Action();
    }
    if(reflect_timer_->IsActive() && IsMoved()) {
        reflect_timer_->SetInactive();
    }
}


void PlayerComponent::Update(float dt) {
    CharacterComponent::Update(dt);

    if(!obj()->IsEnabled()) {
        return;
    }

    TokamakFieldUpdate(dt);
    PowerShieldUpdate(dt);
    ReflectShieldUpdate(dt);

    //이 부분을 레이어 관련으로 묶자
    //반사 쉴드 그리기
    //크기는 어떻게 할까
    obj()->world()->shield_layer->set_player_reflect_state(reflect_timer_->IsActive());

    //체력을 info_layer에 넘겨줌
    obj()->world()->game_info_layer->set_player_hit_point(hit_point());

    //조금씩 체력 상승
    set_hit_point(hit_point() + 0.2);

    //파티클 그리기 요청
    b2Vec2 pos = obj()->phy_comp()->main_body()->GetPosition();
    obj()->world()->aura_layer->RequestRenderPlayerAura(obj()->id(), Unit::ToUnitFromMeter(pos));
}

void PlayerComponent::InitMsgHandler() {
    CharacterComponent::InitMsgHandler();
    RegisterMsgFunc(this, &PlayerComponent::OnRequestPlayerPositionMessage);
    RegisterMsgFunc(this, &PlayerComponent::OnRequestRecoveryMessage);
    RegisterMsgFunc(this, &PlayerComponent::OnCollidePlaneMessage);
    RegisterMsgFunc(this, &PlayerComponent::OnIsEnemyMessage);
}

void PlayerComponent::AfterDestroy() {
    obj()->world()->aura_layer->StopRenderAura(obj()->id());
    obj()->world()->shield_layer->StopRenderTokamakField(obj()->id());
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
    if(reflect_timer_->IsActive()) {
        GameObject *bullet = msg->bullet;
        if(static_cast<BulletComponent*>(bullet->logic_comp())->from_enemy() == is_enemy())
            return;

        b2Body *bullet_body = bullet->phy_comp()->main_body();

        //TODO
        //총돌한 위치에서의 충돌 박스 면의 각도를 가져올 수 있나?
        // - world manifold 이용해서 충돌점과 normal vector 가져올 수 있음
        b2Body *player_body = obj()->phy_comp()->main_body();
        b2Vec2 player_pos = player_body->GetPosition();
        b2Vec2 bullet_dir_vec = bullet_body->GetLinearVelocity();
        //일반적인 면에 대한 반사 로직
        /*
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
        */
        
        //새로 짠 로직 (적당히 반사 해줌)
        b2PolygonShape *body_shape = static_cast<b2PolygonShape*>(player_body->GetFixtureList()->GetShape());
        b2Vec2 left_bottom = body_shape->GetVertex(0);
        b2Vec2 right_top = body_shape->GetVertex(2);
        float half_width = glm::abs((right_top.x - left_bottom.x) / 2.0f);
        
        float reflect_angle = 0;
        float collide_pos = msg->manifold.points[1].x;
        //왼쪽
        //CCLOG("%f %f %f", collide_pos, player_pos.x, half_width);
        if(collide_pos < player_pos.x - half_width/4) {
            reflect_angle = M_PI_2 + kmDegreesToRadians(30) * (player_pos.x - collide_pos) / (half_width);
        }
        //오른쪽
        else if(collide_pos > player_pos.x + half_width/4) {
            reflect_angle = M_PI_2 - kmDegreesToRadians(30) * (collide_pos - player_pos.x) / (half_width);
        }
        //가운데
        else {
            reflect_angle = M_PI_2;
        }
        static_cast<BulletComponent*>(bullet->logic_comp())->set_from_enemy(false);
        bullet_body->SetTransform(bullet_body->GetPosition(), reflect_angle);

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

    b2Body *body = obj()->phy_comp()->main_body();
    body->SetTransform(msg->prev_pos, body->GetAngle());
    body->SetLinearVelocity(b2Vec2_zero);
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

        b2Body *player_body = obj()->phy_comp()->main_body();
        b2Vec2 player_velocity = player_body->GetLinearVelocity();
        b2Body *counter_body = msg->counter_obj->phy_comp()->main_body();
        b2Vec2 pos_diff = counter_body->GetPosition() - player_body->GetPosition();
        pos_diff.Normalize();
        player_velocity.Normalize();
        b2Vec2 force_vec = pos_diff + player_velocity;
        force_vec.Normalize();
        force_vec *= 2000;
        counter_body->ApplyForceToCenter(force_vec);

        //플레이어도 밀려야 함
        player_body->SetLinearVelocity(b2Vec2_zero);
        b2Vec2 force = force_vec;
        force *= (-10);
        player_body->ApplyForceToCenter(force);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/collide.mp3");
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

#ifdef _REFACTORING
    if(tokamak_timer_->IsAvailable()) {
        set_unbeatable(true);
        tokamak_timer_->Action();
    }
#else

#endif
}

void PlayerComponent::EndTokamakField() {
    CCNode *sprite = static_cast<NodeDrawableComponent*>(obj()->drawable_comp())->node();
    static_cast<CCSprite*>(sprite)->setColor(ccc3(255, 255, 255));
    set_unbeatable(false);
}

void PlayerComponent::UsePowerShield() {
    if(power_shield_timer_->IsAvailable()) {
        CreateShieldMessage msg = CreateShieldMessage::Create(false);
        obj()->world()->OnMessage(&msg);
        power_shield_timer_->Action();
    }
}

void PlayerComponent::EndPowerShield() {

}