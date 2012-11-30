ï»?/ Å¬nicode please
#include "stdafx.h"
#include "game_component.h"
#include "game_message.h"

GameComponent::GameComponent(GameObject *obj)
	: obj_(obj) {
}

GameComponent::~GameComponent() {
}

void GameComponent::OnMessage(const GameMessage *msg) {
	msg_handler_.HandleMsg(msg);
}