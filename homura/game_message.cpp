// Ŭnicode please
#include "stdafx.h"
#include "game_message.h"

// 메세지 구현체가 생기면 여기에 하나씩 추가하기

DamageObjectMessage DamageObjectMessage::Create(GameObject *obj) {
    DamageObjectMessage msg;
    msg.obj = obj;
    return msg;
}

ApplyDamageMessage ApplyDamageMessage::Create(float damage) {
    ApplyDamageMessage msg;
    msg.damage = damage;
    return msg;
}

MoveMessage MoveMessage::Create(glm::vec2 vec2) {
    MoveMessage msg;
    msg.vec2 = vec2;
    return msg;
}

DestroyMessage DestroyMessage::Create(int obj_id) {
	DestroyMessage msg;
	msg.obj_id = obj_id;
	return msg;
}


DelayedGameMessage::DelayedGameMessage(GameMessage *msg, float delay) 
    : msg(msg), remain_time_(delay) {}

bool DelayedGameMessage::operator<(const DelayedGameMessage &o) const {
	return remain_time_ < o.remain_time_;
}

void DelayedGameMessage::Update(float dt) {
    remain_time_ -= dt;
}