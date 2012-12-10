﻿// Ŭnicode please
#include "stdafx.h"
#include "collision_handler.h"
#include "collision_tuple.h"

#include "game_message.h"
#include "game_object.h"

using namespace std;

CompTypeTuple::CompTypeTuple(CompType a, CompType b)
 : type_a(a), type_b(b) {
     //대소관계로 type의 순서를 뒤집어 버리면 handler 함수 호출할때
     //인자순서가 안맞을수 있다.
}

bool CompTypeTuple::IsMatch_AB(CompType a, CompType b) {
    return (type_a == a && type_b == b);
}

bool CompTypeTuple::IsMatch_BA(CompType a, CompType b) {
    return (type_b == a && type_a == b);
}

bool CompTypeTuple::IsMatch(CompType a, CompType b) {
    return IsMatch_AB(a, b) || IsMatch_BA(a, b);
}

int CompTypeTuple::Hash() const {
    CompType low_type = type_a < type_b ? type_a : type_b;
    CompType high_type = type_a > type_b ? type_a : type_b;
    int hash = high_type * 100 + low_type;
    return hash;
}

bool CompTypeTuple::operator<(const CompTypeTuple &o) const {
    int hash_a = Hash();
    int hash_b = o.Hash();
    return hash_a < hash_b;
}

void CollisionHandler_Object_Object::OnCollision(CollisionTuple &collision) {
    auto comp_type_tuple_list = GetCompTypeTupleList();
    for(auto type_tuple : comp_type_tuple_list) {
        GameObject *obj_a = collision.obj_a().get();
        GameObject *obj_b = collision.obj_b().get();
        
        if(type_tuple.IsMatch_AB(obj_a->Type(), obj_b->Type())) {
            OnCollision(obj_a, obj_b, collision);
        } else if(type_tuple.IsMatch_BA(obj_a->Type(), obj_b->Type())) {
            OnCollision(obj_b, obj_a, collision);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
CollisionHandler_Bullet_PlayerPlane::CollisionHandler_Bullet_PlayerPlane() {
}
CollisionHandler_Bullet_PlayerPlane::~CollisionHandler_Bullet_PlayerPlane() {
}

const std::vector<CompTypeTuple> CollisionHandler_Bullet_PlayerPlane::GetCompTypeTupleList() const {
    static vector<CompTypeTuple> tpl_list;
    if(tpl_list.empty()) {
        tpl_list.push_back(CompTypeTuple(kCompBullet, kCompPlayer));
        tpl_list.push_back(CompTypeTuple(kCompBullet, kCompCombatPlane));
    }
    return tpl_list;
}

void CollisionHandler_Bullet_PlayerPlane::OnCollision(GameObject *bullet, GameObject *etc, CollisionTuple &collision) {
    BulletDamageObjectMessage msg = BulletDamageObjectMessage::Create(etc);
    bullet->OnMessage(&msg);
}

//////////////////////////////////////////////////////////////////////////
CollisionHandler_Player_Plane::CollisionHandler_Player_Plane() {

}
CollisionHandler_Player_Plane::~CollisionHandler_Player_Plane() {

}

const std::vector<CompTypeTuple> CollisionHandler_Player_Plane::GetCompTypeTupleList() const {
    static vector<CompTypeTuple> tpl_list;
    if(tpl_list.empty()) {
        tpl_list.push_back(CompTypeTuple(kCompPlayer, kCompCombatPlane));
    }
    return tpl_list;
}

void CollisionHandler_Player_Plane::OnCollision(GameObject *player, GameObject *etc, CollisionTuple &collision) {
    //박치기 데미지 (충돌 메시지를 만들어놓았지만 일단 바로 데미지 처리)
    DamageObjectMessage msg = DamageObjectMessage::Create(10);
    player->OnMessage(&msg);
    etc->OnMessage(&msg);
}