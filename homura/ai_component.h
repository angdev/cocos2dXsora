// Ŭnicode please
#ifndef __AI_COMPONENT_H__
#define __AI_COMPONENT_H__

#include "game_component.h"

class AIComponent : public GameComponent {
public:
    AIComponent(GameObject *obj) : GameComponent(obj) { }
    virtual ~AIComponent() { }

public:
    virtual void InitMsgHandler();

public:
    void OnIsEnemyMessage(IsEnemyMessage *msg);
    virtual bool IsEnemy() = 0;
};

#endif