// Ŭnicode please
#include "stdafx.h"
#include "game_stage.h"
#include "game_event.h"
#include "game_trigger.h"
#include "game_event_handler.h"
#include "game_layer.h"

#include "game_object_factory.h"

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

USING_NS_CC;

GameStage::GameStage(GameWorld *world)
    : world_(world), elapsed_time_(0), current_event_(0), layer_(0) {

}

GameStage::~GameStage() {

}

bool GameStage::Init() {
    layer_ = CCLayer::create();

    if(NULL == layer_)
        return false;

    factory_ = new GameObjectFactory(world_);

    //Test
    
    //EventGroup #0
    GameEventHandler *evt_hnd0 = new GameEventHandler();

    TestCombatPlaneObjectHeader combat_header;
    combat_header.angle = 0;
    combat_header.hit_point = 200;
    combat_header.x = 100;
    combat_header.y = 200;
    combat_header.is_enemy = false;
    combat_header.sprite_name = "";
    GameEvent *evt0 = MakeCreateObjectEvent(this, combat_header);
    evt_hnd0->AddEvent(evt0);
    AddEventHandler(evt_hnd0);

    
    //EventGroup #1
    GameEventHandler *evt_hnd1 = new GameEventHandler();

    //Event 1
    combat_header.angle = M_PI;
    combat_header.is_enemy = true;
    combat_header.hit_point = 100;
    combat_header.x = 100;
    combat_header.y = 1000;
    combat_header.sprite_name = "";
    GameEvent *evt1 = MakeCreateObjectEvent(this, combat_header, new SpecificDestroyTrigger());
    evt_hnd1->AddEvent(evt1);
    
    combat_header.x = 300;
    //Event 2
    GameEvent *evt2 = MakeCreateObjectEvent(this, combat_header);
    evt_hnd1->AddEvent(evt2);

    AddEventHandler(evt_hnd1);
    
    
    //EventGroup #2
    //Event 1
    GameEventHandler *evt_hnd2 = new GameEventHandler();

    TestBulletObjectHeader bullet_header;
    bullet_header.x = 600;
    bullet_header.y = 1000;
    bullet_header.angle_rad = 0;
    bullet_header.speed = 10;
    bullet_header.damage = 10;
    GameEvent *evt3 = MakeCreateObjectEvent(this, bullet_header);
    evt_hnd2->AddEvent(evt3);

    AddEventHandler(evt_hnd2);
    
    return true;
}

void GameStage::AddEventHandler(GameEventHandler *event_handler) {
    event_handlers_.push_back(GameEventHandlerPtr(event_handler));
}

void GameStage::Update(float dt) {

    //스케쥴 돌면서 시간 지나면 실행
    elapsed_time_ += dt;
    if(current_event_ < event_handlers_.size()) {
        if(!event_handlers_[current_event_]->all_executed())
            event_handlers_[current_event_]->Run(elapsed_time_);
        if(event_handlers_[current_event_]->IsEnd())
            current_event_++;
    }
}