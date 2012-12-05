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

    CreateObjectEvent *evt = new CreateObjectEvent(new SpecificDestroyTrigger(), factory_->CreateDemoCombatPlane(
        glm::vec2(400, 400), layer()));
    stage_events_.push_back(evt);

    CreateObjectEvent *evt_ = new CreateObjectEvent(new SpecificDestroyTrigger(), factory_->CreateDemoCombatPlane(
        glm::vec2(200, 1000), layer()));
    stage_events_.push_back(evt_);
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