// Ŭnicode please
#include "stdafx.h"
#include "combat_plane_component.h"
#include "game_object.h"
#include "game_world.h"
#include "phy_component.h"
#include "game_object_factory.h"
#include "character_fsm.h"
#include "character_normal_state.h"
#include "enemy_ai_component.h"

#include "sora/unit.h"

#include <random>

using namespace sora;
USING_NS_CC;

CombatPlaneComponent::CombatPlaneComponent(GameObject *obj, cocos2d::CCNode *layer)
    : CharacterComponent(obj, layer), attack_timer_(0), attack_cool_down_(0.3f), bullet_damage_(1.0f), suicide_flag_(false), available_suicide_(false) {
        //상태 초기화 따로 빼야할듯?
//        fsm()->InsertState(CharacterStatePtr(new CharacterNormalState(fsm())));
}

CombatPlaneComponent::~CombatPlaneComponent() {

}

void CombatPlaneComponent::Update(float dt) {
    //TODO
    CharacterComponent::Update(dt);

    if(!obj()->IsEnabled()) {
        return;
    }

    if(suicide_flag_) {
        //이동만 슈웅 한다
        MoveByMessage move_msg = MoveByMessage::Create(Unit::ToUnitFromMeter(suicide_vec), 0.2f);
        CCLOG("move %f %f", suicide_vec.x, suicide_vec.y);
        obj()->OnMessage(&move_msg);
        return;
    }

    if(attack_timer_ < attack_cool_down_) {
        attack_timer_ += dt;
        return;
    }

    //일단 편대로 움직임 제어를 넘긴다.
    //AIMove(dt);
}

void CombatPlaneComponent::InitMsgHandler() {
    CharacterComponent::InitMsgHandler();
    RegisterMsgFunc(this, &CombatPlaneComponent::OnAttackMessage);
    //TODO
}

void CombatPlaneComponent::Attack() {

    if(!(attack_timer_ > attack_cool_down_)) {
        return;
    }

    attack_timer_ = 0;

    PhyBodyInfo body_info;
    RequestPhyBodyInfoMessage msg = RequestPhyBodyInfoMessage::Create(&body_info);
    obj()->OnMessage(&msg);

    if(!msg.is_ret)
        return;
        
    BulletObjectHeader header;

    float look_angle = body_info.angle_rad;
    //각도로부터 벡터 만들기
    //cocos2dx랑 box2d 각도 기준점이 다름.
    header.angle_rad = body_info.angle_rad;
    //방향과 속력은 분리해야 함 - 총알도 돌려야 함.
    header.speed = 60;
    header.x = Unit::ToUnitFromMeter(body_info.x);
    header.y = Unit::ToUnitFromMeter(body_info.y);

    header.damage = bullet_damage_;
    header.from_enemy = is_enemy();  //IsEnemy 필요없을지도?
    header.sprite_name = "";
    
    GameObjectFactory factory(obj()->world());
    obj()->world()->AddObject(factory.Create(header, layer()));
}

void CombatPlaneComponent::AfterDestroy() {
    //파티클을 터뜨리자
    CCParticleSystem *emitter = new CCParticleSystemQuad();
    //create 함수를 쓰니까 죽음
    //왜?
    emitter->initWithFile("particles/ExplodingRing.plist");
    assert(emitter != NULL);
    //아직 안 없어져있으니 괜찮음
    PhyBodyInfo body_info;
    RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&body_info);
    obj()->OnMessage(&body_info_msg);

    if(!body_info_msg.is_ret)
        return;

    emitter->setPosition(Unit::ToUnitFromMeter(body_info.x), Unit::ToUnitFromMeter(body_info.y));
    layer()->addChild(emitter, 10);
    emitter->autorelease();
}

void CombatPlaneComponent::AIMove( float dt )
{
    /*
    static unsigned int temp_rnd_factor = 0;
    std::default_random_engine rand_engine((unsigned int)time(0) + temp_rnd_factor++);
    int dir_x = rand_engine() % 2 == 0? -1 : 1;
    dir_x *= rand_engine() % 500;
    rand_engine.seed((unsigned int)time(0) + temp_rnd_factor++);
    int dir_y = rand_engine() % 2 == 0? -1 : 1;
    dir_y *= rand_engine() % 500;
    MoveToMessage msg = MoveToMessage::Create(b2Vec2(dir_x * dt, dir_y * dt));
    obj()->OnMessage(&msg);
    */
}

void CombatPlaneComponent::OnAttackMessage(AttackMessage *msg) {
    //TODO
    if(available_suicide_ && !suicide_flag_ && hit_point() < max_hit_point() * 0.4f) {
        GameObjectPtr target = obj()->world()->FindObject(msg->target_id);
        if(target == NULL) {
            Attack();
            return;
        }
        b2Vec2 target_pos = target->phy_comp()->main_body()->GetPosition();
        SuicideAttack(target_pos);
    } 
    else if(!suicide_flag_) {
        Attack();
    }
}

void CombatPlaneComponent::SuicideAttack(const b2Vec2 &object_pos) {
    if(!suicide_flag_) {
        suicide_flag_ = true;
        
        b2Vec2 current_pos = obj()->phy_comp()->main_body()->GetPosition();
        b2Vec2 diff_pos = object_pos - current_pos;
        diff_pos *= 0.2f;

        suicide_vec = diff_pos;
        
        obj()->ai_comp()->set_state(kEnemyOutOfControlState);
    }
}
