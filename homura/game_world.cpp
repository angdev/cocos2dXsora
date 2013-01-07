// Ŭnicode please
#include "stdafx.h"
#include "game_world.h"
#include "game_object.h"
#include "game_component.h"
#include "game_message.h"
#include "phy_world.h"

#include "laser_layer.h"
#include "game_info_layer.h"
#include "shield_layer.h"
#include "chain_layer.h"

#if SR_USE_PCH == 0
#include <algorithm>
#include "cocos2d.h"
#endif

using namespace std;
using namespace cocos2d;

GameWorld::GameWorld()
    : laser_layer(nullptr), game_info_layer(nullptr), shield_layer(nullptr), chain_layer(nullptr), aura_layer(nullptr) {
    phy_world_ = std::move(unique_ptr<PhyWorld>(new PhyWorld(this)));
}

GameWorld::~GameWorld() {
    obj_table_.clear();
}

void GameWorld::Update(float dt) {
    //TODO
    phy_world_->Update(dt);

    //TODO
    //게임 오브젝트 돌면서 Update 호출. 우선순위 통제 필요하면 적절히 처리하기
    for(auto it : obj_table_) {
        if(it.second->IsEnabled())
            it.second->Update(dt);
    }

    //지연된 객체 삭제 처리
    for(auto obj : request_remove_list_) {
        RemoveObject(obj);
    }
    request_remove_list_.clear();
}

void GameWorld::OnMessage(GameMessage *msg) {
    //TODO
    for(auto it : obj_table_) {
        it.second->OnMessage(msg);
    }
    

    //TODO: 레이어들 묶어서 관리합시다
    //레이어에도 메세지 알리기
    if(laser_layer != nullptr) {
        laser_layer->OnMessage(msg);
    }

    if(game_info_layer != nullptr) {
        game_info_layer->OnMessage(msg);
    }

    if(shield_layer != nullptr) {
        shield_layer->OnMessage(msg);
    }

    if(chain_layer != nullptr) {
        chain_layer->OnMessage(msg);
    }
    
}

void GameWorld::OnMessage(GameMessage *msg, float delay) {
    //msg를 상속을 기반으로 만들엇기떄문에 큐를 거치기 위해서는 clone이 가능해야한다
    DelayedGameMessagePtr msg_tuple(new DelayedGameMessage(msg->Clone(), delay));
    delayed_msg_list_.push_back(msg_tuple);
}

int GameWorld::AddObject(GameObject *obj, ObjectType type) {
    //같은 id를 가진 객체가 있는지 확인
    if(IsExist(obj->id()) == true) {
        return 0;
    }

    obj_table_.insert(make_pair(type, GameObjectPtr(obj)));
    return obj->id();
}

int GameWorld::AddObject(GameObject *obj) {
    return AddObject(obj, obj->Type());
}

GameObjectPtr GameWorld::FindObject(int id, ObjectType type) {
    auto range_ret = obj_table_.equal_range(type);
    auto begin = range_ret.first;
    auto end = range_ret.second;
    return FindObject(begin, end, 
        [&](GameObjectTable::iterator::value_type o) { return o.second->id() == id; });
}

GameObjectPtr GameWorld::FindObject(int id) {
    //같은 id를 가진 객체가 있는지 확인
    auto end = obj_table_.end();
    return FindObject(obj_table_.begin(), end, 
        [&](GameObjectTable::iterator::value_type o) { return o.second->id() == id; });
}

b2World *GameWorld::b2_world() {
    return phy_world_->b2_world();
}

bool GameWorld::IsExist(int id) const {
    auto end = obj_table_.end();
    return IsExist(obj_table_.begin(), end, 
        [&](GameObjectTable::iterator::value_type o) { return o.second->id() == id; });
}
bool GameWorld::IsExist(int id, ObjectType type) const {
    auto range_ret = obj_table_.equal_range(type);
    auto begin = range_ret.first;
    auto end = range_ret.second;
    return IsExist(begin, end, 
        [&](GameObjectTable::iterator::value_type o) { return o.second->id() == id; });
}

bool GameWorld::RemoveObject(GameObjectPtr obj) {
    auto type = obj->Type();
    auto range_ret = obj_table_.equal_range(type);
    auto begin = range_ret.first;
    auto end = range_ret.second;
    auto found = std::find_if(begin, end, 
        [&](GameObjectTable::iterator::value_type o) { return o.second == obj; });
    if(found == end) {
        return false;
    }
    obj_table_.erase(found);
    return true;
}
bool GameWorld::RequestRemoveObject(GameObjectPtr obj) {
    auto found = std::find(request_remove_list_.begin(), request_remove_list_.end(), obj);
    if(found == request_remove_list_.end()) {
        request_remove_list_.push_back(obj);
        return true;
    } else {
        return false;
    }
}
