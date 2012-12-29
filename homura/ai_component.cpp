// Ŭnicode please
#include "stdafx.h"
#include "ai_component.h"
#include "game_object.h"
#include "character_component.h"

void AIComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &AIComponent::OnIsEnemyMessage);
}

void AIComponent::OnIsEnemyMessage(IsEnemyMessage *msg) {
    if(IsEnemy())
        msg->is_enemy = true;
}

CharacterComponent* AIComponent::char_comp() {
    return static_cast<CharacterComponent*>(obj()->logic_comp());
}