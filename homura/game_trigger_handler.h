// Ŭnicode please
#ifndef __GAME_TRIGGER_HANDLER_H__
#define __GAME_TRIGGER_HANDLER_H__

class GameTrigger;
typedef std::shared_ptr<GameTrigger> GameTriggerPtr;

typedef unsigned int TriggerID;
typedef std::vector<TriggerID> NextTriggers;
typedef std::shared_ptr<NextTriggers> NextTriggersPtr;

//TODO
//일단 이벤트는 여기에 포함되어 있어야 한다.
//나중에 스테이지 부분 수정할 것.
//이 클래스가 있는건 한 번에 여러 이벤트/트리거를 돌릴 수 있게 하기 위함.
class GameTriggerHandler {
public:
    GameTriggerHandler();
    virtual ~GameTriggerHandler();
    
public:
    void AddTrigger(GameTrigger *trigger);
    void Run(float elapsed_time);
    bool IsEnd();
    void Reset();

public:
    bool all_executed() { return all_executed_; }
    void set_trigger_id(TriggerID trigger_id) { trigger_id_ = trigger_id; }
    TriggerID trigger_id() { return trigger_id_; }
    void set_next_triggers(NextTriggers *next_triggers);
    NextTriggers *next_triggers() { return next_triggers_.get(); }

private:
    typedef std::vector<GameTriggerPtr> GameTriggers;
    GameTriggers triggers_;

    TriggerID trigger_id_;
    std::unique_ptr<NextTriggers> next_triggers_;

    bool all_executed_;
};

#endif