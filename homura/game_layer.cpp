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
: simple_layer_(NULL), player_(NULL) {
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
    factory_ = new GameObjectFactory(world_.get());

    stage_ = new GameStage(world_.get());
    if(!stage_->Init()) {
        return false;
    }

    world_->set_stage(stage_);

    this->addChild(stage_->layer());

    set_player(CreatePlayer());

    //물리 디버깅용
    PhyDebugLayer *phy_debug_layer = new PhyDebugLayer(world_->b2_world());
    phy_debug_layer->autorelease();
    this->addChild(phy_debug_layer, 100);
    
    return true;
}

void GameLayer::update(float dt) {
    
    //TODO
    //스테이지 유효 체크
    stage_->Update(dt);

    //플레이어 죽었나?
    if(!player_->IsEnabled()) {
        cocos2d::CCLog("Player die");
        world_->RequestRemoveObject(world_->FindObject(player_->id()));
        
        //제거와 생성 하다가 2개가 생기는데 하나는 비활성화되니 문제 없을듯
        set_player(CreatePlayer());

        //player_ = CreatePlayer();
        //world_->AddObject(player_);
        //set_player(CreatePlayer());
    }

    //게임 클리어?
    if(stage_->IsCleared()) {
        EndStage();
    }
    //게임 오버?
    else if(stage_->IsGameOver()) {
        //일단 끝냄
        EndStage();
    }

    world_->Update(dt);

}

GameObject *GameLayer::player() {
    return player_;
}

//이렇게 쓰는거 맞나..?
void GameLayer::set_player(GameObject *player) {
    /*
    if(player_ != NULL)
        world_->RequestRemoveObject(world_->FindObject(player_->id()));
    */
    player_ = player;
    world_->AddObject(player_);
}

void GameLayer::ccTouchesEnded(CCSet *touches, CCEvent *event) {

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

void GameLayer::MoveBodyByDelta(const float &dx, const float &dy) {
    //TODO
    //플레이어가 없어지면? 어짜피 플레이어 다른데로 옮길거
    
    //메시지 보내기
    b2Vec2 vec2(dx, dy);
    MoveMessage msg = MoveMessage::Create(vec2);
    player_->OnMessage(&msg);

    /*
    b2Body *player_body = player_->phy_comp()->main_body();
    b2Vec2 player_position = player_body->GetPosition() + b2Vec2(Unit::ToMeterFromUnit(dx), Unit::ToMeterFromUnit(dy));
    player_body->SetTransform(player_position, player_body->GetAngle());
    //계속 awake 꺼지는거 신경 쓰여서 걍 추가해둠. 필요는 없지만 =ㅅ=
    player_body->SetAwake(true);
    */
}

GameObject *GameLayer::CreatePlayer() {
    //플레이어 초기화
    //set stage 함수를 두고 플레이어를 여기서 초기화.
    //플레이어 정보는 여기서 들고 있기 때문.

    PlayerObjectHeader player_header;
    player_header.angle_rad = M_PI_2;
    player_header.x = 350;
    player_header.y = 100;
    player_header.hit_point = 100;
    player_header.sprite_name = "";
    GameObject* init_player = factory_->Create(player_header, stage_->layer());

    return init_player;
}

void GameLayer::EndStage() {
    CCLOG("Stage Clear");
}
