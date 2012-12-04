// Ŭnicode please
#include "stdafx.h"
#include "ai_character_component.h"
#include "game_object.h"
#include "sora/unit.h"

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
    
    glm::vec2 vec2(0, 1);
    o->OnMessage(&MoveMessage::Create(vec2));
}