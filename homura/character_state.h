// Ŭnicode please
#ifndef __CHARACTER_STATE_H__
#define __CHARACTER_STATE_H__

class CharacterFSM;

enum CharacterStateType {
    kCharacterNormalState,
};

class CharacterState {
public:
    CharacterState(CharacterFSM *fsm) : fsm_(fsm) {}
    virtual ~CharacterState() {}

public:
    virtual CharacterStateType type() = 0;
    virtual void Init() = 0;
    virtual void Handle(float dt) = 0;
    virtual void Cleanup() = 0;

private:
    CharacterFSM *fsm_;
};

#endif