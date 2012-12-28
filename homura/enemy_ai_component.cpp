// Ŭnicode please
#include "stdafx.h"
#include "enemy_ai_component.h"

EnemyAIComponent::EnemyAIComponent(GameObject *obj) : AIComponent(obj) {

}

EnemyAIComponent::~EnemyAIComponent() {

}

void EnemyAIComponent::Update(float dt) {

}

void EnemyAIComponent::InitMsgHandler() {
    AIComponent::InitMsgHandler();
}

