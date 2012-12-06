// Ŭnicode please
#include "stdafx.h"
#include "game_event_component.h"
#include "game_event_handler.h"
#include "game_object.h"
#include "game_world.h"

GameEventComponent::GameEventComponent(GameObject *obj, EventID event_id, NextEventsPtr next_events, GameEventHandlerPtr game_event_handler) 
    : LogicComponent(obj), elapsed_time_(0), event_id_(event_id), next_events_(next_events), game_event_handler_(game_event_handler), is_flag_on_(false) {

}

GameEventComponent::~GameEventComponent() {

}

CompType GameEventComponent::type() const {
    return kCompGameEvent;
}

void GameEventComponent::Update(float dt) {

    if(!is_flag_on_)
        return;

    elapsed_time_ += dt;
    if(!game_event_handler_->all_executed()) {
        game_event_handler_->Run(elapsed_time_);
    }

    if(game_event_handler_->IsEnd()) {
        //월드에 다음 이벤트를 작동 시키는 메시지를 보냄.
        for(int event_id : *next_events_) {
            BeginEventMessage msg = BeginEventMessage::Create(event_id);
            obj()->world()->OnMessage(&msg);
        }
        //종료
        is_flag_on_ = false;
        //삭제는 보류
    }
}

void GameEventComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &GameEventComponent::OnBeginEventMessage);
}

void GameEventComponent::OnBeginEventMessage(BeginEventMessage *msg) {
    if(msg->event_id == event_id_)
        is_flag_on_ = true;
}
