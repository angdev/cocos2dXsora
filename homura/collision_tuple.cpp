// Ŭnicode please
#include "stdafx.h"
#if SR_USE_PCH == 0
#include "Box2D/Box2D.h"
#endif
#include "collision_tuple.h"
#include "game_object.h"
#include "game_world.h"

using namespace std;

CollisionTuple::CollisionTuple(GameObjectPtr &obj_a,
	GameObjectPtr &obj_b, 
	b2Fixture *fixture_a, 
	b2Fixture *fixture_b, 
	const b2WorldManifold &world_manifold,
	GameWorld *world)
	: world_(world), world_manifold_(world_manifold) {
	SR_ASSERT(obj_a.get() != obj_b.get());
	if(obj_a.get() < obj_b.get()) {
		this->obj_a_ = obj_a;
		this->obj_b_ = obj_b;
		this->fixture_a_ = fixture_a;
		this->fixture_b_ = fixture_b;
	} else {
		this->obj_a_ = obj_b;
		this->obj_b_ = obj_a;
		this->fixture_a_ = fixture_b;
		this->fixture_b_ = fixture_a;
	}
}

CollisionTuple::CollisionTuple()
    : fixture_a_(NULL), fixture_b_(NULL), world_(NULL) {
}

bool CollisionTuple::operator==(const CollisionTuple &o) const {
	if(obj_a_ != o.obj_a_) {
		return false;
	}
	if(obj_b_ != o.obj_b_) {
		return false;
	}
	if(fixture_a_ != o.fixture_a_) {
		return false;
	}
	if(fixture_b_ != o.fixture_b_) {
		return false;
	}
	return true;
}

bool CollisionTuple::operator!=(const CollisionTuple &o) const {
	return !(*this == o); 
}

bool CollisionTuple::operator<(const CollisionTuple &o) const {
    int a = reinterpret_cast<int>(obj_a_.get());
    int b = reinterpret_cast<int>(obj_b_.get());
    return a < b;
}

bool CollisionTuple::IsMatch(ObjectType type1, ObjectType type2) const
{
	ObjectType valid_type_table[][2] = {
		{ type1, type2 },
		{ type2, type1 }
	};
	for(int i = 0 ; i < 2 ; i++) {
        //XXX obj의 타입은 일단 제거
        if(obj_a_->Type() == valid_type_table[i][0] && obj_b_->Type() == valid_type_table[i][1]) {
			return true;
		}
	}
	return false;
}

GameObjectPtr CollisionTuple::GetObj(ObjectType type) const {
    if(obj_a_->Type() == type) {
		return obj_a_;
	} else if(obj_b_->Type() == type) {
		return obj_b_;
	}
    static GameObjectPtr empty;
	return empty;
}

GameObjectPtr CollisionTuple::GetOther(GameObject *obj) const
{
	if(obj_a_.get() == obj) {
		return obj_b_;
	} else if(obj_b_.get() == obj) {
		return obj_a_;
	} else {
		return NULL;
	}
}