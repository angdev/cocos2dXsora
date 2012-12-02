// Ŭnicode please
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
private:
    typedef std::multimap<ObjectType, GameObjectPtr> GameObjectTable;

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

    GameObjectPtr FindObject(int id);
    GameObjectPtr FindObject(int id, ObjectType type);
    bool IsExist(int id) const;
    bool IsExist(int id, ObjectType type) const;

    bool RemoveObject(GameObjectPtr obj);
    bool RequestRemoveObject(GameObjectPtr obj);

    template<typename Functor, typename Iter>
    GameObjectPtr FindObject(Iter begin, Iter end, Functor func);
    template<typename Functor, typename Iter>
    bool IsExist(Iter begin, Iter end, Functor func) const;

private:
    typedef std::list<DelayedGameMessagePtr> DelayedMessageListType;
    DelayedMessageListType delayed_msg_list_;

    std::vector<GameObjectPtr> request_remove_list_;

private:
    GameObjectTable obj_table_;

    //box2d 관련 내용
public:
    b2World *b2_world();
    PhyWorld *phy_world() { return phy_world_.get(); }
private:
    std::unique_ptr<PhyWorld> phy_world_;
};


template<typename Functor, typename Iter>
GameObjectPtr GameWorld::FindObject(Iter begin, Iter end, Functor func) {
    auto found = std::find_if(begin, end, func);
    if(found != end) {
        return found->second;
    } else {
        static GameObjectPtr empty;
        return empty;
    }
}
template<typename Functor, typename Iter>
bool GameWorld::IsExist(Iter begin, Iter end, Functor func) const {
    auto found = std::find_if(begin, end, func);
    return (found != end);
}