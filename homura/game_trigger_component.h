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
    GameTriggerComponent(GameObject *obj, TriggerID trigger_id, NextTriggersPtr next_triggers, GameTriggerHandlerPtr game_trigger_handler);
    virtual ~GameTriggerComponent();

public:
    CompType type() const;
    void Update(float dt);
    void InitMsgHandler();

public:
    void OnBeginTriggerMessage(BeginTriggerMessage *msg);

public:
    void set_trigger_id(TriggerID trigger_id) { trigger_id_ = trigger_id; }
    TriggerID trigger_id() { return trigger_id_; }

private:
    TriggerID trigger_id_;
    NextTriggersPtr next_triggers_;
    GameTriggerHandlerPtr game_trigger_handler_;

    bool is_flag_on_;

    //이 이벤트가 시작되고나서부터 지난 시간
    float elapsed_time_;
};

#endif