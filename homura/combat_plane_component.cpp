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

    TestBulletObjectHeader header;
    header.dir_x = 0;
    header.dir_y = 4;

    //일단 직접 접근.
    //메시지로 리턴값을 보낼 수 있을까?
    //처리 방법 1 : 여기서 다른 component가 필요하지 않은 
    //정보를 모두 기록 -> 그리고 다른 component로 기록을 요청(메시지 만들어야함)
    //-> 기록이 끝나면 총알 생성
    b2Vec2 pos = obj()->phy_comp()->main_body()->GetPosition();
    header.x = Unit::ToUnitFromMeter(pos.x);
    header.y = Unit::ToUnitFromMeter(pos.y);

    header.damage = 10;
    header.from_id = obj()->id();
    header.sprite_name = "";
    
    GameObjectFactory factory(obj()->world());
    factory.CreateDemoBullet(header, layer());
}