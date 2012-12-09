// Ŭnicode please
#include "stdafx.h"
#include "game_object.h"
#include "game_world.h"
#include "game_event.h"
#include "game_trigger.h"
#include "game_action.h"

GameEvent::GameEvent(GameStage *stage) 
    : start_time_(0), end_time_(0), stage_(stage), is_action_set_(false), is_trigger_set_(false) {
}

GameEvent::~GameEvent() {

}

bool GameEvent::IsEnd() {
    return trigger_->Check();
}

bool GameEvent::InvokeRun(float elapsed_time) {
    
    if(elapsed_time > start_time_ && !IsRun()) {
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

void GameEvent::set_action( GameAction *action ) {
    if(!is_action_set_) {
        action_ = std::move(std::unique_ptr<GameAction>(action));
        action_->set_event(this);
        is_action_set_ = true;
    }
    else {
        action_.reset(action);
    }
}

void GameEvent::set_trigger( GameTrigger *trigger ) {
    if(!is_trigger_set_) {
        trigger_ = std::move(std::unique_ptr<GameTrigger>(trigger));
        is_trigger_set_ = true;
    }
    else {
        trigger_.reset(trigger);
    }
}

//여기부터 새로운 게임 이벤트를 나열
