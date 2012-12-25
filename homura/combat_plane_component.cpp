// Ŭnicode please
#include "stdafx.h"
#include "combat_plane_component.h"
#include "game_object.h"
#include "game_world.h"
#include "phy_component.h"
#include "game_object_factory.h"

#include "sora/unit.h"

#include <random>

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
    
    //일단 편대로 움직임 제어를 넘긴다.
    //AIMove(dt);
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
    RequestPhyBodyInfoMessage msg = RequestPhyBodyInfoMessage::Create(&body_info);
    obj()->OnMessage(&msg);

    if(!msg.is_ret)
        return;
        
    TestBulletObjectHeader header;

    float look_angle = body_info.angle_rad;
    //각도로부터 벡터 만들기
    //cocos2dx랑 box2d 각도 기준점이 다름.
    header.angle_rad = body_info.angle_rad;
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

void CombatPlaneComponent::AfterDestroy() {
}

void CombatPlaneComponent::AIMove( float dt )
{
    static unsigned int temp_rnd_factor = 0;
    std::default_random_engine rand_engine((unsigned int)time(0) + temp_rnd_factor++);
    int dir_x = rand_engine() % 2 == 0? -1 : 1;
    dir_x *= rand_engine() % 500;
    rand_engine.seed((unsigned int)time(0) + temp_rnd_factor++);
    int dir_y = rand_engine() % 2 == 0? -1 : 1;
    dir_y *= rand_engine() % 500;
    MoveMessage msg = MoveMessage::Create(b2Vec2(dir_x * dt, dir_y * dt));
    obj()->OnMessage(&msg);
}
