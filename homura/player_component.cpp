// Ŭnicode please
#include "stdafx.h"
#include "player_component.h"
#include "game_object.h"
#include "game_world.h"

PlayerComponent::PlayerComponent(GameObject *obj, cocos2d::CCNode *layer)
    : CharacterComponent(obj, layer) {

}

PlayerComponent::~PlayerComponent() {

}

void PlayerComponent::Update(float dt) {
    CharacterComponent::Update(dt);
}

void PlayerComponent::InitMsgHandler() {
    CharacterComponent::InitMsgHandler();
}

void PlayerComponent::Destroy() {
    obj()->Disable();
}