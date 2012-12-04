// Ŭnicode please
#include "stdafx.h"
#include "game_object_factory.h"

#include "game_world.h"
#include "game_object.h"
#include "drawable_component.h"
#include "phy_component.h"
#include "ai_character_component.h"
#include "combat_plane_component.h"
#include "player_component.h"
#include "bullet_component.h"
#include "sora/unit.h"

#include "data_define.h"

#if SR_USE_PCH == 0
#include <Box2D/Box2D.h>
#endif

USING_NS_CC;
using namespace sora;
using namespace std;

GameObject *GameObjectFactory::CreateDemoBullet(const glm::vec2 &ut_pos, cocos2d::CCNode *parent) {
    
    //적당히 스프라이트 시트에서 일단 하나 가져옴.
    CCSprite *sprite = CCSprite::create("circle_bullet.png");
    sprite->setScale(0.2f);
    GameObject *obj = new GameObject(world_);
    
    //스프라이트로부터 박스 만들기 분리?
    CCRect rect = sprite->boundingBox();
    b2Body *body = CreateCollisionBox(ut_pos, rect.size.width/2, rect.size.height/2);

    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);

    world_->AddObject(obj, obj->Type());

    return obj;
}

GameObject *GameObjectFactory::CreateDemoBullet(const TestBulletObjectHeader &header, cocos2d::CCNode *parent) {
    
    //충돌 박스를 스프라이트로부터 끌어낸다. (함수 만들자)
    //스프라이트는 헤더에 있는 것과 관계없이 그냥 생성
    CCSprite *sprite = CCSprite::create("circle_bullet.png");
    sprite->setScale(0.2f);
    sprite->setPosition(ccp(header.x, header.y));
    CCRect sprite_box = sprite->boundingBox();

    b2Body *body = CreateCollisionBox(glm::vec2(header.x, header.y),
        sprite_box.size.width / 2.0f, sprite_box.size.height / 2.0f);
    
    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    BulletComponent *logic = new BulletComponent(obj);
    logic->set_damage(10);
    logic->set_dir_vec(glm::vec2(header.dir_x, header.dir_y));
    logic->set_from_enemy(header.from_enemy);
    
    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);

    world_->AddObject(obj, obj->Type());

    return obj;
}

GameObject *GameObjectFactory::CreateDemoCombatPlane(const glm::vec2 &ut_pos, cocos2d::CCNode *parent) {
    b2Body *body = CreateCollisionBox(ut_pos, Unit::ToUnitFromMeter(2.0f), Unit::ToUnitFromMeter(2.0f));
    //바라보는 방향 등 생성을 적절히 해야함
    body->SetTransform(body->GetPosition(), M_PI/6);

    CCSprite *sprite = CCSprite::create("kyoko_icon.png");
    sprite->setScale(0.2f);

    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    LogicComponent *logic = new CombatPlaneComponent(obj, parent);

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);

    world_->AddObject(obj, obj->Type());

    return obj;
}

GameObject *GameObjectFactory::CreateDemoEnemy(const glm::vec2 &ut_pos, cocos2d::CCNode *parent) {
    //Temp
    b2Body *body = CreateCollisionBox(ut_pos, Unit::ToUnitFromMeter(1.0f), Unit::ToUnitFromMeter(1.0f));
    
    CCSprite *sprite = CCSprite::create("kyoko_icon.png");
    sprite->setScale(0.2f);
    
    GameObject *obj = new GameObject(world_);
    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    LogicComponent *logic = new AICharacterComponent(obj);
    
    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);

    world_->AddObject(obj, obj->Type());

    return obj;
}

GameObject *GameObjectFactory::CreateDemoObj(const glm::vec2 &ut_pos, cocos2d::CCNode *parent) {
    const float ptm_ratio = kUnitToMeterRatio;
    
    b2Body *body = CreateCollisionBox(ut_pos, Unit::ToUnitFromMeter(1.0f), Unit::ToUnitFromMeter(1.0f));

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
    world_->AddObject(obj, obj->Type());

    return obj;
}

GameObject *GameObjectFactory::CreateDemoPlayer(const glm::vec2 &ut_pos, cocos2d::CCNode *parent) {
    //일단 그냥 뿌린다 - 전투기 객체랑 다를 바 없음
    //테스트용으로 조작할 걸 만드는게 목적
    GameObject *obj = new GameObject(world_);
    b2Body *body = CreateCollisionBox(ut_pos, Unit::ToUnitFromMeter(2.0f), Unit::ToUnitFromMeter(2.0f));

    CCSprite *sprite = CCSprite::create("kyoko_icon.png");
    sprite->setScale(0.2f);

    DrawableComponent *drawable = new NodeDrawableComponent(obj, parent, sprite);
    PhyComponent *phy = PhyComponent::SinglePhy(obj, body);
    LogicComponent *logic = new PlayerComponent(obj, parent);

    obj->set_drawable_comp(drawable);
    obj->set_phy_comp(phy);
    obj->set_logic_comp(logic);

    world_->AddObject(obj, obj->Type());

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