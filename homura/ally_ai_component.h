// Ŭnicode please
#ifndef __ALLY_AI_COMPONENT_H__
#define __ALLY_AI_COMPONENT_H__

#include "ai_component.h"

class AllyAIComponent : public AIComponent {
public:
    AllyAIComponent(GameObject *obj);
    virtual ~AllyAIComponent();

public:
    virtual CompType type() const { return kCompAllyAI; }
    virtual void Update(float dt);
    virtual void InitMsgHandler();
};

#endif