// Ŭnicode please
#include "stdafx.h"
#include "game_action.h"
#include "game_trigger_handler.h"
#include "game_condition.h"

GameAction::GameAction() : trigger_(NULL), is_run_(false) {

}

GameAction::~GameAction() {

}

void GameAction::InvokeRun() {
    if(trigger_ == NULL)
        return;
    Run();
}

void GameAction::Reset() {
    is_run_ = false;
}

void GameAction::EndRun() {
    is_run_ = true;
    trigger()->condition()->set_valid(true);
}

void VictoryAction::Run() {
    trigger()->stage()->set_is_cleared(true);
    EndRun();
}

void RepeatAction::Run() {
    if(current_repeat_number_ < repeat_number_) {
        current_repeat_number_++;
    }
    else {
        //unique_ptr로 들어감.
        NextTriggers *next_trgs = new NextTriggers();
        next_trgs->push_back(next_trigger_id_);
        trigger()->trigger_handler()->set_next_triggers(next_trgs);
    }
    EndRun();
}
