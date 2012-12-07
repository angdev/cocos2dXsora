// Ŭnicode please
#ifndef __STAGE_EVENT_H__
#define __STAGE_EVENT_H__

#include "game_stage.h"
#include "game_world.h"

class GameObject;
class GameWorld;
//조건을 확인하기 위한 클래스
struct GameTrigger;

class GameEvent {
public:
    GameEvent(GameStage *stage, GameTrigger *trigger);
    virtual ~GameEvent();

public:
    bool InvokeRun(float elapsed_time);
    //이벤트&트리거가 끝났는가?
    bool IsEnd();

public:
    //void set_trigger(GameTrigger *trigger) { trigger_ = trigger; }
    GameTrigger *trigger() { return trigger_.get(); }
    bool is_event_executed() { return is_event_executed_; }
    GameStage *stage() { return stage_; }

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
    std::unique_ptr<GameTrigger> trigger_;
    GameStage *stage_;
};


//여기에 게임 이벤트들 나열

template <typename T>
class CreateObjectEvent : public GameEvent {
public:
    CreateObjectEvent(GameStage *stage, const T &header, GameTrigger *trigger = new NullTrigger()) 
        : GameEvent(stage, trigger), header_(header) {}

    virtual ~CreateObjectEvent() {}

    void Run() {
        cocos2d::CCLog("Create Object Event");
        
        //지연 생성
        GameObjectFactory factory(stage()->world());
        //팩토리 수정해서 이렇게 씀, 유효성 검사?
        GameObject* created_obj = factory.Create(header_, stage()->layer());
        stage()->world()->AddObject(created_obj, created_obj->Type());
        
        if(trigger()->Type() == kTriggerSpecificDestroy) {
            static_cast<SpecificDestroyTrigger*>(trigger())->SetParams(created_obj);
        }

        //
        is_event_executed_ = true;
        trigger()->set_valid(true);
    }

private:
    T header_;
};

//클래스 템플릿 추론 헬퍼 함수 (아직은 일반화 시키지 말자)
template <typename T>
CreateObjectEvent<T> *MakeCreateObjectEvent(GameStage *stage, const T &header, GameTrigger *trigger = new NullTrigger()) {
    CreateObjectEvent<T> *event = new CreateObjectEvent<T>(stage, header, trigger);
    return event;
};



#endif