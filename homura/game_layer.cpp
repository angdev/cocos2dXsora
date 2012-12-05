// Ŭnicode please
#include "stdafx.h"
#include "game_layer.h"

#include "game_world.h"
#include "game_stage.h"
#include "sora/unit.h"

#include "game_object.h"
#include "drawable_component.h"
#include "phy_component.h"
#include "game_object_factory.h"

#include "character_component.h"

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

    stage_ = new GameStage(world_.get());
    if(!stage_->Init()) {
        return false;
    }
    this->addChild(stage_->layer());

    //물리 디버깅용
    PhyDebugLayer *phy_debug_layer = new PhyDebugLayer(world_->b2_world());
    phy_debug_layer->autorelease();
    this->addChild(phy_debug_layer, 100);
    
    /*
    //플레이어 초기화
    GameObjectFactory factory(world_.get());
    player_ = factory.CreateDemoPlayer(glm::vec2(300, 300), stage_->layer());
    */
    //체력 설정은 임시. (생성시에 초기화할 것)
    //static_cast<CharacterComponent*>(player_->logic_comp())->set_hit_point(100);
    /*
    //AI 테스트용
    GameObject *obj_ai = factory.CreateDemoEnemy(glm::vec2(500, 500), stage_->layer());
    //캐스팅 방식 말고 메시지 방식을 쓰던지 생성할 때만 잘 처리하던지 해야함. 일단은 캐스팅.
    static_cast<CharacterComponent*>(obj_ai->logic_comp())->set_hit_point(100);
    */

    //전투기 테스트
    /*
    GameObject* plane = factory.CreateDemoCombatPlane(glm::vec2(400, 1200), stage_->layer());
    static_cast<CharacterComponent*>(plane->logic_comp())->set_hit_point(100);
    */
    return true;
}

void GameLayer::update(float dt) {
    world_->Update(dt);
    stage_->Update(dt);
}

void GameLayer::ccTouchesEnded(CCSet *touches, CCEvent *event) {
    /*
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
    */
}

void GameLayer::ccTouchesBegan(CCSet *touches, CCEvent *event) {
}
void GameLayer::ccTouchesMoved(CCSet *touches, CCEvent *event) {
    //객체 이동 테스트
    CCSetIterator it;
    CCTouch *touch;
    for( it = touches->begin(); it != touches->end(); it++) {
        touch = static_cast<CCTouch*>(*it);
        if(!touch) {
            break;
        }
        CCPoint location = touch->getLocation();
        CCPoint prev_location = touch->getPreviousLocation();
        //CCLog("%f %f", location.x - prev_location.x, location.y - prev_location.y);
        MoveBodyByDelta(location.x - prev_location.x, location.y - prev_location.y);
    }
}
void GameLayer::ccTouchesCancelled(CCSet *touches, CCEvent *event) {
}

void GameLayer::AddNewBodyAtPosition(const CCPoint &p) {
    glm::vec2 ut_pos(p.x, p.y);
    GameObjectFactory factory(world_.get());
    factory.CreateDemoObj(ut_pos, stage_->layer());
}

void GameLayer::MoveBodyByDelta(const float &dx, const float &dy) {
    //TODO
    //플레이어가 없어지면? 어짜피 플레이어 다른데로 옮길거
    
    //메시지 보내기
    player_->OnMessage(&MoveMessage::Create(glm::vec2(dx, dy)));

    /*
    b2Body *player_body = player_->phy_comp()->main_body();
    b2Vec2 player_position = player_body->GetPosition() + b2Vec2(Unit::ToMeterFromUnit(dx), Unit::ToMeterFromUnit(dy));
    player_body->SetTransform(player_position, player_body->GetAngle());
    //계속 awake 꺼지는거 신경 쓰여서 걍 추가해둠. 필요는 없지만 =ㅅ=
    player_body->SetAwake(true);
    */
}