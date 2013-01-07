﻿// Ŭnicode please
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
#include "laser_layer.h"
#include "game_info_layer.h"
#include "shield_layer.h"
#include "chain_layer.h"
#include "aura_layer.h"

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
: player_(NULL), state_(kGameReadyState) {
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

    ReadyPlayer(CreatePlayer());

    //물리 디버깅용
    PhyDebugLayer *phy_debug_layer = new PhyDebugLayer(world_->b2_world());
    phy_debug_layer->autorelease();
    this->addChild(phy_debug_layer, 100);

    //레이저 레이어
    LaserLayer *laser_layer = new LaserLayer(world_.get());
    laser_layer->init();
    laser_layer->autorelease();
    world_->laser_layer = laser_layer;
    this->addChild(laser_layer);
    
    //게임 정보 레이어
    GameInfoLayer *info_layer = new GameInfoLayer(world_.get());
    info_layer->init();
    info_layer->autorelease();
    //CharacterComponent 얻어서 정보 레이어 초기화
    //그냥 레이어가 플레이어를 가지고 있어도 되지 않을까 싶긴 한데
    CharacterComponent *player_comp = static_cast<CharacterComponent*>(player_->logic_comp());
    info_layer->set_player_max_hit_point(player_comp->max_hit_point());
    info_layer->set_player_hit_point(player_comp->hit_point());
    world_->game_info_layer = info_layer;
    this->addChild(info_layer);

    //쉴드 레이어
    ShieldLayer *shield_layer = new ShieldLayer(world_.get());
    shield_layer->init();
    shield_layer->autorelease();
    world_->shield_layer = shield_layer;
    this->addChild(shield_layer);

    //체인 레이어
    ChainLayer *chain_layer = new ChainLayer(world_.get());
    chain_layer->init();
    chain_layer->autorelease();
    world_->chain_layer = chain_layer;
    this->addChild(chain_layer);

    //오라 레이어
    AuraLayer *aura_layer = new AuraLayer(world_.get());
    aura_layer->init();
    aura_layer->autorelease();
    world_->aura_layer = aura_layer;
    this->addChild(aura_layer);

    return true;
}

void GameLayer::update(float dt) {
    
    //State 패턴 같은걸 쓸 게 있나?


    //TODO
    //스테이지 유효 체크    
    stage_->Update(dt);
    world_->Update(dt);
    

    if(state_ == kGameReadyState) {
        //아직 아무 것도 없음
        
        //위치를 적절히 가져와본다
        b2Vec2 player_pos = player_->phy_comp()->main_body()->GetPosition();
        float length = glm::length(glm::vec2(PLAYER_START_POINT_X, PLAYER_START_POINT_Y) - Unit::ToUnitFromMeter(player_pos));
        
        if(length < 0.3) {
            state_ = kGameProgressState;
            stage_->Start();
        }
    }

    else if(state_ == kGameProgressState) {

        
        //플레이어 죽었나?
        if(!player_->IsEnabled()) {
            cocos2d::CCLog("Player die");
            world_->RequestRemoveObject(world_->FindObject(player_->id()));
            world_->game_info_layer->StopRenderHitPointBar(player_->id());

            //제거와 생성 하다가 2개가 생기는데 하나는 비활성화되니 문제 없을듯
            ReadyPlayer(CreatePlayer());

            //player_ = CreatePlayer();
            //world_->AddObject(player_);
            //set_player(CreatePlayer());
        }

        //게임 클리어?
        if(stage_->IsCleared()) {
            state_ = kGameVictoryState;
            EndStage();
        }
        //게임 오버?
        else if(stage_->IsGameOver()) {
            //일단 끝냄
            state_ = kGameOverState;
            EndStage();
        }
    }

    else if(state_ == kGameVictoryState) {

    }

    else if(state_ == kGameOverState) {

    }

}

GameObject *GameLayer::player() {
    return player_;
}

//이렇게 쓰는거 맞나..?
void GameLayer::ReadyPlayer(GameObject *player) {
    /*
    if(player_ != NULL)
        world_->RequestRemoveObject(world_->FindObject(player_->id()));
    */
    player_ = player;
    world_->AddObject(player_);
    glm::vec2 move_vec(PLAYER_START_POINT_X, PLAYER_START_POINT_Y);
    MoveToMessage msg = MoveToMessage::Create(move_vec, 1);
    player_->OnMessage(&msg);
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
        if(state_ != kGameReadyState)
            MoveBodyByDelta(location.x - prev_location.x, location.y - prev_location.y);
    }
}
void GameLayer::ccTouchesCancelled(CCSet *touches, CCEvent *event) {
}

void GameLayer::MoveBodyByDelta(const float &dx, const float &dy) {
    //TODO
    //플레이어가 없어지면? 어짜피 플레이어 다른데로 옮길거
    
    //메시지 보내기
    //이건 걍 set으로 해야 제대로 될듯?
    PhyBodyInfo body_info;
    b2Body *body = player_->phy_comp()->main_body();
    b2Vec2 vec2 = body->GetPosition();
    body_info.x = vec2.x + Unit::ToMeterFromUnit(dx);
    body_info.y = vec2.y + Unit::ToMeterFromUnit(dy);
    body_info.angle_rad = body->GetAngle();
    SetPhyBodyInfoMessage msg = SetPhyBodyInfoMessage::Create(&body_info);
    
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
    player_header.y = -100;
    player_header.hit_point = 100;
    player_header.sprite_name = "";
    GameObject* init_player = factory_->Create(player_header, stage_->layer());

    return init_player;
}

void GameLayer::EndStage() {
    CCLOG("Game End");
}
