// Ŭnicode please
#ifndef __PLAYER_COMPONENT_H__
#define __PLAYER_COMPONENT_H__

#include "character_component.h"

class PlayerComponent : public CharacterComponent {
public:
    PlayerComponent(GameObject *obj);
    ~PlayerComponent();

private:

};

#endif