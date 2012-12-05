// Ŭnicode please
#include "stdafx.h"
#include "game_stage.h"
#include "game_event.h"
#include "game_trigger.h"

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
    
    TestCombatPlaneObjectHeader combat_header;
    combat_header.hit_point = 100;
    combat_header.x = 100;
    combat_header.y = 1000;
    combat_header.sprite_name = "";
    GameEvent *evt = MakeCreateObjectEvent(this, combat_header);
    stage_events_.push_back(evt);

    combat_header.x = 300;

    GameEvent *evt_ = MakeCreateObjectEvent(this, combat_header, new SpecificDestroyTrigger());
    stage_events_.push_back(evt_);
    

    TestBulletObjectHeader bullet_header;
    bullet_header.x = 100;
    bullet_header.y = 100;
    bullet_header.angle_rad = M_PI/2;
    bullet_header.speed = 10;
    bullet_header.damage = 10;
    GameEvent *evt__ = MakeCreateObjectEvent(this, bullet_header);
    stage_events_.push_back(evt__);

    return true;
}

void GameStage::AddEvent(GameEvent *event) {
    stage_events_.push_back(event);
}

void GameStage::Update(float dt) {

    //스케쥴 돌면서 시간 지나면 실행
    elapsed_time_ += dt;
    if(current_event_ < stage_events_.size()) {
        if(!stage_events_[current_event_]->is_event_executed())
            stage_events_[current_event_]->InvokeRun(elapsed_time_);
        if(stage_events_[current_event_]->IsEnd()) {
            current_event_++;
        }
    }
}