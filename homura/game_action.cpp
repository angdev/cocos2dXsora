// Ŭnicode please
#include "stdafx.h"
#include "game_action.h"


GameAction::GameAction() : event_(NULL), is_run_(false) {

}

GameAction::~GameAction() {

}

void GameAction::InvokeRun() {
    if(event_ == NULL)
        return;
    Run();
}
