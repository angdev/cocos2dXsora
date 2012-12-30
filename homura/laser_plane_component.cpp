// Ŭnicode please
#include "stdafx.h"
#include "laser_plane_component.h"

#include "game_object.h"
#include "game_world.h"
#include "phy_component.h"

#include "sora/unit.h"

LaserPlaneComponent::LaserPlaneComponent(GameObject *obj, cocos2d::CCNode *layer)
    : CharacterComponent(obj, layer) {
        ray_cast_callback_ = std::move(std::unique_ptr<RayCastCallback>(new RayCastCallback(this)));
}

LaserPlaneComponent::~LaserPlaneComponent() {

};

void LaserPlaneComponent::Update(float dt) {
    Attack();
}

void LaserPlaneComponent::InitMsgHandler() {

}

void LaserPlaneComponent::Attack() {
    //보이지 않는 레이저를 쏩니다?
    
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
    dir_vec *= sora::Unit::ToMeterFromUnit(800);
    dir_vec += obj_pos_vec;
    
    ray_cast_callback_->Reset();
    obj()->world()->b2_world()->RayCast(ray_cast_callback_.get(), obj_pos_vec, dir_vec);
    ray_cast_callback_->AfterCallback();
}

void LaserPlaneComponent::AfterDestroy() {

}


void LaserPlaneComponent::RayCastCallback::Reset() {
    nearest_fraction_ = 10;
    is_hit_ = false;
}

float32 LaserPlaneComponent::RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
    
    if(nearest_fraction_ > fraction) {
        nearest_fraction_ = fraction;
        this->point = point;
        this->fixture = fixture;
        is_hit_ = true;
    }

    return 1;
}

void LaserPlaneComponent::RayCastCallback::AfterCallback() {
    if(!is_hit_)
        return;

    GameObject *counter_obj = static_cast<GameObject*>(fixture->GetBody()->GetUserData());

    IsEnemyMessage enemy_msg = IsEnemyMessage::Create();
    counter_obj->OnMessage(&enemy_msg);

    if(enemy_msg.is_enemy == owner_comp_->is_enemy())
        return;

    DamageObjectMessage damage_msg = DamageObjectMessage::Create(0.1f);
    counter_obj->OnMessage(&damage_msg);
}
