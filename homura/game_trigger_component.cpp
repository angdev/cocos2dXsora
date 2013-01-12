// Ŭnicode please
#include "stdafx.h"
#include "game_trigger_component.h"
#include "game_trigger_handler.h"
#include "game_object.h"
#include "game_world.h"

GameTriggerComponent::GameTriggerComponent(GameObject *obj,  GameTriggerHandlerPtr game_trigger_handler) 
    : LogicComponent(obj), elapsed_time_(0), game_trigger_handler_(game_trigger_handler), is_flag_on_(false) {

}

GameTriggerComponent::~GameTriggerComponent() {

}

CompType GameTriggerComponent::type() const {
    return kCompGameTrigger;
}

void GameTriggerComponent::Update(float dt) {

    if(!is_flag_on_)
        return;

    if(!obj()->IsEnabled()) {
        return;
    }

    elapsed_time_ += dt;
    if(!game_trigger_handler_->all_executed()) {
        game_trigger_handler_->Run(elapsed_time_);
    }

    if(game_trigger_handler_->IsEnd()) {
        //월드에 다음 이벤트를 작동 시키는 메시지를 보냄.
        for(int trigger_id : *(game_trigger_handler_->next_triggers())) {
            BeginTriggerMessage msg = BeginTriggerMessage::Create(trigger_id);
            obj()->world()->OnMessage(&msg);
        }
        //종료
        is_flag_on_ = false;
        //삭제는 보류
        //삭제 시키자
        //반복 안 쓰니까
        obj()->Disable();
        obj()->world()->RequestRemoveObject(obj()->world()->FindObject(obj()->id()));
    }
}

void GameTriggerComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &GameTriggerComponent::OnBeginTriggerMessage);
}

void GameTriggerComponent::OnBeginTriggerMessage(BeginTriggerMessage *msg) {
    if(msg->trigger_id == game_trigger_handler_->trigger_id()) {
        //트리거 초기화
        Reset();
        is_flag_on_ = true;
    }
}

void GameTriggerComponent::Reset() {
    game_trigger_handler_->Reset();
}
