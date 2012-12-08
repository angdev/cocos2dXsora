// Ŭnicode please
#ifndef __STAGE_EVENT_H__
#define __STAGE_EVENT_H__

#include "game_stage.h"
#include "game_world.h"

class GameObject;
class GameWorld;
//조건을 확인하기 위한 클래스
struct GameTrigger;
class GameAction;

//GameEvent = GameAction + GameTrigger
class GameEvent {
public:
    GameEvent(GameStage *stage);
    virtual ~GameEvent();

public:
    bool InvokeRun(float elapsed_time);
    //이벤트&트리거가 끝났는가?
    bool IsEnd();

public:
    void set_action(GameAction *action);
    GameAction *action() { return action_.get(); }
    void set_trigger(GameTrigger *trigger);
    GameTrigger *trigger() { return trigger_.get(); }
    bool IsRun();
    GameStage *stage() { return stage_; }

    //시작, 끝 시간 set/getter
    /*
    */
    
    bool is_action_set() { return is_action_set_; }
    bool is_trigger_set() { return is_trigger_set_; }

private:
    
    float start_time_;
    float end_time_;

    //실행되는 액션
    std::unique_ptr<GameAction> action_;
    //후결 조건 (다음 이벤트가 실행될 조건)
    std::unique_ptr<GameTrigger> trigger_;

    //동적 생성을 편하게 하기 위해서
    //액션이 등록되었나
    bool is_action_set_;
    //트리거가 등록되었나
    bool is_trigger_set_;

    GameStage *stage_;
};


#endif