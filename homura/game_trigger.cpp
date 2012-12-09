// Ŭnicode please
#include "stdafx.h"
#include "game_object.h"
#include "game_world.h"
#include "game_condition.h"
#include "game_trigger.h"
#include "game_action.h"
#include "game_trigger_handler.h"

GameTrigger::GameTrigger(GameStage *stage) 
    : start_time_(0), end_time_(0), stage_(stage), is_action_set_(false), is_condition_set_(false) {
}

GameTrigger::~GameTrigger() {

}

bool GameTrigger::IsEnd() {
    return condition_->Check();
}

bool GameTrigger::InvokeRun(float elapsed_time) {
    
    if(elapsed_time > start_time_ && !IsRun()) {
        if(action_->trigger() != NULL) {
            cocos2d::CCLog("trigger %d start", trigger_handler()->trigger_id());
            action_->InvokeRun();
            return true;
        }
    }
    return false;
    
}

bool GameTrigger::IsRun() {
    return action_->IsRun();
}

void GameTrigger::set_action( GameAction *action ) {
    if(!is_action_set_) {
        action_ = std::move(std::unique_ptr<GameAction>(action));
        action_->set_trigger(this);
        is_action_set_ = true;
    }
    else {
        action_.reset(action);
    }
}

void GameTrigger::set_condition( GameCondition *condition ) {
    if(!is_condition_set_) {
        condition_ = std::move(std::unique_ptr<GameCondition>(condition));
        is_condition_set_ = true;
    }
    else {
        condition_.reset(condition);
    }
}

void GameTrigger::Reset() {
    action_->Reset();
    condition_->Reset();
}

//여기부터 새로운 게임 이벤트를 나열
