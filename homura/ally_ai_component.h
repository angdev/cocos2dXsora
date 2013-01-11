// Ŭnicode please
#ifndef __ALLY_AI_COMPONENT_H__
#define __ALLY_AI_COMPONENT_H__

#include "ai_component.h"

enum AllyAIState {
    kAllyAppearState,
    kAllyFallState,
    kAllyArrestState,
    kAllyNormalState,
    kAllyFormationState,    //편대에 속해있는 상태
};

class AllyAIComponent : public AIComponent {
public:
    AllyAIComponent(GameObject *obj);
    virtual ~AllyAIComponent();

public:
    virtual CompType type() const { return kCompAllyAI; }
    virtual void Update(float dt);
    virtual void InitMsgHandler();

public:
    virtual bool IsEnemy() { return false; }
    AllyAIState state() { return state_; }
    virtual void set_state(int state) { state_ = static_cast<AllyAIState>(state); }

private:
    AllyAIState state_;
};

#endif