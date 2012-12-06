// Ŭnicode please
#ifndef __GAME_EVENT_COMPONENT_H__
#define __GAME_EVENT_COMPONENT_H__

#include "logic_component.h"

class GameEventHandler;
typedef std::shared_ptr<GameEventHandler> GameEventHandlerPtr;

typedef unsigned int EventID;
typedef std::vector<EventID> NextEvents;
typedef std::shared_ptr<NextEvents> NextEventsPtr;

class GameEventComponent : public LogicComponent {
public:
    GameEventComponent(GameObject *obj, EventID event_id, NextEventsPtr next_events, GameEventHandlerPtr game_event_handler);
    virtual ~GameEventComponent();

public:
    CompType type() const;
    void Update(float dt);
    void InitMsgHandler();

public:
    void OnBeginEventMessage(BeginEventMessage *msg);

public:
    void set_event_id(EventID event_id) { event_id_ = event_id; }
    EventID event_id() { return event_id_; }

private:
    EventID event_id_;
    NextEventsPtr next_events_;
    GameEventHandlerPtr game_event_handler_;

    bool is_flag_on_;

    //이 이벤트가 시작되고나서부터 지난 시간
    float elapsed_time_;
};

#endif