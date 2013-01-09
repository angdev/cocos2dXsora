// Ŭnicode please
#include "stdafx.h"
#include "game_object_factory.h"

#include "game_world.h"
#include "game_object.h"
#include "drawable_component.h"
#include "phy_component.h"
#include "combat_plane_component.h"
#include "laser_plane_component.h"
#include "player_component.h"
#include "bullet_component.h"
#include "shield_component.h"
#include "formation_component.h"
#include "chain_component.h"
#include "ally_ai_component.h"
#include "enemy_ai_component.h"
#include "laser_layer.h"
#include "game_trigger_component.h"
#include "game_trigger_handler.h"
#include "character_fsm.h"
#include "sora/unit.h"

#include "data_define.h"

#if SR_USE_PCH == 0
#include <Box2D/Box2D.h>
#endif

USING_NS_CC;
using namespace sora;
using namespace std;

GameObject *GameObjectFactory::Create( const PlayerObjectHeader &header, cocos2d::CCNode *parent ) {
    //테스트용으로 조작할 걸 만드는게 목적
    GameObject *obj = new GameObject(world_);

    glm::vec2 obj_pos(header.x, header.y);
    b2Body *body = CreateCollisionCircle(obj_pos, Unit::ToUnitFromMeter(2.0f));

    body->SetTransform(body->GetPosition(), header.angle_rad);
    
    CCSprite *sprite = CCSprite::create("dead_star.png");
    sprite->setScale(0.2f);

    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    PlayerComponent *logic = new PlayerComponent(obj, parent);
    logic->set_max_hit_point(header.hit_point);
    logic->set_hit_point(header.hit_point);

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);

    //world_->AddObject(obj, obj->Type());

    return obj;
}

GameObject *GameObjectFactory::Create( const BulletObjectHeader &header, cocos2d::CCNode *parent ) {
    //충돌 박스를 스프라이트로부터 끌어낸다. (함수 만들자)
    //스프라이트는 헤더에 있는 것과 관계없이 그냥 생성
    CCSprite *sprite = CCSprite::create("circle_bullet.png");
    sprite->setScale(0.1f);
    sprite->setPosition(ccp(header.x, header.y));
    CCRect sprite_box = sprite->boundingBox();

    b2Body *body = CreateCollisionCircle(glm::vec2(header.x, header.y),
        sprite_box.size.width / 2.0f);
    body->SetBullet(true);
    body->SetTransform(body->GetPosition(), header.angle_rad);

    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    BulletComponent *logic = new BulletComponent(obj);
    logic->set_damage(header.damage);
    logic->set_from_enemy(header.from_enemy);
    logic->set_speed(header.speed);

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);

    //world_->AddObject(obj, obj->Type());

    return obj;
}

/************************************************************************/
/* 마음대로 설정 집어넣어서 만들 수 있는 기체들                         */
/************************************************************************/

GameObject *GameObjectFactory::Create( const CombatPlaneObjectHeader &header, cocos2d::CCNode *parent ) {
    glm::vec2 obj_pos(header.x, header.y);
    
    b2Body *body = CreateCollisionCircle(obj_pos, Unit::ToUnitFromMeter(1.0f));
    //바라보는 방향 등 생성을 적절히 해야함
    body->SetTransform(body->GetPosition(), header.angle);

    CCSprite *sprite;
    if(header.is_enemy) {
        sprite = CCSprite::create("dead_star.png");
        sprite->setScale(0.2f);
    }
    else {
        sprite = CCSprite::create("ally_plane.png");
        sprite->setScale(0.8f);
    }

    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    CombatPlaneComponent *logic = new CombatPlaneComponent(obj, parent);

    //temp
    //객체 마다 header로 걍 초기화하는거 넣을 것.
    logic->set_max_hit_point(header.hit_point);
    //maxHP 받는 부분 넣어야하나. 비율은 적절히 랜덤으로 조정하면 될 것 같은데
    logic->set_hit_point(header.is_fall? header.hit_point * 0.3f : header.hit_point);
    
    //AI!
    AIComponent *ai;
    if(header.is_enemy) {
        ai = new EnemyAIComponent(obj);
        ai->set_start_position(b2Vec2(Unit::ToMeterFromUnit(header.x), Unit::ToMeterFromUnit(header.y - 300)));
    }
     else {
        ai = new AllyAIComponent(obj);
        ai->set_state(header.is_fall? kAllyFallState : kAllyNormalState);
     }

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);
    obj->set_ai_comp(ai);

    //world_->AddObject(obj, obj->Type());

    return obj;
}

