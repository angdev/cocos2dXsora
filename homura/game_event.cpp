// Ŭnicode please
#include "stdafx.h"
#include "game_object.h"
#include "game_world.h"
#include "game_event.h"
#include "game_trigger.h"

GameEvent::GameEvent(GameStage *stage, GameTrigger *trigger = new NullTrigger()) 
    : start_time_(0), end_time_(0), stage_(stage), trigger_(trigger), is_event_executed_(false) {

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
