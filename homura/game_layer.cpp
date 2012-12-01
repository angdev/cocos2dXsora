// Ŭnicode please
#include "stdafx.h"
#include "game_layer.h"

#include "game_world.h"
#include "sora/unit.h"

using namespace std;
using namespace sora;
using namespace cocos2d;

GameLayer::GameLayer() {
}

GameLayer::~GameLayer() {
}

CCScene *GameLayer::scene() {
    CCScene *scene = CCScene::create();
    scene->setTag(kSceneGame);
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

    world_ = std::move(unique_ptr<GameWorld>(new GameWorld()));

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
    world_->b2_world()->DrawDebugData();
    kmGLPopMatrix();
#endif
}

void GameLayer::update(float dt) {
    world_->Update(dt);
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

    b2World *b2_world = world_->b2_world();
    b2Body *body = b2_world->CreateBody(&bodyDef);
    
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