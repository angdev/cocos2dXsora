// Ŭnicode please
#include "stdafx.h"
#include "game_action.h"


GameAction::GameAction() : trigger_(NULL), is_run_(false) {

}

GameAction::~GameAction() {

}

void GameAction::InvokeRun() {
    if(trigger_ == NULL)
        return;
    Run();
}

void VictoryAction::Run() {
    trigger()->stage()->set_is_cleared(true);
    is_run_ = true;
}
