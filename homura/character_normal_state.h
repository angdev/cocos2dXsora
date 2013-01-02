// Ŭnicode please
#ifndef __CHARACTER_NORMAL_STATE_H__
#define __CHARACTER_NORMAL_STATE_H__

#include "character_state.h"

class CharacterNormalState : public CharacterState {
public:
    CharacterNormalState(CharacterFSM *fsm) : CharacterState(fsm) {}
    virtual ~CharacterNormalState() {}

public:
    virtual CharacterStateType type() { return kCharacterNormalState; }
    virtual void Init() {}
    virtual void Handle(float dt) {}
    virtual void Cleanup() {}
};

#endif