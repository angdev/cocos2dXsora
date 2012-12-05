// Ŭnicode please
#ifndef __STAGE_EVENT_H__
#define __STAGE_EVENT_H__

class GameObject;
//조건을 확인하기 위한 클래스
struct GameTrigger;

class GameEvent {
public:
    GameEvent(GameTrigger *trigger);
    virtual ~GameEvent();

public:
    void InvokeRun(float elapsed_time);
    //이벤트&트리거가 끝났는가?
    bool IsEnd();

public:
    //void set_trigger(GameTrigger *trigger) { trigger_ = trigger; }
    GameTrigger *trigger() { return trigger_; }
    bool is_event_executed() { return is_event_executed_; }

    //시작, 끝 시간 set/getter
    /*
    */

protected:
    bool is_event_executed_;

private:
    virtual void Run() = 0;
    
private:
    float start_time_;
    float end_time_;

    //후결 조건 (다음 이벤트가 실행될 조건)
    GameTrigger *trigger_;
};


//여기에 게임 이벤트들 나열

class CreateObjectEvent : public GameEvent {
public:
    CreateObjectEvent(GameTrigger *trigger, GameObject *obj) 
        : GameEvent(trigger), obj_(obj) {}
    virtual ~CreateObjectEvent() {}
    
    void Run();

private:
    GameObject *obj_;
};



#endif