// Ŭnicode please
#ifndef __PHY_COMPONENT_H__
#define __PHY_COMPONENT_H__

#include "game_component.h"

class PhyComponent : public GameComponent {
public:
    PhyComponent(GameObject *obj) : GameComponent(obj) { }
    virtual ~PhyComponent() { }

public:
    virtual CompType type() const { return kCompPhy; }
	virtual void Update(float dt);
	virtual void InitMsgHandler();

private:

};

#endif