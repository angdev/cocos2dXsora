// Ŭnicode please
#ifndef __CHARACTER_FSM_H__
#define __CHARACTER_FSM_H__

class CharacterComponent;

enum CharacterStateType;
class CharacterState;
typedef std::shared_ptr<CharacterState> CharacterStatePtr;

class CharacterFSM {
public:
    CharacterFSM(CharacterComponent *char_comp);
    virtual ~CharacterFSM();

public:
    void Update(float dt);

    //초기화 용도로 써야 적절
    void InsertState(CharacterStatePtr state);
    //내부에 추가되어 있어야 사용 가능
    void SetState(CharacterStateType state_type);
    
    CharacterComponent *char_comp() { return char_comp_; }

private:
    typedef std::map<CharacterStateType, CharacterStatePtr> CharacterStateMap;
    CharacterStateMap state_map_;
    CharacterStatePtr current_state_;

    CharacterComponent *char_comp_;
};

#endif