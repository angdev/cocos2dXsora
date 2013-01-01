// Ŭnicode please
#include "stdafx.h"
#include "character_fsm.h"

#include "character_state.h"

CharacterFSM::CharacterFSM(CharacterComponent *char_comp)
    : char_comp_(char_comp), current_state_(NULL) {

}

CharacterFSM::~CharacterFSM() {

}

void CharacterFSM::Update(float dt) {
    if(current_state_ != NULL)
        current_state_->Handle(dt);
}

void CharacterFSM::InsertState(CharacterStatePtr state) {
    //중복 검사
    //set이 나으려나 =ㅅ=;;
    auto found = state_map_.find(state->type());
    if(found != state_map_.end())
        return;

    state_map_.insert(std::make_pair(state->type(), state));
}

void CharacterFSM::SetState(CharacterStateType state_type) {
    auto found = state_map_.find(state_type);
    if(found != state_map_.end())
        return;

    if(current_state_ != NULL)
        current_state_->Cleanup();
    current_state_ = found->second;
    current_state_->Init();
}