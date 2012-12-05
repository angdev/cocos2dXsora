// Ŭnicode please
#include "stdafx.h"
#include "ai_character_component.h"
#include "game_object.h"
#include "sora/unit.h"

#include "game_message.h"

AICharacterComponent::AICharacterComponent(GameObject *obj)
    : CharacterComponent(obj) {

}

AICharacterComponent::~AICharacterComponent() {

}

void AICharacterComponent::Update(float dt) {
    CharacterComponent::Update(dt);

    AIUpdate(dt);
}

void AICharacterComponent::AIUpdate(float dt) {
    GameObject *o = obj();
    if(NULL == o)
        return;
    
    glm::vec2 vec(0, 1);
    MoveMessage msg = MoveMessage::Create(vec);
    o->OnMessage(&msg);
}