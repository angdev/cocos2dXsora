// Ŭnicode please
#include "stdafx.h"
#include "game_layer.h"

#include "sora/unit.h"
#include "GLES-Render.h"

using namespace std;
using namespace sora;
using namespace cocos2d;

GameLayer::GameLayer() {
}

GameLayer::~GameLayer() {
}

CCScene *GameLayer::scene() {
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init() {
    if ( !CCLayer::init() ) {
        return false;
    }

    setTouchEnabled(true);
    scheduleUpdate();

    InitPhy();
    return true;
}


bool GameLayer::InitPhy() {
    b2Vec2 gravity;
    //gravity.Set(0.0f, -10.0f);
    gravity.Set(0.0f, 0.0f);
    world_ = std::move(unique_ptr<b2World>(new b2World(gravity)));

    // Do we want to let bodies sleep?
    world_->SetAllowSleeping(true);

    world_->SetContinuousPhysics(true);

    const float ptm_ratio = kUnitToMeterRatio;
    debug_draw_ = std::move(unique_ptr<GLESDebugDraw>(new GLESDebugDraw(ptm_ratio)));
    world_->SetDebugDraw(debug_draw_.get());

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
    b2Body* groundBody = world_->CreateBody(&groundBodyDef);

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

void GameLayer::draw() {
    CHECK_GL_ERROR_DEBUG();
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();

#if CC_ENABLE_BOX2D_INTEGRATION
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    world_->DrawDebugData();

    kmGLPopMatrix();
#endif
}

void GameLayer::update(float dt) {
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;

    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world_->Step(dt, velocityIterations, positionIterations);
}

void GameLayer::ccTouchesEnded(CCSet *touches, CCEvent *event) {
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++)  {
        touch = (CCTouch*)(*it);
        if(!touch) {
            break;
        }
        CCPoint location = touch->getLocation();
        AddNewBodyAtPosition(location);
    }
}

void GameLayer::ccTouchBegan(CCSet *touches, CCEvent *event) {
}
void GameLayer::ccTouchMoved(CCSet *touches, CCEvent *event) {
}
void GameLayer::ccTouchesCancelled(CCSet *touches, CCEvent *event) {
}

void GameLayer::AddNewBodyAtPosition(const CCPoint &p) {
    const float ptm_ratio = kUnitToMeterRatio;
    
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x/ptm_ratio, p.y/ptm_ratio);

    b2Body *body = world_->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(.5f, .5f);//These are mid points for our 1m box
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;    
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
}