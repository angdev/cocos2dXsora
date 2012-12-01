// Ŭnicode please
#pragma once

class GameObject;
class GameWorld;
class b2Fixture;
typedef std::shared_ptr<GameObject> GameObjectPtr;

struct CollisionTuple {
	CollisionTuple(GameObjectPtr &obj_a, 
		GameObjectPtr &obj_b,
		b2Fixture *fixture_a, 
		b2Fixture *fixture_b,
		const b2WorldManifold &world_manifold,
		GameWorld *world);
	CollisionTuple()
		: fixture_a(NULL), fixture_b(NULL), world_(NULL) {}
	
	GameObjectPtr obj_a;
	GameObjectPtr obj_b;
	b2Fixture *fixture_a;
	b2Fixture *fixture_b;
	b2WorldManifold world_manifold;

	bool operator==(const CollisionTuple &o) const;
	bool operator!=(const CollisionTuple &o) const;
	//bool operator<(const CollisionTuple &o) const;

	//GameObjectPtr GetObj(int type) const;

	//bool IsMatch(int type1, int type2) const;
	GameObject *GetOther(GameObject *obj) const;

private:
	GameWorld *world_;
};