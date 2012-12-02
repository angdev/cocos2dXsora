// Ŭnicode please
#include "stdafx.h"
#include "game_layer.h"

#include "game_world.h"
#include "sora/unit.h"

#include "game_object.h"
#include "drawable_component.h"
#include "phy_component.h"
#include "game_object_factory.h"

using namespace std;
using namespace sora;
using namespace cocos2d;

class PhyDebugLayer : public cocos2d::CCLayer {
public:
    PhyDebugLayer(b2World *world) : world_(world) {}
    virtual ~PhyDebugLayer() {}
    virtual bool init() {
        if(!CCLayer::init()) {
            return false;
        }
        return true;
    }
    virtual void draw() {
        //
        // IMPORTANT:
        // This is only for debug purposes
        // It is recommend to disable it
        //
        #if CC_ENABLE_BOX2D_INTEGRATION
            CHECK_GL_ERROR_DEBUG();
            ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
            kmGLPushMatrix();
            world_->DrawDebugData();
            kmGLPopMatrix();
            CHECK_GL_ERROR_DEBUG();
        #endif
    }
private:
    b2World *world_;
};

GameLayer::GameLayer()
: simple_layer_(NULL) {
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

    simple_layer_ = CCLayer::create();
    this->addChild(simple_layer_);

    //물리 디버깅용
    PhyDebugLayer *phy_debug_layer = new PhyDebugLayer(world_->b2_world());
    phy_debug_layer->autorelease();
    this->addChild(phy_debug_layer, 100);


    return true;
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
    glm::vec2 ut_pos(p.x, p.y);
    GameObjectFactory factory(world_.get());
    factory.CreateDemoObj(ut_pos, simple_layer_);
}