// Ŭnicode please
#ifndef __PHY_COMPONENT_H__
#define __PHY_COMPONENT_H__

#include "game_component.h"

class PhyComponent : public GameComponent
{
public:
    PhyComponent(GameObject *obj) : GameComponent(obj) { }
    virtual ~PhyComponent() { }

private:

};

#endif