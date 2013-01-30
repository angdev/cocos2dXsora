// Ŭnicode please
#include "stdafx.h"
#include "game_stage.h"
#include "game_trigger.h"
#include "game_condition.h"
#include "game_trigger_handler.h"
#include "game_trigger_component.h"
#include "game_layer.h"
#include "game_action.h"

#include "game_object_factory.h"

#include "CCParallaxScrollNode.h"

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

USING_NS_CC;

GameStage::GameStage(GameWorld *world)
    : world_(world), elapsed_time_(0), layer_(0), is_cleared_(false), is_game_over_(false) {

}

GameStage::~GameStage() {

}

bool GameStage::Init() {
    layer_ = CCLayer::create();

    if(NULL == layer_)
        return false;

    bullet0_node_ = CCSpriteBatchNode::create("bullet0.png");
    layer_->addChild(bullet0_node_);

    //parallax 초기화
    //테스트 중
    parallax_ = CCParallaxScrollNode::create();
    parallax_->retain();
    //아 변환해야지..
    CCSprite *background_sprite1 = CCSprite::create("background.png");
    CCSprite *background_sprite2 = CCSprite::create("background.png");
    parallax_->addInfiniteScrollYWithZ(0, ccp(1, 1), ccp(0, 0), background_sprite1, background_sprite2, NULL);
    //추가하지 말고 게임 레이어에서 그리도록 일단 변경
    //layer_->addChild(parallax_);

    factory_ = new GameObjectFactory(world_);
    //편대 로직을 구현한 객체 삽입
    FormationHeader formation_header;
    GameObject *formation_obj = factory_->Create(formation_header);
    world_->AddObject(formation_obj);

    
    //////////////////////////////////////////////////////////////////////////
    //여기서부터 스테이지 구성 시작
    //////////////////////////////////////////////////////////////////////////

    int current_id = 0;
    //2
    EnemyCombatPlaneObjectHeader e0;
    CruiserPlaneObjectHeader c0;
    DeadstarPlaneObjectHeader d0;
    AllyPlaneObjectHeader a0;
    GameTriggerObjectHeader t0;

    MakeObjectHeader(a0, 100, -100, 100, 200);
    world_->AddObject(MakeCreateObjectTriggerObject(a0, 0, current_id++));

    MakeObjectHeader(a0, 800, -100, 650, 200);
    world_->AddObject(MakeCreateObjectTriggerObject(a0, 0.5, current_id++));

    MakeObjectHeader(e0, -240, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));
    /*
    //3
    MakeObjectHeader(e0, 120, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //4
    MakeObjectHeader(e0, 120, 1466, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //5
    MakeObjectHeader(e0, -240, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //6
    MakeObjectHeader(e0, 120, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 1, current_id++));

    //7
    MakeObjectHeader(e0, 600, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //8
    MakeObjectHeader(e0, 960, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));
    
    //9
    MakeObjectHeader(e0, 960, 1066, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //10
    MakeObjectHeader(e0, 600, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //11
    MakeObjectHeader(e0, 120, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 5, current_id++));

    //12
    MakeObjectHeader(c0, 600, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 0, current_id++, true));

    //13
    MakeObjectHeader(c0, 120, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 1, current_id++, true));

    //14
    MakeObjectHeader(e0, 120, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //15
    MakeObjectHeader(e0, 120, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //16
    MakeObjectHeader(e0, -240, 640, 120, 640);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //17
    MakeObjectHeader(e0, 960, 640, 600, 600);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //18
    MakeObjectHeader(d0, -240, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(d0, 0, current_id++, true));

    //19
    MakeObjectHeader(d0, 960, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(d0, 3, current_id++, true));

    //20
    MakeObjectHeader(e0, 120, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0, current_id++));

    //21
    MakeObjectHeader(e0, -240, 1066, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //22
    MakeObjectHeader(e0, 960, 1066, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0, current_id++));

    //23
    MakeObjectHeader(e0, 600, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 4.5, current_id++));

    //24
    MakeObjectHeader(e0, -240, 640, 120, 640);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0, current_id++));

    //25
    MakeObjectHeader(e0, -240, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0, current_id++));

    //26
    MakeObjectHeader(e0, -360, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0, current_id++));

    //27
    MakeObjectHeader(e0, 960, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0, current_id++));

    //28
    MakeObjectHeader(e0, 960, 640, 600, 640);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 2, current_id++));

    //29
    MakeObjectHeader(c0, 120, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 3, current_id++, true));

    //30
    MakeObjectHeader(c0, 960, 1066, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 3, current_id++));

    //31
    MakeObjectHeader(c0, 240, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 3, current_id++, true));

    //32
    MakeObjectHeader(c0, 600, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 8, current_id++));

    //33
    MakeObjectHeader(d0, -240, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(d0, 0, current_id++));

    //34
    MakeObjectHeader(d0, 960, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(d0, 5, current_id++));

    //35
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //36
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //37
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //38
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //39
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 3, current_id++));

    //40
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //41
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //42
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //43
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));

    //44
    MakeObjectHeader(c0, 120, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 0, current_id++));

    //45
    MakeObjectHeader(d0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(d0, 0, current_id++, true));

    //46
    MakeObjectHeader(c0, 960, 1066, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 1, current_id++));

    //47
    MakeObjectHeader(e0, -240, 640, 120, 640);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0, current_id++));

    //48
    MakeObjectHeader(e0, 960, 640, 600, 640);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 2, current_id++));

    //49
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0, current_id++));

    //50
    MakeObjectHeader(e0, 600, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 5, current_id++));

    //51
    MakeObjectHeader(c0, -240, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 0, current_id++));

    //52
    MakeObjectHeader(c0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 0, current_id++, true));

    //53
    MakeObjectHeader(c0, 960, 1707, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(c0, 5, current_id++, true));

    //54
    MakeObjectHeader(e0, 120, 1707, 120, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0, current_id++));

    //55
    MakeObjectHeader(e0, 360, 1707, 360, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0, current_id++));

    //56
    MakeObjectHeader(e0, 960, 1066, 600, 1066);
    world_->AddObject(MakeCreateObjectTriggerObject(e0, 0.5, current_id++));
    */
    return true;
}

void GameStage::Update(float dt) {

    parallax_->updateWithVelocity(ccp(0, -5.0f), dt);

    CheckForcesNumberMessage msg = CheckForcesNumberMessage::Create(false);
    world_->OnMessage(&msg);

    if(msg.forces_number == 0) {
        is_game_over_ = true;
    }
}

void GameStage::Start() {
    //0번부터 시작하라는 메시지를 보냄.
    BeginTriggerMessage begin_msg = BeginTriggerMessage::Create(0);
    world_->OnMessage(&begin_msg);
}
