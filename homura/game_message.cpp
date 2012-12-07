// Ŭnicode please
#include "stdafx.h"
#include "game_message.h"

// 메세지 구현체가 생기면 여기에 하나씩 추가하기


RequestRecoveryMessage RequestRecoveryMessage::Create(CharacterComponent *char_comp) {
    RequestRecoveryMessage msg;
    msg.char_comp = char_comp;
    return msg;
}


RequestPlayerPositionMessage RequestPlayerPositionMessage::Create(b2Vec2 *position) {
    RequestPlayerPositionMessage msg;
    msg.position = position;
    msg.is_ret = false;
    return msg;
}

SetAngleMessage SetAngleMessage::Create(float angle) {
    SetAngleMessage msg;
    msg.angle = angle;
    return msg;
}

BeginEventMessage BeginEventMessage::Create(int event_id) {
    BeginEventMessage msg;
    msg.event_id = event_id;
    return msg;
}

BoundCheckMessage BoundCheckMessage::Create(cocos2d::CCSize size) {
    BoundCheckMessage msg;
    msg.window_size = size;
    return msg;
}

RequestPhyBodyInfoMessage RequestPhyBodyInfoMessage::Create(PhyBodyInfo *info) {
    RequestPhyBodyInfoMessage msg;
    msg.phy_body_info = info;
    msg.is_ret = false;
    return msg;
}

DamageObjectMessage DamageObjectMessage::Create(GameObject *obj) {
    DamageObjectMessage msg;
    msg.obj = obj;
    return msg;
}

CollideBulletMessage CollideBulletMessage::Create(GameObject *bullet, float damage, bool from_enemy) {
    CollideBulletMessage msg;
    msg.bullet = bullet;
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