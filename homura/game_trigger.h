// Ŭnicode please
#ifndef __GAME_TRIGGER_H__
#define __GAME_TRIGGER_H__

struct GameTrigger {
public:
    GameTrigger();
    virtual ~GameTrigger();

    bool Check();
    virtual TriggerType Type() = 0;

public:
    void set_valid(bool valid) { valid_ = valid; }
    bool valid() { return valid_; }

protected:
    //이벤트가 먼저 실행되어야 후결 트리거 체크 가능
    bool valid_;
    bool complete_;

private:
    virtual bool CheckFunc() = 0;
};


//여기 아래에 트리거들을 작성

//아무 조건 없는 빈 트리거
struct NullTrigger : public GameTrigger {
    virtual bool CheckFunc() { return true; }
    virtual TriggerType Type();
};

//특정한 객체가 파괴되었을 때
struct SpecificDestroyTrigger : public GameTrigger {
private:
    virtual bool CheckFunc();
    
    GameWorld *world_;
    int id_;

public:
    SpecificDestroyTrigger() {}
    
public:
    virtual TriggerType Type();
    
    void SetParams(GameObject *obj);

};

#endif