GameObject * GameObjectFactory::Create(const LaserPlaneObjectHeader &header, cocos2d::CCNode *parent) {
    //일단 복붙
    //이후에 분리 예정

    glm::vec2 obj_pos(header.x, header.y);

    b2Body *body = CreateCollisionCircle(obj_pos, Unit::ToUnitFromMeter(1.0f));
    //바라보는 방향 등 생성을 적절히 해야함
    body->SetTransform(body->GetPosition(), header.angle);

    CCSprite *sprite;
    if(header.is_enemy) {
        sprite = CCSprite::create("dead_star.png");
        sprite->setScale(0.2f);
    }
    else {
        sprite = CCSprite::create("ally_plane.png");
        sprite->setScale(0.8f);
    }

    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    LaserPlaneComponent *logic = new LaserPlaneComponent(obj, parent);

    //temp
    //객체 마다 header로 걍 초기화하는거 넣을 것.
    logic->set_max_hit_point(header.hit_point);
    //maxHP 받는 부분 넣어야하나. 비율은 적절히 랜덤으로 조정하면 될 것 같은데
    logic->set_hit_point(header.is_fall? header.hit_point * 0.3f : header.hit_point);

    //AI!
    AIComponent *ai;
    if(header.is_enemy) {
        ai = new EnemyAIComponent(obj);
        ai->set_start_position(b2Vec2(Unit::ToMeterFromUnit(header.x), Unit::ToMeterFromUnit(header.y - 300)));
    }
    else {
        ai = new AllyAIComponent(obj);
        ai->set_state(header.is_fall? kAllyFallState : kAllyNormalState);
    }

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);
    obj->set_ai_comp(ai);

    //world_->AddObject(obj, obj->Type());

    return obj;
}

/************************************************************************/
/* Preset 있는 기체들 (일단 하드코딩)                                   */
/************************************************************************/

//이건 아군기체 전용

GameObject * GameObjectFactory::Create(const AllyPlaneObjectHeader &header, cocos2d::CCNode *parent) {
    glm::vec2 obj_pos(header.x, header.y);

    b2Body *body = CreateCollisionCircle(obj_pos, Unit::ToUnitFromMeter(1.0f));
    //바라보는 방향 등 생성을 적절히 해야함
    body->SetTransform(body->GetPosition(), M_PI_2);

    CCSprite *sprite = CCSprite::create("ally_plane.png");
    sprite->setScale(0.8f);

    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    LaserPlaneComponent *logic = new LaserPlaneComponent(obj, parent);

    //temp
    //객체 마다 header로 걍 초기화하는거 넣을 것.

    logic->set_max_hit_point(100);
    //maxHP 받는 부분 넣어야하나. 비율은 적절히 랜덤으로 조정하면 될 것 같은데
    logic->set_hit_point(header.is_fall? 30 : 100);

    //AI!
    AIComponent *ai = new AllyAIComponent(obj);
    ai->set_state(header.is_fall? kAllyFallState : kAllyAppearState);
    ai->set_start_position(b2Vec2(Unit::ToMeterFromUnit(header.x), Unit::ToMeterFromUnit(header.y + 300)));

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);
    obj->set_ai_comp(ai);

    //world_->AddObject(obj, obj->Type());

    return obj;
}


//이건 적기체 전용


GameObject * GameObjectFactory::Create(const EnemyCombatPlaneObjectHeader &header, cocos2d::CCNode *parent) {
    glm::vec2 obj_pos(header.x, header.y);

    b2Body *body = CreateCollisionCircle(obj_pos, Unit::ToUnitFromMeter(1.0f));
    //바라보는 방향 등 생성을 적절히 해야함
    body->SetTransform(body->GetPosition(), -M_PI_2);

    CCSprite *sprite = CCSprite::create("suicide_bomber.png");
    sprite->setScale(0.8f);

    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    CombatPlaneComponent *logic = new CombatPlaneComponent(obj, parent);

    //temp
    //객체 마다 header로 걍 초기화하는거 넣을 것.

    logic->set_max_hit_point(30);
    //maxHP 받는 부분 넣어야하나. 비율은 적절히 랜덤으로 조정하면 될 것 같은데
    logic->set_hit_point(30);
    logic->set_bullet_damage(5);
    logic->set_available_suicide(true);

    //AI!
    AIComponent *ai = new EnemyAIComponent(obj);
    ai->set_start_position(b2Vec2(Unit::ToMeterFromUnit(header.x), Unit::ToMeterFromUnit(header.y - 300)));

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);
    obj->set_ai_comp(ai);

    //world_->AddObject(obj, obj->Type());

    return obj;
}


GameObject *GameObjectFactory::Create(const CruiserPlaneObjectHeader &header, cocos2d::CCNode *parent) {
    glm::vec2 obj_pos(header.x, header.y);

    b2Body *body = CreateCollisionCircle(obj_pos, Unit::ToUnitFromMeter(1.0f));
    //바라보는 방향 등 생성을 적절히 해야함
    body->SetTransform(body->GetPosition(), -M_PI_2);

    CCSprite *sprite = CCSprite::create("cruiser.png");
    sprite->setScale(0.8f);

    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    CombatPlaneComponent *logic = new CombatPlaneComponent(obj, parent);

    //temp
    //객체 마다 header로 걍 초기화하는거 넣을 것.

    logic->set_max_hit_point(150);
    //maxHP 받는 부분 넣어야하나. 비율은 적절히 랜덤으로 조정하면 될 것 같은데
    logic->set_hit_point(150);
    logic->set_bullet_damage(20);
    logic->set_available_suicide(false);

    //AI!
    AIComponent *ai = new EnemyAIComponent(obj);
    ai->set_start_position(b2Vec2(Unit::ToMeterFromUnit(header.x), Unit::ToMeterFromUnit(header.y - 300)));

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);
    obj->set_ai_comp(ai);

    //world_->AddObject(obj, obj->Type());

    return obj;
}


