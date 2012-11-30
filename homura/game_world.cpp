// Ŭnicode please
#include "stdafx.h"
#include "game_world.h"
#include "game_object.h"
#include "game_component.h"
#include "game_message.h"

template<typename ObjectPtr>
struct ObjectIdCompare {
    ObjectIdCompare(int id) : id(id) {}
    int id;
    bool operator()(const ObjectPtr &obj) { return (obj->id() == id); }
};

template<typename ObjectPtr>
struct ObjectNameCompare {
    ObjectNameCompare(const std::string &name) : name(name) {}
    std::string name;
    bool operator()(const ObjectPtr &obj) { return (obj->name() == name); }
};

GameWorld::GameWorld() {
}

GameWorld::~GameWorld() {
}

void GameWorld::Update(float dt) {
    //TODO

    //게임 오브젝트 돌면서 Update 호출
    for(auto table_iter : game_object_table_) {
        for(auto vec_iter : table_iter.second) {
            //컴포넌트 업데이트
            vec_iter->Update(dt);
        }
    }
}

void GameWorld::OnMessage(GameMessage *msg) {
    //TODO
    for(auto table_iter : game_object_table_) {
        for(auto vec_iter : table_iter.second) {
            //컴포넌트 업데이트
            vec_iter->OnMessage(msg);
        }
    }
}

void GameWorld::OnMessage(GameMessage *msg, float delay) {
    //msg를 상속을 기반으로 만들엇기떄문에 큐를 거치기 위해서는 clone이 가능해야한다
    DelayedGameMessagePtr msg_tuple(new DelayedGameMessage(msg->Clone(), delay));
    delayed_msg_list_.push_back(msg_tuple);
}

int GameWorld::AddObject(GameObject *obj, kObjectType type) {
    //같은 id를 가진 객체가 있는지 확인
    auto iter_begin = game_object_table_[type].begin();
    auto iter_end = game_object_table_[type].end();
    auto find_iter = std::find_if(iter_begin, iter_end,
        [&obj](GameObjectPtr &ptr) {
        if(obj->id() == ptr->id())
            return true;
        return false;
    });

    if(find_iter != game_object_table_[type].end())
        return -1;
    
    game_object_table_[type].push_back(GameObjectPtr(obj));
    return obj->id();
}

GameObject* GameWorld::GetObject(const int &id) {
    
    for(auto table_iter : game_object_table_) {
        for(auto vec_iter : table_iter.second) {
            if(vec_iter->id() == id)
                return vec_iter.get();
        }
    }

    return 0;
}
