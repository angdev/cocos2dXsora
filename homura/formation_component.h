// Ŭnicode please
#ifndef __FORMATION_COMPONENT_H__
#define __FORMATION_COMPONENT_H__

#include "logic_component.h"

class GameObject;

//편대 로직
class FormationComponent : public LogicComponent {
public:
    FormationComponent(GameObject *obj);
    virtual ~FormationComponent();
};

#endif