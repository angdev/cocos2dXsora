// Ŭnicode please
#ifndef __GAME_EVENT_HANDLER_H__
#define __GAME_EVENT_HANDLER_H__

class GameEvent;
typedef std::shared_ptr<GameEvent> GameEventPtr;

//TODO
//일단 이벤트는 여기에 포함되어 있어야 한다.
//나중에 스테이지 부분 수정할 것.
//이 클래스가 있는건 한 번에 여러 이벤트/트리거를 돌릴 수 있게 하기 위함.
class GameEventHandler {
public:
    GameEventHandler();
    virtual ~GameEventHandler();
    
public:
    void AddEvent(GameEvent *event);
    void Run(float elapsed_time);
    bool IsEnd();

public:
    bool all_executed() { return all_executed_; }

private:
    typedef std::vector<GameEventPtr> GameEvents;
    GameEvents events_;

    bool all_executed_;
};

#endif