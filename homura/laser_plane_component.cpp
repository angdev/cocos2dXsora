// Ŭnicode please
#include "stdafx.h"
#include "laser_plane_component.h"

#include "game_object.h"
#include "game_world.h"
#include "phy_component.h"

#include "action_timer.h"

#include "sora/unit.h"

USING_NS_CC;
using namespace sora;

LaserPlaneComponent::LaserPlaneComponent(GameObject *obj, cocos2d::CCNode *layer)
    : 
    CharacterComponent(obj, layer),
    laser_damage_(50.0f) {
        attack_timer_.reset(new ActionTimer<LaserPlaneComponent>(this, 2.0f, 8.0f));
        attack_timer_->RegisterEndAction(&LaserPlaneComponent::StopAttack);
        ray_cast_callback_ = std::move(std::unique_ptr<RayCastCallback>(new RayCastCallback(this)));
}

LaserPlaneComponent::~LaserPlaneComponent() {

};

void LaserPlaneComponent::UpdateAttackLogic(float dt) {
    attack_timer_->Update(dt);
    /*
    if(attack_timer_->IsInactive()) {
        StopAttack();
    }
    */
    if(attack_timer_->IsActive()) {
        Attack();
    }
}

void LaserPlaneComponent::DerivedUpdate(float dt) {
    
    if(!obj()->IsEnabled()) {
        return;
    }

    UpdateAttackLogic(dt);
}

void LaserPlaneComponent::InitMsgHandler() {
    CharacterComponent::InitMsgHandler();
    RegisterMsgFunc(this, &LaserPlaneComponent::OnAttackMessage);
    RegisterMsgFunc(this, &LaserPlaneComponent::OnMoveByMessage);
    RegisterMsgFunc(this, &LaserPlaneComponent::OnMoveToMessage);
}

void LaserPlaneComponent::Attack() {   
    //body를 얻어와서
    PhyBodyInfo body_info;
    RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&body_info);
    obj()->OnMessage(&body_info_msg);
    
    assert(body_info_msg.is_ret && "body info is not returned (laser comp attack)");
    //걍 전방이겠지만..
    b2Vec2 obj_pos_vec(body_info.x, body_info.y);
    b2Vec2 dir_vec(glm::cos(body_info.angle_rad), glm::sin(body_info.angle_rad));
    //월드 경계까지긴 한데 음.
    //미리 상수로 바꿔놔도 될듯
    dir_vec *= sora::Unit::ToMeterFromUnit(1280);
    dir_vec += obj_pos_vec;
    
    //래핑하는거 만들자?
    ray_cast_callback_->Reset();
    obj()->world()->b2_world()->RayCast(ray_cast_callback_.get(), obj_pos_vec, dir_vec);
    ray_cast_callback_->AfterCallback();
}

void LaserPlaneComponent::AfterDestroy() {
    //파티클을 터뜨리자
    CCParticleSystem *emitter = new CCParticleSystemQuad();
    //create 함수를 쓰니까 죽음
    //왜?
    emitter->initWithFile("particles/ExplodingRing.plist");
    emitter->setTotalParticles(30);
    assert(emitter != NULL);
    //아직 안 없어져있으니 괜찮음
    PhyBodyInfo body_info;
    RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&body_info);
    obj()->OnMessage(&body_info_msg);

    if(!body_info_msg.is_ret)
        return;

    emitter->setPosition(Unit::ToUnitFromMeter(body_info.x), Unit::ToUnitFromMeter(body_info.y));
    layer()->addChild(emitter, 10);
}

void LaserPlaneComponent::OnAttackMessage(AttackMessage *msg) {
    //TODO
    if(obj()->IsEnabled() && attack_timer_->IsAvailable()) {
        //소리 재생
        
        char sound_rand = '0' + std::default_random_engine((unsigned int)time(0))() % 2;
        std::string file_path = "sound/laser";
        file_path += sound_rand;
        file_path += ".mp3";
        laser_sound_id_ = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(file_path.c_str());
        
        attack_timer_->Action();
    }
}

void LaserPlaneComponent::StopAttack() {

    //그리기 끝
    StopRenderLaserMessage stop_msg = StopRenderLaserMessage::Create(obj()->id());
    obj()->world()->OnMessage(&stop_msg);

    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(laser_sound_id_);

    attack_timer_->SetInactive();
}

void LaserPlaneComponent::OnMoveToMessage(MoveToMessage *msg) {
    StopAttack();
}

void LaserPlaneComponent::OnMoveByMessage(MoveByMessage *msg) {
    StopAttack();
}


void LaserPlaneComponent::RayCastCallback::Reset() {
    nearest_fraction_ = 10;
    is_hit_ = false;
}

float32 LaserPlaneComponent::RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
    
    if(nearest_fraction_ > fraction) {
        //적군 기체가 부딪혔는지가 중요함
        GameObject *obj = static_cast<GameObject*>(fixture->GetBody()->GetUserData());
        IsEnemyMessage msg = IsEnemyMessage::Create();
        obj->OnMessage(&msg);

        if(msg.is_ret && owner_comp_->is_enemy() != msg.is_enemy) {
            nearest_fraction_ = fraction;
            this->point = point;
            this->fixture = fixture;
            is_hit_ = true;

        }
    }

    return 1;
}

void LaserPlaneComponent::RayCastCallback::AfterCallback() {
    //현재 위치에서 point 까지 레이저 그리면 됨

    PhyBodyInfo body_info;
    RequestPhyBodyInfoMessage phy_body_msg = RequestPhyBodyInfoMessage::Create(&body_info);
    owner_comp_->obj()->OnMessage(&phy_body_msg);
    glm::vec2 body_pos(Unit::ToUnitFromMeter(body_info.x), Unit::ToUnitFromMeter(body_info.y));

    assert(phy_body_msg.is_ret && "Raycast; req phy body error");

    if(!is_hit_) {
        //안 맞으면 화면 끝까지
        glm::vec2 dir_vec(glm::cos(body_info.angle_rad), glm::sin(body_info.angle_rad));
        dir_vec *= 1280;    //일단 대충 떼움

        RequestRenderLaserMessage render_msg = RequestRenderLaserMessage::Create(owner_comp_->obj()->id(), body_pos, body_pos+dir_vec);
        owner_comp_->obj()->world()->OnMessage(&render_msg);
        return;
    }

    //레이저 그리자
    RequestRenderLaserMessage render_msg = RequestRenderLaserMessage::Create(owner_comp_->obj()->id(), body_pos, Unit::ToUnitFromMeter(point));
    owner_comp_->obj()->world()->OnMessage(&render_msg);


    GameObject *counter_obj = static_cast<GameObject*>(fixture->GetBody()->GetUserData());

    IsEnemyMessage enemy_msg = IsEnemyMessage::Create();
    counter_obj->OnMessage(&enemy_msg);

    if(enemy_msg.is_ret && enemy_msg.is_enemy == owner_comp_->is_enemy())
        return;

    DamageObjectMessage damage_msg = DamageObjectMessage::Create(owner_comp_->laser_damage() * (1/60.0f));
    counter_obj->OnMessage(&damage_msg);
}
