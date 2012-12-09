// Ŭnicode please
#ifndef __GAME_TRIGGER_H__
#define __GAME_TRIGGER_H__

#include "game_stage.h"
#include "game_world.h"

class GameObject;
class GameWorld;
//조건을 확인하기 위한 클래스
struct GameCondition;
class GameAction;

//GameTrigger = GameAction + GameCondition
class GameTrigger {
public:
    GameTrigger(GameStage *stage);
    virtual ~GameTrigger();

public:
    bool InvokeRun(float elapsed_time);
    //이벤트&트리거가 끝났는가?
    bool IsEnd();

public:
    void set_action(GameAction *action);
    GameAction *action() { return action_.get(); }
    void set_condition(GameCondition *condition);
    GameCondition *condition() { return condition_.get(); }
    bool IsRun();
    GameStage *stage() { return stage_; }

    //시작, 끝 시간 set/getter
    /*
    */
    
    bool is_action_set() { return is_action_set_; }
    bool is_condition_set() { return is_condition_set_; }

private:
    
    float start_time_;
    float end_time_;

    //실행되는 액션
    std::unique_ptr<GameAction> action_;
    //후결 조건 (다음 이벤트가 실행될 조건)
    std::unique_ptr<GameCondition> condition_;

    //동적 생성을 편하게 하기 위해서
    //액션이 등록되었나
    bool is_action_set_;
    //트리거가 등록되었나
    bool is_condition_set_;

    GameStage *stage_;
};


#endif