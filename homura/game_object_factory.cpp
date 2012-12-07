// Ŭnicode please
#include "stdafx.h"
#include "game_object_factory.h"

#include "game_world.h"
#include "game_object.h"
#include "drawable_component.h"
#include "phy_component.h"
#include "combat_plane_component.h"
#include "player_component.h"
#include "bullet_component.h"
#include "game_event_component.h"
#include "sora/unit.h"

#include "data_define.h"

#if SR_USE_PCH == 0
#include <Box2D/Box2D.h>
#endif

USING_NS_CC;
using namespace sora;
using namespace std;

GameObject *GameObjectFactory::Create( const TestPlayerObjectHeader &header, cocos2d::CCNode *parent ) {
    //테스트용으로 조작할 걸 만드는게 목적
    GameObject *obj = new GameObject(world_);

    glm::vec2 obj_pos(header.x, header.y);
    b2Body *body = CreateCollisionBox(obj_pos, Unit::ToUnitFromMeter(2.0f), Unit::ToUnitFromMeter(2.0f));

    body->SetTransform(body->GetPosition(), header.angle_rad);

    CCSprite *sprite = CCSprite::create("kyoko_icon.png");
    sprite->setScale(0.2f);

    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    PlayerComponent *logic = new PlayerComponent(obj, parent);
    logic->set_hit_point(header.hit_point);

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);

    //world_->AddObject(obj, obj->Type());

    return obj;
}

GameObject *GameObjectFactory::Create( const TestBulletObjectHeader &header, cocos2d::CCNode *parent ) {
    //충돌 박스를 스프라이트로부터 끌어낸다. (함수 만들자)
    //스프라이트는 헤더에 있는 것과 관계없이 그냥 생성
    CCSprite *sprite = CCSprite::create("circle_bullet.png");
    sprite->setScale(0.1f);
    sprite->setPosition(ccp(header.x, header.y));
    CCRect sprite_box = sprite->boundingBox();

    b2Body *body = CreateCollisionBox(glm::vec2(header.x, header.y),
        sprite_box.size.width / 2.0f, sprite_box.size.height / 2.0f);
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

GameObject *GameObjectFactory::Create( const TestCombatPlaneObjectHeader &header, cocos2d::CCNode *parent ) {
    glm::vec2 obj_pos(header.x, header.y);
    
    b2Body *body = CreateCollisionBox(obj_pos, Unit::ToUnitFromMeter(1.0f), Unit::ToUnitFromMeter(1.0f));
    //바라보는 방향 등 생성을 적절히 해야함
    body->SetTransform(body->GetPosition(), header.angle);

    CCSprite *sprite = CCSprite::create("kyoko_icon.png");
    sprite->setScale(0.1f);

    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    CombatPlaneComponent *logic = new CombatPlaneComponent(obj, parent);

    //temp
    //객체 마다 header로 걍 초기화하는거 넣을 것.
    logic->set_hit_point(header.hit_point);
    logic->set_is_enemy(header.is_enemy);

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);

    //world_->AddObject(obj, obj->Type());

    return obj;
}

//헤더는 의미없음. 그냥 넣어둔거.
GameObject * GameObjectFactory::Create( const DemoObjectHeader &header, cocos2d::CCNode *parent ) {
    glm::vec2 obj_pos(header.x, header.y);
    b2Body *body = CreateCollisionBox(obj_pos, Unit::ToUnitFromMeter(1.0f), Unit::ToUnitFromMeter(1.0f));

    // 적절히 스프라이트 생성하기
    CCSprite *sprite = CCSprite::create("kyoko_icon.png");
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

GameObject * GameObjectFactory::Create(const GameEventObjectHeader &header, EventID event_id, NextEventsPtr next_events, 
                                       GameEventHandlerPtr game_event_handler_) {
    GameObject *obj = new GameObject(world_);
    GameEventComponent *logic = new GameEventComponent(obj, event_id, next_events, game_event_handler_);
    obj->set_logic_comp(logic);

    return obj;
}

b2Body *GameObjectFactory::CreateCollisionBox(const glm::vec2 &ut_pos, float half_width_px, float half_height_px) {
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    b2Vec2 mt_pos = Unit::ToMeterFromUnit(ut_pos);
    bodyDef.position = mt_pos;

    b2World *b2_world = world_->b2_world();
    b2Body *body = b2_world->CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    //총알 크기만한 박스 생성
    //크기로 b2Body 만들어주는거 있어도 될듯.
    dynamicBox.SetAsBox(Unit::ToMeterFromUnit(half_width_px), Unit::ToMeterFromUnit(half_height_px));//These are mid points for our 1m box

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    // 충돌 체크용으로만 사용.
    fixtureDef.isSensor = true;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

    return body;
}