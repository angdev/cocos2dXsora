// Ŭnicode please
#include "stdafx.h"
#include "game_trigger_component.h"
#include "game_trigger_handler.h"
#include "game_object.h"
#include "game_world.h"

GameTriggerComponent::GameTriggerComponent(GameObject *obj, TriggerID trigger_id, NextTriggersPtr next_triggers, GameTriggerHandlerPtr game_trigger_handler) 
    : LogicComponent(obj), elapsed_time_(0), trigger_id_(trigger_id), next_triggers_(next_triggers), game_trigger_handler_(game_trigger_handler), is_flag_on_(false) {

}

GameTriggerComponent::~GameTriggerComponent() {

}

CompType GameTriggerComponent::type() const {
    return kCompGameTrigger;
}

void GameTriggerComponent::Update(float dt) {

    if(!is_flag_on_)
        return;

    elapsed_time_ += dt;
    if(!game_trigger_handler_->all_executed()) {
        game_trigger_handler_->Run(elapsed_time_);
    }

    if(game_trigger_handler_->IsEnd()) {
        //월드에 다음 이벤트를 작동 시키는 메시지를 보냄.
        for(int trigger_id : *next_triggers_) {
            BeginTriggerMessage msg = BeginTriggerMessage::Create(trigger_id);
            obj()->world()->OnMessage(&msg);
        }
        //종료
        is_flag_on_ = false;
        //삭제는 보류
    }
}

void GameTriggerComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &GameTriggerComponent::OnBeginTriggerMessage);
}

void GameTriggerComponent::OnBeginTriggerMessage(BeginTriggerMessage *msg) {
    if(msg->trigger_id == trigger_id_)
        is_flag_on_ = true;
}
