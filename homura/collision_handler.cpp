// Ŭnicode please
#include "stdafx.h"
#include "collision_handler.h"
#include "collision_tuple.h"

#include "game_message.h"
#include "game_object.h"

#include "combat_plane_component.h"

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
        tpl_list.push_back(CompTypeTuple(kCompBullet, kCompLaserPlane));
        tpl_list.push_back(CompTypeTuple(kCompBullet, kCompGuidedLaserPlane));
        tpl_list.push_back(CompTypeTuple(kCompBullet, kCompShield));
    }
    return tpl_list;
}

void CollisionHandler_Bullet_PlayerPlane::OnCollision(GameObject *bullet, GameObject *etc, CollisionTuple &collision) {
    /*
    BulletDamageObjectMessage msg = BulletDamageObjectMessage::Create(etc);
    bullet->OnMessage(&msg);
    */

    b2WorldManifold manifold = collision.world_manifold();
    //fixtureA -> fixtureB 방향으로 normal vector가 생김
    if(collision.obj_a()->Type() == kCompBullet) {
        //A가 총알이면 normal vector 방향을 바꾼다
        manifold.normal = -manifold.normal;
    }

    CollideBulletMessage msg = CollideBulletMessage::Create(bullet, manifold);
    etc->OnMessage(&msg);

    
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
        tpl_list.push_back(CompTypeTuple(kCompPlayer, kCompLaserPlane));
    }
    return tpl_list;
}

void CollisionHandler_Player_Plane::OnCollision(GameObject *player, GameObject *etc, CollisionTuple &collision) {
    //플레이어로 메시지 보내고 -> 서로 데미지를 입음
    CollidePlaneMessage msg = CollidePlaneMessage::Create(etc);
    player->OnMessage(&msg);
}


//////////////////////////////////////////////////////////////////////////
CollisionHandler_Suicider_Plane::CollisionHandler_Suicider_Plane() {

}

CollisionHandler_Suicider_Plane::~CollisionHandler_Suicider_Plane() {

}

const std::vector<CompTypeTuple> CollisionHandler_Suicider_Plane::GetCompTypeTupleList() const {
    static vector<CompTypeTuple> tpl_list;
    if(tpl_list.empty()) {
        tpl_list.push_back(CompTypeTuple(kCompCombatPlane, kCompCombatPlane));
        tpl_list.push_back(CompTypeTuple(kCompCombatPlane, kCompLaserPlane));
        tpl_list.push_back(CompTypeTuple(kCompCombatPlane, kCompPlayer));
        tpl_list.push_back(CompTypeTuple(kCompCombatPlane, kCompShield));
    }
    return tpl_list;
}

void CollisionHandler_Suicider_Plane::OnCollision(GameObject *suicider, GameObject *etc, CollisionTuple &collision) {
    CombatPlaneComponent *comp = static_cast<CombatPlaneComponent*>(suicider->logic_comp());
    if(!comp->suicide_flag()) {
        return;
    }
    comp->Destroy();
    //핸들링을 자폭기로 넘겨야하지만 일단 여기서 처리해보자
    DamageObjectMessage msg = DamageObjectMessage::Create(30);
    etc->OnMessage(&msg);
}
