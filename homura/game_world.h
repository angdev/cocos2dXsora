﻿// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include "game_globals.h"
#include <list>
#include <map>
#include <memory>
#include <vector>
#endif

struct GameMessage;
struct DelayedGameMessage;
class GameObject;
class GameObjectFactory;
class PhyWorld;

typedef std::shared_ptr<GameObject> GameObjectPtr;
typedef std::shared_ptr<DelayedGameMessage> DelayedGameMessagePtr;

class GameWorld {
public:
    GameWorld();
    ~GameWorld();

public:
    void Update(float dt);
    void OnMessage(GameMessage *msg);
    void OnMessage(GameMessage *msg, float delay);

public:
    //GameObject Handling
    //id를 리턴함.
    int AddObject(GameObject *obj, ObjectType type);
    GameObject* GetObject(const int &id);
    
private:
    typedef std::list<DelayedGameMessagePtr> DelayedMessageListType;
    DelayedMessageListType delayed_msg_list_;

private:
    typedef std::map<ObjectType, std::vector<GameObjectPtr> > GameObjectTable;
    GameObjectTable game_object_table_;

    //box2d 관련 내용
public:
    b2World *b2_world();
    PhyWorld *phy_world() { return phy_world_.get(); }
private:
    std::unique_ptr<PhyWorld> phy_world_;
};
