// Ŭnicode please
#include "stdafx.h"
#include "game_message.h"

// 메세지 구현체가 생기면 여기에 하나씩 추가하기


StopRenderLaserMessage StopRenderLaserMessage::Create(int id) {
    StopRenderLaserMessage msg;
    msg.id = id;
    return msg;
}

RequestRenderLaserMessage RequestRenderLaserMessage::Create(int id, const glm::vec2 &start_point, const glm::vec2 &end_point) {
    RequestRenderLaserMessage msg;
    msg.id = id;
    msg.start_point = start_point;
    msg.end_point = end_point;
    return msg;
}

ResetMessage ResetMessage::Create() {
    ResetMessage msg;
    return msg;
}


AttackMessage AttackMessage::Create(int target_id) {
    AttackMessage msg;
    msg.target_id = target_id;
    return msg;
}


IsEnemyMessage IsEnemyMessage::Create() {
    IsEnemyMessage msg;
    msg.is_enemy = false;
    msg.is_ret = false;
    return msg;
}


CheckConnectedChainMessage CheckConnectedChainMessage::Create(int id) {
    CheckConnectedChainMessage msg;
    msg.id = id;
    msg.checked = false;
    return msg;
}


RequestJoinFormationMessage RequestJoinFormationMessage::Create(int id){
    RequestJoinFormationMessage msg;
    msg.id = id;
    return msg;
}


CheckForcesNumberMessage CheckForcesNumberMessage::Create(bool is_enemy) {
    CheckForcesNumberMessage msg;
    msg.is_enemy = is_enemy;
    msg.forces_number = 0;
    return msg;
}

CreateShieldMessage CreateShieldMessage::Create(bool from_enemy) {
    CreateShieldMessage msg;
    msg.from_enemy = from_enemy;
    return msg;
}

SetPhyBodyInfoMessage SetPhyBodyInfoMessage::Create(PhyBodyInfo *info) {
    SetPhyBodyInfoMessage msg;
    msg.info = info;
    return msg;
}

DamageObjectMessage DamageObjectMessage::Create(float damage) {
    DamageObjectMessage msg;
    msg.damage = damage;
    return msg;
}


CollidePlaneMessage CollidePlaneMessage::Create(GameObject *counter_obj) {
    CollidePlaneMessage msg;
    msg.counter_obj = counter_obj;
    return msg;
}


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

BeginTriggerMessage BeginTriggerMessage::Create(int trigger_id) {
    BeginTriggerMessage msg;
    msg.trigger_id = trigger_id;
    return msg;
}

OutOfBoundMessage OutOfBoundMessage::Create(const b2Vec2 &prev_pos, const b2Vec2 &current_pos) {
    OutOfBoundMessage msg;
    msg.prev_pos = prev_pos;
    msg.current_pos = current_pos;
    return msg;
}

RequestPhyBodyInfoMessage RequestPhyBodyInfoMessage::Create(PhyBodyInfo *info) {
    RequestPhyBodyInfoMessage msg;
    msg.phy_body_info = info;
    msg.is_ret = false;
    return msg;
}

BulletDamageObjectMessage BulletDamageObjectMessage::Create(GameObject *obj) {
    BulletDamageObjectMessage msg;
    msg.obj = obj;
    return msg;
}

CollideBulletMessage CollideBulletMessage::Create(GameObject *bullet, const b2WorldManifold &manifold) {
    CollideBulletMessage msg;
    msg.bullet = bullet;
    msg.manifold = manifold;
    return msg;
}


MoveByMessage MoveByMessage::Create( const glm::vec2 &vec, float duration ) {
    MoveByMessage msg;
    msg.vec = vec;
    msg.duration = duration;
    return msg;
}


MoveToMessage MoveToMessage::Create( const glm::vec2 &vec, float duration ) {
    MoveToMessage msg;
    msg.vec = vec;
    msg.duration = duration;
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

