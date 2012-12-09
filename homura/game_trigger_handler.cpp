// Ŭnicode please
#include "stdafx.h"
#include "game_trigger_handler.h"
#include "game_trigger.h"

GameTriggerHandler::GameTriggerHandler() : all_executed_(false) {

}

GameTriggerHandler::~GameTriggerHandler() {

}

void GameTriggerHandler::AddTrigger(GameTrigger *trigger) {
    //이벤트에 트리거, 액션 준비가 되어야 함.
    if(trigger->is_action_set() && trigger->is_condition_set()) {
        triggers_.push_back(GameTriggerPtr(trigger));
    }
}

void GameTriggerHandler::Run(float elapsed_time) {
    //이 중에 elapsed time때문에 실행되지 않는 이벤트가 있다면?
    //그런 이벤트는 따로 빼야지
    all_executed_ = true;
    for(GameTriggerPtr trigger : triggers_) {
        if(trigger->IsRun())
            continue;

        if(!trigger->InvokeRun(elapsed_time))
            all_executed_ = false;
    }
}

bool GameTriggerHandler::IsEnd() {
    for(GameTriggerPtr trigger : triggers_) {
        if(!trigger->IsEnd())
            return false;
    }
    return true;
}