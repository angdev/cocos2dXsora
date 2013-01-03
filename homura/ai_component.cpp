// Ŭnicode please
#include "stdafx.h"
#include "ai_component.h"
#include "game_object.h"
#include "character_component.h"

AIComponent::AIComponent(GameObject *obj) : GameComponent(obj), start_position_() {

}

void AIComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &AIComponent::OnIsEnemyMessage);
}

void AIComponent::OnIsEnemyMessage(IsEnemyMessage *msg) {
    msg->is_ret = true;

    if(IsEnemy()) {
        msg->is_enemy = true;
    }
}

CharacterComponent* AIComponent::char_comp() {
    return static_cast<CharacterComponent*>(obj()->logic_comp());
}