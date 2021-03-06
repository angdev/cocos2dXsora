﻿// Ŭnicode please
#include "stdafx.h"
#include "game_trigger_handler.h"
#include "game_trigger.h"

GameTriggerHandler::GameTriggerHandler() : all_executed_(false) {

}

GameTriggerHandler::~GameTriggerHandler() {

}

void GameTriggerHandler::AddTrigger(GameTrigger *trigger) {
    if(trigger->is_action_set() && trigger->is_condition_set()) {
        trigger->set_trigger_handler(this);
        triggers_.push_back(GameTriggerPtr(trigger));
    }
}

void GameTriggerHandler::Run(float elapsed_time) {
    //이 중에 elapsed time때문에 실행되지 않는 트리거가 있다면?
    //그런 트리거는 따로 빼야지
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

void GameTriggerHandler::Reset() {
    for(GameTriggerPtr trigger : triggers_) {
        trigger->Reset();
    }
    all_executed_ = false;
}

void GameTriggerHandler::set_next_triggers(NextTriggers *next_triggers) {
    next_triggers_.reset(next_triggers);
}
