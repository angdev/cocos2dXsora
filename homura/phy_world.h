// Ŭnicode please
#pragma once

#include "collision_tuple.h"

class GLESDebugDraw;
class GameWorld;

class PhyWorld {
public:
    PhyWorld(GameWorld *game_world);
    ~PhyWorld();

public:
    void Update(float dt);
    void Init();

    // getter / setter
public:
    const b2World *b2_world() const { return b2_world_.get(); }
    b2World *b2_world() { return b2_world_.get(); }

private:
    void InitDebug(b2World *world);
    void InitGround(b2World *world);

    void UpdateCollision();
    std::vector<CollisionTuple> GetCollisionList();
    void HandleCollision(CollisionTuple &collision);

private:
    std::unique_ptr<b2World> b2_world_;
    std::unique_ptr<GLESDebugDraw> debug_draw_;

    GameWorld *game_world_;
};