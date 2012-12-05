// Ŭnicode please
#include "stdafx.h"
#include "game_trigger.h"
#include "game_world.h"
#include "game_object.h"

GameTrigger::GameTrigger() : complete_(false) {

}

GameTrigger::~GameTrigger() {

}

bool GameTrigger::Check() {

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

//여기부터 새로운 트리거들을 작성

TriggerType NullTrigger::Type() {
    return kTriggerNull;
}

TriggerType SpecificDestroyTrigger::Type() {
    return kTriggerSpecificDestroy;
}


bool SpecificDestroyTrigger::CheckFunc() {
    if(NULL == world_->FindObject(id_)) {
        return true;
    }
    return false;
}

void SpecificDestroyTrigger::SetParams(GameObject *obj) {
    world_ = obj->world();
    id_ = obj->id();
}