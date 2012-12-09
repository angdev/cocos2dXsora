// Ŭnicode please
#ifndef __GAME_EVENT_COMPONENT_H__
#define __GAME_EVENT_COMPONENT_H__

#include "logic_component.h"

class GameTriggerHandler;
typedef std::shared_ptr<GameTriggerHandler> GameTriggerHandlerPtr;

typedef unsigned int TriggerID;
typedef std::vector<TriggerID> NextTriggers;
typedef std::shared_ptr<NextTriggers> NextTriggersPtr;

class GameTriggerComponent : public LogicComponent {
public:
    GameTriggerComponent(GameObject *obj, GameTriggerHandlerPtr game_trigger_handler);
    virtual ~GameTriggerComponent();

public:
    CompType type() const;
    void Update(float dt);
    void InitMsgHandler();

public:
    void OnBeginTriggerMessage(BeginTriggerMessage *msg);

public:

private:
    void Reset();

private:
    GameTriggerHandlerPtr game_trigger_handler_;

    bool is_flag_on_;

    //이 이벤트가 시작되고나서부터 지난 시간
    float elapsed_time_;
};

#endif