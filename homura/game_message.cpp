// Ŭnicode please
#include "stdafx.h"
#include "game_message.h"

// 메세지 구현체가 생기면 여기에 하나씩 추가하기

GetPhyBodyInfoMessage GetPhyBodyInfoMessage::Create(PhyBodyInfo *info) {
    GetPhyBodyInfoMessage msg;
    msg.phy_body_info = info;
    msg.is_ret = false;
    return msg;
}

DamageObjectMessage DamageObjectMessage::Create(GameObject *obj) {
    DamageObjectMessage msg;
    msg.obj = obj;
    return msg;
}

ApplyDamageMessage ApplyDamageMessage::Create(float damage, bool from_enemy) {
    ApplyDamageMessage msg;
    msg.damage = damage;
    msg.from_enemy = from_enemy;
    msg.applied = false;
    return msg;
}

MoveMessage MoveMessage::Create(const glm::vec2 &vec) {
    MoveMessage msg;
    msg.vec = vec;
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