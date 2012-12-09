// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include <Box2D/Box2D.h>
#endif

class GameObject;
class GameWorld;
class b2Fixture;

class CollisionTuple {
public:
	CollisionTuple(GameObjectPtr &obj_a, 
		GameObjectPtr &obj_b,
		b2Fixture *fixture_a, 
		b2Fixture *fixture_b,
		const b2WorldManifold &world_manifold,
		GameWorld *world);

	CollisionTuple();

public:
    bool operator==(const CollisionTuple &o) const;
	bool operator!=(const CollisionTuple &o) const;
	bool operator<(const CollisionTuple &o) const;

	GameObjectPtr GetObj(ObjectType type) const;

    bool IsMatch(ObjectType type1, ObjectType type2) const;
	GameObjectPtr GetOther(GameObject *obj) const;

    GameObjectPtr obj_a() { return obj_a_; }
    GameObjectPtr obj_b() { return obj_b_; }

    b2WorldManifold world_manifold() { return world_manifold_; }

private:
	GameObjectPtr obj_a_;
	GameObjectPtr obj_b_;
	b2Fixture *fixture_a_;
	b2Fixture *fixture_b_;
	b2WorldManifold world_manifold_;

	GameWorld *world_;	
};