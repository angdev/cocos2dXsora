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

void SpecificDestroyTrigger::SetParams(GameObject* obj) {

    cocos2d::CCLog("trg/obj id : %d", obj->id());

    id_ = obj->id();
    world_ = obj->world();
    //오브젝트를 들고 있으면 참조할 수도 있으니 일단 이렇게 한다.
}

bool SpecificDestroyTrigger::CheckFunc() {
    if(NULL == world_->FindObject(id_)) {
        return true;
    }
    return false;
}