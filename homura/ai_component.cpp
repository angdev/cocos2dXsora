// Ŭnicode please
#include "stdafx.h"
#include "ai_component.h"

void AIComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &AIComponent::OnIsEnemyMessage);
}

void AIComponent::OnIsEnemyMessage(IsEnemyMessage *msg) {
    if(IsEnemy())
        msg->is_enemy = true;
}
