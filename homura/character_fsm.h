/*
// Ŭnicode please
#ifndef __CHARACTER_FSM_H__
#define __CHARACTER_FSM_H__

class CharacterComponent;

enum AllyCharacterState {
    kAllyFallState, //낙오 상태
    kAllyArrestState,   //구속 상태
    kAllyNormalState,   //편대에 들어간 보통 상태
};

enum EnemyCharacterState {
    kEnemyNormalState,
};

class CharacterFSM {
public:
    CharacterFSM(CharacterComponent *char_comp);
    virtual ~CharacterFSM();

public:
    //상속의 가능성?
    virtual void Update(float dt);

public:
    AllyCharacterState ally_state() { return ally_state_; }
    void set_ally_state(AllyCharacterState state) { ally_state_ = state; }
    EnemyCharacterState enemy_state() { return enemy_state_; }
    void set_enemy_state(EnemyCharacterState state) { enemy_state_ = state; }

private:
    //이걸 나중에 클래스로 분리하던지.
    void AllyFSMUpdate(float dt);
    void EnemyFSMUpdate(float dt);

private:
    CharacterComponent *char_comp_;
    AllyCharacterState ally_state_;
    EnemyCharacterState enemy_state_;
};

#endif
*/