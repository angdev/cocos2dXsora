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
	: world_(world), world_manifold(world_manifold) {
	SR_ASSERT(obj_a.get() != obj_b.get());
	if(obj_a.get() < obj_b.get()) {
		this->obj_a = obj_a;
		this->obj_b = obj_b;
		this->fixture_a = fixture_a;
		this->fixture_b = fixture_b;
	} else {
		this->obj_a = obj_b;
		this->obj_b = obj_a;
		this->fixture_a = fixture_b;
		this->fixture_b = fixture_a;
	}
}

bool CollisionTuple::operator==(const CollisionTuple &o) const {
	if(obj_a != o.obj_a) {
		return false;
	}
	if(obj_b != o.obj_b) {
		return false;
	}
	if(fixture_a != o.fixture_a) {
		return false;
	}
	if(fixture_b != o.fixture_b) {
		return false;
	}
	return true;
}

bool CollisionTuple::operator!=(const CollisionTuple &o) const {
	return !(*this == o); 
}

bool CollisionTuple::operator<(const CollisionTuple &o) const {
    int a = reinterpret_cast<int>(obj_a.get());
    int b = reinterpret_cast<int>(obj_b.get());
    return a < b;
}

/*
bool CollisionTuple::IsMatch(int type1, int type2) const
{
	const GameObjectPtr &obj1 = obj_a;
	const GameObjectPtr &obj2 = obj_b ;

	int valid_type_table[][2] = {
		{ type1, type2 },
		{ type2, type1 }
	};
	for(int i = 0 ; i < 2 ; i++) {
        //XXX obj의 타입은 일단 제거
		//if(obj1->type() == valid_type_table[i][0] && obj2->type() == valid_type_table[i][1]) {
		//	return true;
		//}
	}
	return false;
}
*/

/*
GameObjectPtr CollisionTuple::GetObj(int type) const {
	if(obj_a->type() == type) {
		return obj_a;
	} else if(obj_b->type() == type) {
		return obj_b;
	}
    static GameObjectPtr empty;
	return empty;
}
*/

GameObjectPtr CollisionTuple::GetOther(GameObject *obj) const
{
	if(obj_a.get() == obj) {
		return obj_b;
	} else if(obj_b.get() == obj) {
		return obj_a;
	} else {
		return NULL;
	}
}