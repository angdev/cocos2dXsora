// Ŭnicode please
#include "stdafx.h"
#include "phy_world.h"

#include "sora/unit.h"
#include "GLES-Render.h"
#include "collision_tuple.h"
#include "game_object.h"
#include "game_world.h"

#include "collision_manager.h"

//temp
#include "character_component.h"

using namespace std;
using namespace sora;
using namespace cocos2d;

PhyWorld::PhyWorld(GameWorld *game_world)
: game_world_(game_world) {
    Init();
}

PhyWorld::~PhyWorld() {

}

void PhyWorld::Init() {
    b2Vec2 gravity;
    //어차피 충돌처리만 사용할거니까 중력은 필요없다
    gravity.Set(0.0f, 0.0f);
    b2_world_ = std::move(unique_ptr<b2World>(new b2World(gravity)));
    // Do we want to let bodies sleep?
    b2_world_->SetAllowSleeping(true);
    b2_world_->SetContinuousPhysics(true);

    InitDebug(b2_world());
    //InitGround(b2_world());
    
    collision_mgr_.reset(new CollisionManager(game_world_, b2_world_.get()));
    contact_listener_.reset(new ContactListener());
    b2_world_->SetContactListener(contact_listener_.get());
}

void PhyWorld::InitDebug(b2World *world) {
    const float ptm_ratio = kUnitToMeterRatio;
    debug_draw_ = std::move(unique_ptr<GLESDebugDraw>(new GLESDebugDraw(ptm_ratio)));
    world->SetDebugDraw(debug_draw_.get());

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    debug_draw_->SetFlags(flags);

}

void PhyWorld::InitGround(b2World *world) {
    const float ptm_ratio = kUnitToMeterRatio;

     // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

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
}

void PhyWorld::Update(float dt) {
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8; //8
    int positionIterations = 4; //1

    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    b2_world_->Step(dt, velocityIterations, positionIterations);

    collision_mgr_->Update();
}
