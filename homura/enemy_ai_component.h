// Ŭnicode please
#ifndef __ENEMY_AI_COMPONENT_H__
#define __ENEMY_AI_COMPONENT_H__

#include "ai_component.h"

class EnemyAIComponent : public AIComponent {
public:
    EnemyAIComponent(GameObject *obj);
    virtual ~EnemyAIComponent();

public:
    virtual CompType type() const { return kCompEnemyAI; }
    virtual void Update(float dt);
    virtual void InitMsgHandler();
};

#endif