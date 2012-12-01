// Ŭnicode please
#include "stdafx.h"
#include "game_world.h"
#include "game_object.h"
#include "game_component.h"
#include "game_message.h"

#include "sora/unit.h"
#include "GLES-Render.h"

#if SR_USE_PCH == 0
#include <algorithm>
#endif

using namespace std;
using namespace sora;
using namespace cocos2d;

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
    InitPhysics();
}

GameWorld::~GameWorld() {
}

void GameWorld::Update(float dt) {
    //TODO
    UpdatePhysics(dt);

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

int GameWorld::AddObject(GameObject *obj, ObjectType type) {
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

bool GameWorld::InitPhysics() {
    b2Vec2 gravity;
    //gravity.Set(0.0f, -10.0f);
    gravity.Set(0.0f, 0.0f);
    b2_world_ = std::move(unique_ptr<b2World>(new b2World(gravity)));

    // Do we want to let bodies sleep?
    b2_world_->SetAllowSleeping(true);

    b2_world_->SetContinuousPhysics(true);

    const float ptm_ratio = kUnitToMeterRatio;
    debug_draw_ = std::move(unique_ptr<GLESDebugDraw>(new GLESDebugDraw(ptm_ratio)));
    b2_world_->SetDebugDraw(debug_draw_.get());

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    debug_draw_->SetFlags(flags);


    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = b2_world_->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    float bottom = 0;
    float top = visibleSize.height;
    float left = 0;
    float right = visibleSize.width;

    // bottom
    groundBox.Set(b2Vec2(left/ptm_ratio, bottom/ptm_ratio), b2Vec2(right/ptm_ratio, bottom/ptm_ratio));
    groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(left/ptm_ratio, top/ptm_ratio), b2Vec2(right /ptm_ratio, top/ptm_ratio));
    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(left/ptm_ratio, top/ptm_ratio), b2Vec2(left/ptm_ratio, bottom/ptm_ratio));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(right/ptm_ratio, bottom/ptm_ratio), b2Vec2(right/ptm_ratio, top/ptm_ratio));
    groundBody->CreateFixture(&groundBox,0);

    return true;
}

void GameWorld::UpdatePhysics(float dt) {
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 0; //8
    int positionIterations = 4; //1

    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    b2_world_->Step(dt, velocityIterations, positionIterations);
}