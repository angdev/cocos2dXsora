// Ŭnicode please
#ifndef __GAME_ACTION_H__
#define __GAME_ACTION_H__

#include "game_trigger.h"

typedef unsigned int TriggerID;
typedef std::vector<TriggerID> NextTriggers;

class GameAction {
public:
    GameAction();
    virtual ~GameAction();

public:
    void InvokeRun();
    
public:
    void set_trigger(GameTrigger *trigger) { trigger_ = trigger; }
    GameTrigger *trigger() { return trigger_; }

    bool IsRun() { return is_run_; }
    void Reset();

protected:
    //Run 끝에 넣을 것.
    void EndRun();

private:
    virtual void Run() = 0;

private:
    GameTrigger *trigger_;
    bool is_run_;
};

//여기부터 액션들 추가
class NullAction : public GameAction {
public:
    NullAction() : GameAction() {}
    virtual ~NullAction() {}

private:
    void Run() {}
};

//몇 회 반복하고 특정 트리거로 넘어감.
//구조적으로 좀 이상하지만 일단 이렇게 쓰도록 한다.
class RepeatAction : public GameAction {
public:
    RepeatAction(unsigned int repeat_number, TriggerID next_trigger_id) : GameAction(),
        repeat_number_(repeat_number), current_repeat_number_(0), next_trigger_id_(next_trigger_id) {}
    virtual ~RepeatAction() {}

private:
    void Run();

    //unique_ptr에서 옮길 때 포인터 형식으로 옮길 수는 없나?
    TriggerID next_trigger_id_;
    //현재 반복 횟수
    unsigned int current_repeat_number_;
    const unsigned int repeat_number_;
};

template <typename T>
class CreateObjectAction : public GameAction {
public:
    CreateObjectAction(const T &header) 
        : GameAction(), header_(header) {}

    virtual ~CreateObjectAction() {}
private:
    void Run() {
        cocos2d::CCLog("Create Object Event");

        //지연 생성
        GameObjectFactory factory(trigger()->stage()->world());
        //팩토리 수정해서 이렇게 씀, 유효성 검사?
        GameObject* created_obj = factory.Create(header_, trigger()->stage()->layer());
        trigger()->stage()->world()->AddObject(created_obj, created_obj->Type());

        if(trigger()->condition()->Type() == kConditionSpecificDestroy) {
            static_cast<SpecificDestroyCondition*>(trigger()->condition())->SetParams(created_obj);
        }

        EndRun();
    }

private:
    T header_;
};

//클래스 템플릿 추론 헬퍼 함수 (아직은 일반화 시키지 말자)
template <typename T>
CreateObjectAction<T> *MakeCreateObjectAction(const T &header) {
    CreateObjectAction<T> *action = new CreateObjectAction<T>(header);
    return action;
};


class VictoryAction : public GameAction {
public:
    VictoryAction() {}
    virtual ~VictoryAction() {}

private:
    void Run();
};

#endif