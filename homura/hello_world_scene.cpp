#include "stdafx.h"
#include "stdafx.h"
#include "hello_world_scene.h"
#include "app_macros.h"

#include <cstdlib>
#include <cstdio>
#include "sora/ani_node.h"
#include "sora/unit.h"

#include "GLES-Render.h"

#include "draw_nodes/CCDrawingPrimitives.h"

USING_NS_CC;
using namespace sora;

HelloWorld::HelloWorld()
: world_(NULL), 
debug_draw_(NULL) {
}

HelloWorld::~HelloWorld() {
    CC_SAFE_DELETE(world_);
    delete(debug_draw_);
}

CCScene* HelloWorld::scene() {
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() ) {
        return false;
    }

    setTouchEnabled(true);
    scheduleUpdate();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
        "CloseNormal.png",
        "CloseSel_ected.png",
        this,
        menu_selector(HelloWorld::menuCloseCallback));

    pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
        origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", kTitleFontSize);

    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
        origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    //CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    //pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    //this->addChild(pSprite, 0);

    InitPhy();

    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool HelloWorld::InitPhy() {
    b2Vec2 gravity;
    //gravity.Set(0.0f, -10.0f);
    gravity.Set(0.0f, 0.0f);
    world_ = new b2World(gravity);

    // Do we want to let bodies sleep?
    world_->SetAllowSleeping(true);

    world_->SetContinuousPhysics(true);

    const float ptm_ratio = kUnitToMeterRatio;
    debug_draw_ = new GLESDebugDraw(ptm_ratio);
    world_->SetDebugDraw(debug_draw_);

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

void HelloWorld::draw() {
    CHECK_GL_ERROR_DEBUG();
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();

#if CC_ENABLE_BOX2D_INTEGRATION
    CCGLProgram *prog =  CCShaderCache::sharedShaderCache()->programForKey(kCCShader_Position_uColor);
    prog->use();
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    world_->DrawDebugData();

    kmGLPopMatrix();
#endif
}

void HelloWorld::update(float dt) {
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

void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *event) {
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
void HelloWorld::ccTouchBegan(CCSet *touches, CCEvent *event) {
}
void HelloWorld::ccTouchMoved(CCSet *touches, CCEvent *event) {
}
void HelloWorld::ccTouchesCancelled(CCSet *touches, CCEvent *event) {
}

void HelloWorld::AddNewBodyAtPosition(const CCPoint &p) {
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