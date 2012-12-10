// Ŭnicode please
#ifndef __GAME_CONDITION_H__
#define __GAME_CONDITION_H__

struct GameCondition {
public:
    GameCondition();
    virtual ~GameCondition();

    bool Check();
    void Reset();
    virtual ConditionType Type() = 0;

public:
    void set_valid(bool valid) { valid_ = valid; }
    bool valid() { return valid_; }

protected:
    //액션이 먼저 실행되어야 후결 트리거 체크 가능
    bool valid_;
    bool complete_;

private:
    virtual bool CheckFunc() = 0;
};


//여기 아래에 조건들을 작성

//아무 조건 없는 빈 조건
struct NullCondition : public GameCondition {
    virtual bool CheckFunc() { return true; }
    virtual ConditionType Type();
};

//특정한 객체가 파괴되었을 때
struct SpecificDestroyCondition : public GameCondition {
private:
    virtual bool CheckFunc();
    
    GameWorld *world_;
    int id_;

public:
    SpecificDestroyCondition() {}
    
public:
    virtual ConditionType Type();
    
    void SetParams(GameObject *obj);

};

#endif