GameObject *GameObjectFactory::Create(const DeadstarPlaneObjectHeader &header, cocos2d::CCNode *parent) {
    glm::vec2 obj_pos(header.x, header.y);

    b2Body *body = CreateCollisionCircle(obj_pos, Unit::ToUnitFromMeter(1.0f));
    //바라보는 방향 등 생성을 적절히 해야함
    body->SetTransform(body->GetPosition(), -M_PI_2);

    CCSprite *sprite = CCSprite::create("dead_star.png");
    sprite->setScale(0.8f);

    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    LaserPlaneComponent *logic = new LaserPlaneComponent(obj, parent);

    //temp
    //객체 마다 header로 걍 초기화하는거 넣을 것.

    logic->set_max_hit_point(20);
    //maxHP 받는 부분 넣어야하나. 비율은 적절히 랜덤으로 조정하면 될 것 같은데
    logic->set_hit_point(20);

    //AI!
    AIComponent *ai = new EnemyAIComponent(obj);
    ai->set_start_position(b2Vec2(Unit::ToMeterFromUnit(header.x), Unit::ToMeterFromUnit(header.y - 300)));

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);
    obj->set_ai_comp(ai);

    //world_->AddObject(obj, obj->Type());

    return obj;
}



GameObject *GameObjectFactory::Create(const ShieldHeader &header, cocos2d::CCNode *parent) {

    glm::vec2 body_pos(header.x, header.y);
    b2Body *body = CreateCollisionCircle(body_pos, 100);
    
    GameObject *obj = new GameObject(world_);
    //Drawable은 아직 없음
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    ShieldComponent *logic = new ShieldComponent(obj);
    logic->set_max_hit_point(header.hit_point);
    logic->set_hit_point(header.hit_point);
    logic->set_duration(header.duration);
    logic->set_target_id(header.target_id);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);

    return obj;
}


GameObject * GameObjectFactory::Create(const ChainHeader &header) {
    GameObject *obj = new GameObject(world_);
    ChainComponent *logic = new ChainComponent(obj);
    logic->set_master_id(header.master_id);
    logic->set_slave_id(header.slave_id);
    obj->set_logic_comp(logic);

    return obj;
}


//헤더는 의미없음. 그냥 넣어둔거.
GameObject *GameObjectFactory::Create(const ObjectHeader &header, cocos2d::CCNode *parent) {
    glm::vec2 obj_pos(header.x, header.y);
    b2Body *body = CreateCollisionCircle(obj_pos, Unit::ToUnitFromMeter(1.0f));

    // 적절히 스프라이트 생성하기
    CCSprite *sprite = CCSprite::create("dead_star.png");
    sprite->setScale(0.2f);

    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    LogicComponent *logic = new CombatPlaneComponent(obj, parent);
    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);

    //아무것도 아닌거로 일단 설정. 이것은 그냥 그자리에 적절히 존재할뿐이라는 의미
    //world_->AddObject(obj, obj->Type());

    return obj;
}


GameObject *GameObjectFactory::Create(const FormationHeader &header) {
    GameObject *obj = new GameObject(world_);
    LogicComponent *logic = new FormationComponent(obj);
    obj->set_logic_comp(logic);

    return obj;
}


GameObject *GameObjectFactory::Create(const GameTriggerObjectHeader &header, TriggerID trigger_id, NextTriggers *next_triggers, 
                                       GameTriggerHandlerPtr game_trigger_handler_) {
    GameObject *obj = new GameObject(world_);
    game_trigger_handler_->set_next_triggers(next_triggers);
    game_trigger_handler_->set_trigger_id(trigger_id);
    GameTriggerComponent *logic = new GameTriggerComponent(obj, game_trigger_handler_);
    obj->set_logic_comp(logic);

    return obj;
}


b2Body * GameObjectFactory::CreateCollisionCircle(const glm::vec2 &ut_pos, float radius)
{
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    b2Vec2 mt_pos = Unit::ToMeterFromUnit(ut_pos);
    bodyDef.position = mt_pos;

    b2World *b2_world = world_->b2_world();
    b2Body *body = b2_world->CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2CircleShape circleBox;
    //일단 이렇게 떼운다 =ㅅ=;;
    circleBox.m_radius = Unit::ToMeterFromUnit(radius);
    //크기로 b2Body 만들어주는거 있어도 될듯.
    //dynamicBox.SetAsBox(Unit::ToMeterFromUnit(half_width_px), Unit::ToMeterFromUnit(half_height_px));//These are mid points for our 1m box

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    // 충돌 체크용으로만 사용.
    //fixtureDef.isSensor = true;
    fixtureDef.shape = &circleBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

    return body;
}