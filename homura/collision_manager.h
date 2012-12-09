// Ŭnicode please
#pragma once

#include "collision_tuple.h"
#include "collision_handler.h"

class CollisionHandler_Object_Object;

class CollisionManager {
public:
    CollisionManager(GameWorld *world, b2World *b2_world);
    ~CollisionManager();

    void Update();
    void AddHandler(CollisionHandler_Object_Object *handler);

private:
    void UpdateCollision();
    std::vector<CollisionTuple> GetCollisionList();
    void HandleCollision(CollisionTuple &collision);

private:
    GameWorld *game_world_;
    b2World *b2_world_;

    std::vector<CollisionHandler_Object_Object*> obj_handler_list_;
    std::multimap<CompTypeTuple, CollisionHandler_Object_Object*> obj_handler_map_;
};