// Ŭnicode please
#pragma once

class NullComponent;
class CollisionTuple;

struct CompTypeTuple {
    CompTypeTuple() : type_a(kCompNull), type_b(kCompNull) {}
    CompTypeTuple(CompType a, CompType b);
    CompType type_a;
    CompType type_b;

    bool IsMatch_AB(CompType a, CompType b);
    bool IsMatch_BA(CompType a, CompType b);
    bool IsMatch(CompType a, CompType b);

    bool operator<(const CompTypeTuple &o) const;

    int Hash() const;
};

class CollisionHandler {
public:
    CollisionHandler() {}
    virtual ~CollisionHandler() {}
};

class CollisionHandler_Object_Object {
public:
    CollisionHandler_Object_Object() {}
    virtual ~CollisionHandler_Object_Object() {}
    
    virtual const std::vector<CompTypeTuple> GetCompTypeTupleList() const = 0;

    void OnCollision(CollisionTuple &collision);
    virtual void OnCollision(GameObject *a, GameObject *b, CollisionTuple &collision) = 0;
};

/* game object-game object 단위의 충돌 처리 */
class CollisionHandler_Bullet_PlayerPlane : public CollisionHandler_Object_Object {
public:
    CollisionHandler_Bullet_PlayerPlane();
    virtual ~CollisionHandler_Bullet_PlayerPlane();
    
    virtual const std::vector<CompTypeTuple> GetCompTypeTupleList() const;
    virtual void OnCollision(GameObject *bullet, GameObject *etc, CollisionTuple &collision);
};

//플레이어와 기체간의 충돌 처리
class CollisionHandler_Player_Plane : public CollisionHandler_Object_Object {
public:
    CollisionHandler_Player_Plane();
    virtual ~CollisionHandler_Player_Plane();

    virtual const std::vector<CompTypeTuple> GetCompTypeTupleList() const;
    virtual void OnCollision(GameObject *player, GameObject *etc, CollisionTuple &collision);
};

//자폭기와 다른 기체들간의 충돌 처리
class CollisionHandler_Suicider_Plane : public CollisionHandler_Object_Object {
public:
    CollisionHandler_Suicider_Plane();
    virtual ~CollisionHandler_Suicider_Plane();

    virtual const std::vector<CompTypeTuple> GetCompTypeTupleList() const;
    virtual void OnCollision(GameObject *suicider, GameObject *etc, CollisionTuple &collision);
};
