// Ŭnicode please
#include "stdafx.h"
#include "game_object.h"
#include "game_world.h"
#include "game_event.h"
#include "game_trigger.h"
#include "game_action.h"

GameEvent::GameEvent(GameStage *stage, GameAction *action, GameTrigger *trigger = new NullTrigger()) 
    : start_time_(0), end_time_(0), stage_(stage), is_event_executed_(false) {
    trigger_ = std::move(std::unique_ptr<GameTrigger>(trigger));
    action_ = std::move(std::unique_ptr<GameAction>(action));
    action_->set_event(this);
}

GameEvent::~GameEvent() {

}

bool GameEvent::IsEnd() {
    return trigger_->Check();
}

bool GameEvent::InvokeRun(float elapsed_time) {
    
    if(elapsed_time > start_time_ && !is_event_executed_) {
        if(action_->event() != NULL) {
            cocos2d::CCLog("event start");
            action_->InvokeRun();
            return true;
        }
    }
    return false;
    
}

bool GameEvent::IsRun() {
    return action_->IsRun();
}

//여기부터 새로운 게임 이벤트를 나열
