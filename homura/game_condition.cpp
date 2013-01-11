// Ŭnicode please
#include "stdafx.h"
#include "game_condition.h"
#include "game_world.h"
#include "game_object.h"

GameCondition::GameCondition() : valid_(false), complete_(false) {

}

GameCondition::~GameCondition() {

}

bool GameCondition::Check() {

    if(!valid_)
        return false;

    if(complete_)
        return true;

    else {
        if(CheckFunc()) {
            complete_ = true;
            return true;
        }
        return false;
    }
}

void GameCondition::Reset() {
    valid_ = false;
    complete_ = false;
}

//여기부터 새로운 트리거들을 작성

ConditionType NullCondition::Type() {
    return kConditionNull;
}

ConditionType SpecificDestroyCondition::Type() {
    return kConditionSpecificDestroy;
}


bool SpecificDestroyCondition::CheckFunc() {
    if(NULL == world_->FindObject(id_)) {
        return true;
    }
    return false;
}

void SpecificDestroyCondition::SetParams(GameObject *obj) {
    world_ = obj->world();
    id_ = obj->id();
}

bool TimerCondition::CheckFunc() {
    if(elapsed_time_ > time_) {
        return true;
    }
    //dt를 가져와야하는데 흠..
    elapsed_time_ += 1.0/60;
    return false;
}

ConditionType TimerCondition::Type() {
    return kConditionTimer;
}
