// Ŭnicode please
#ifndef __GAME_ACTION_H__
#define __GAME_ACTION_H__

#include "game_trigger.h"

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

protected:
    bool is_run_;

private:
    virtual void Run() = 0;

private:
    GameTrigger *trigger_;
};

//여기부터 액션들 추가
class NullAction : public GameAction {
public:
    NullAction() : GameAction() {}
    virtual ~NullAction() {}

private:
    void Run() {}
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

        //
        is_run_ = true;
        trigger()->condition()->set_valid(true);
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

#endif