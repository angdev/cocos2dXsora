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

    //Test
    
    //EventGroup #0

    //ally laser test
    GameTriggerHandler *trg_hnd = new GameTriggerHandler();

    AllyPlaneObjectHeader ally_header;
    ally_header.x = 300;
    ally_header.y = -100;
    ally_header.is_fall = false;
    GameTrigger *trg = new GameTrigger(this);
    GameAction *act = MakeCreateObjectAction(ally_header);
    trg->set_action(act);
    trg->set_condition(new NullCondition);
    trg_hnd->AddTrigger(trg);
    GameTriggerObjectHeader trg_header;
    NextTriggers *next_trigger = new NextTriggers();
    next_trigger->push_back(1);
    world_->AddObject(factory_->Create(trg_header, 0, next_trigger, GameTriggerHandlerPtr(trg_hnd)));

    //Temp
    //Chain Test
    GameTriggerHandler *trg_hnd0 = new GameTriggerHandler();

    ally_header.x = 400;
    ally_header.y = 100;
    /*
    CombatPlaneObjectHeader combat_enemy_header = combat_header;
    combat_enemy_header.is_enemy = true;
    combat_enemy_header.angle = -M_PI_2;
    combat_header.is_fall = false;
    combat_enemy_header.x = 400;
    combat_enemy_header.y = 500;
    */
    DeadstarPlaneObjectHeader deadstar_header;
    deadstar_header.x = 400;
    deadstar_header.y = 1300;
    GameTrigger *trg0 = new GameTrigger(this);
    GameAction *act0 = MakeCreateObjectsWithChainAction(deadstar_header, ally_header);
    trg0->set_action(act0);
    trg0->set_condition(new NullCondition);
    trg_hnd0->AddTrigger(trg0);
    GameTriggerObjectHeader e_header;
    NextTriggers *next_trigger_1 = new NextTriggers();
    next_trigger_1->push_back(2);
    world_->AddObject(factory_->Create(e_header, 1, next_trigger_1, GameTriggerHandlerPtr(trg_hnd0)));
    

    

    //EventGroup #1
    GameTriggerHandler *trg_hnd1 = new GameTriggerHandler();

    //Event 1
    CruiserPlaneObjectHeader cruiser_header;
    cruiser_header.x = 100;
    cruiser_header.y = 1300;
    GameTrigger *trg1 = new GameTrigger(this);
    GameAction *act1 = MakeCreateObjectAction(cruiser_header);
    trg1->set_action(act1);
    trg1->set_condition(new SpecificDestroyCondition);
    trg_hnd1->AddTrigger(trg1);
    
    EnemyCombatPlaneObjectHeader enemy_combat_header;
    enemy_combat_header.x = 300;
    enemy_combat_header.y = 1200;
    //Event 2
    GameTrigger *trg2 = new GameTrigger(this);
    GameAction *act2 = MakeCreateObjectAction(enemy_combat_header);
    trg2->set_action(act2);
    trg2->set_condition(new SpecificDestroyCondition);
    trg_hnd1->AddTrigger(trg2);
    NextTriggers *next_trigger_2 = new NextTriggers();
    next_trigger_2->push_back(3);

    world_->AddObject(factory_->Create(e_header, 2, next_trigger_2, GameTriggerHandlerPtr(trg_hnd1)));
    
    
    //EventGroup #2
    //Event 1
    GameTriggerHandler *trg_hnd2 = new GameTriggerHandler();

    GameTrigger *trg3 = new GameTrigger(this);
    GameAction *act3 = new RepeatAction(3, 4);
    trg3->set_action(act3);
    trg3->set_condition(new NullCondition);
    trg_hnd2->AddTrigger(trg3);
    NextTriggers *next_trigger_3 = new NextTriggers();
    next_trigger_3->push_back(2);
    world_->AddObject(factory_->Create(e_header, 3, next_trigger_3, GameTriggerHandlerPtr(trg_hnd2)));

    //EventGroup #3
    //Victory

    GameTrigger *trg4 = new GameTrigger(this);
    GameAction *act4 = new VictoryAction();
    trg4->set_action(act4);
    trg4->set_condition(new NullCondition);
    GameTriggerHandler *trg_hnd3 = new GameTriggerHandler();
    trg_hnd3->AddTrigger(trg4);
    NextTriggers *next_trigger_4 = new NextTriggers();

    world_->AddObject(factory_->Create(e_header, 4, next_trigger_4, GameTriggerHandlerPtr(trg_hnd3)));

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
