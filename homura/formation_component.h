// Ŭnicode please
#ifndef __FORMATION_COMPONENT_H__
#define __FORMATION_COMPONENT_H__

#include "logic_component.h"

struct RequestJoinFormationMessage;

class GameObject;

//편대 로직
//실질적인 AI에 해당한다 (명령자)
//사실상 편대장 기체는 필요 없다
class FormationComponent : public LogicComponent {

    enum {
        NO_LEADER = -1,
    };

public:
    explicit FormationComponent(GameObject *obj);
    virtual ~FormationComponent();

public:
    virtual CompType type() const { return kCompFormation; }
    virtual void Update(float dt);
    virtual void InitMsgHandler();

public:
    void OnRequestJoinFormationMessage(RequestJoinFormationMessage *msg);
    void OnDestroyMessage(DestroyMessage *msg);

public:
    std::set<int> member_set_;
    int leader_id_;
};

#endif