// Ŭnicode please
#include "stdafx.h"
#include "game_object.h"
#include "game_world.h"
#include "game_event.h"
#include "game_trigger.h"

GameEvent::GameEvent(GameTrigger *trigger) 
    : start_time_(0), end_time_(0), trigger_(trigger), is_event_executed_(false) {

}

GameEvent::~GameEvent() {

}

bool GameEvent::IsEnd() {
    return trigger_->Check();
}

void GameEvent::InvokeRun(float elapsed_time) {
    cocos2d::CCLog("event start");
    if(elapsed_time > start_time_ && !is_event_executed_)
        Run();
}

//여기부터 새로운 게임 이벤트를 나열
void CreateObjectEvent::Run() {

    cocos2d::CCLog("Create Object Event");
    int obj_id = obj_->world()->AddObject(obj_, obj_->Type());
    
    if(trigger()->Type() == kTriggerSpecificDestroy) {
        static_cast<SpecificDestroyTrigger*>(trigger())->SetParams(obj_);
    }

    //
    is_event_executed_ = true;
    trigger()->set_valid(true);
}