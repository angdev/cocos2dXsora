﻿// Ŭnicode please
#include "stdafx.h"
#include "game_event_handler.h"
#include "game_event.h"

GameEventHandler::GameEventHandler() : all_executed_(false) {

}

GameEventHandler::~GameEventHandler() {

}

void GameEventHandler::AddEvent(GameEvent *event) {
    events_.push_back(GameEventPtr(event));
}

void GameEventHandler::Run(float elapsed_time) {
    //이 중에 elapsed time때문에 실행되지 않는 이벤트가 있다면?
    //그런 이벤트는 따로 빼야지
    all_executed_ = true;
    for(GameEventPtr event : events_) {
        if(event->is_event_executed())
            continue;

        if(!event->InvokeRun(elapsed_time))
            all_executed_ = false;
    }
}

bool GameEventHandler::IsEnd() {
    for(GameEventPtr event : events_) {
        if(!event->IsEnd())
            return false;
    }
    return true;
}