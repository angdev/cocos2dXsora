// Ŭnicode please
#include "stdafx.h"
#include "combat_plane_component.h"
#include "game_object.h"
#include "game_world.h"
#include "phy_component.h"
#include "game_object_factory.h"

#include "sora/unit.h"

using namespace sora;

CombatPlaneComponent::CombatPlaneComponent(GameObject *obj, cocos2d::CCNode *layer)
    : CharacterComponent(obj, layer), attack_timer_(0), attack_cool_down_(0.3f) {

}

CombatPlaneComponent::~CombatPlaneComponent() {

}

void CombatPlaneComponent::Update(float dt) {
    //TODO
    CharacterComponent::Update(dt);
    Attack(dt);
}

void CombatPlaneComponent::InitMsgHandler() {
    CharacterComponent::InitMsgHandler();
    //TODO
}

void CombatPlaneComponent::Attack(float dt) {

    if(attack_timer_ < attack_cool_down_) {
        attack_timer_ += dt;
        return;
    }

    attack_timer_ = 0;

    PhyBodyInfo body_info;
    GetPhyBodyInfoMessage msg = GetPhyBodyInfoMessage::Create(&body_info);
    obj()->OnMessage(&msg);

    if(!msg.is_ret)
        return;
        
    TestBulletObjectHeader header;

    float look_angle = body_info.angle_rad;
    //각도로부터 벡터 만들기
    //cocos2dx랑 box2d 각도 기준점이 다름.
    header.angle_rad = body_info.angle_rad + M_PI/2;
    //cocos2d::CCLog("%f %f %f", look_angle, look_vector.x, look_vector.y);
    //방향과 속력은 분리해야 함 - 총알도 돌려야 함.
    header.speed = 30;
    header.x = Unit::ToUnitFromMeter(body_info.x);
    header.y = Unit::ToUnitFromMeter(body_info.y);

    header.damage = 10;
    header.from_enemy = is_enemy();  //IsEnemy 필요없을지도?
    header.sprite_name = "";
    
    GameObjectFactory factory(obj()->world());
    obj()->world()->AddObject(factory.Create(header, layer()));
}

void CombatPlaneComponent::Destroy() {
    GameWorld *world = obj()->world();
    world->RequestRemoveObject(world->FindObject(obj()->id()));
}