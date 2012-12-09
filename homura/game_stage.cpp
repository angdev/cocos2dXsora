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

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

USING_NS_CC;

GameStage::GameStage(GameWorld *world)
    : world_(world), elapsed_time_(0), layer_(0) {

}

GameStage::~GameStage() {

}

bool GameStage::Init() {
    layer_ = CCLayer::create();

    if(NULL == layer_)
        return false;

    factory_ = new GameObjectFactory(world_);

    //Test
    
    //EventGroup #0
    GameTriggerHandler *trg_hnd0 = new GameTriggerHandler();

    TestCombatPlaneObjectHeader combat_header;
    combat_header.angle = M_PI_2;
    combat_header.hit_point = 200;
    combat_header.x = 100;
    combat_header.y = 200;
    combat_header.is_enemy = false;
    combat_header.sprite_name = "";
    GameTrigger *trg0 = new GameTrigger(this);
    GameAction *act0 = MakeCreateObjectAction(combat_header);
    trg0->set_action(act0);
    trg0->set_condition(new NullCondition);
    trg_hnd0->AddTrigger(trg0);
    GameEventObjectHeader e_header;
    NextTriggersPtr next_trigger_1(new NextTriggers());
    next_trigger_1->push_back(2);
    world_->AddObject(factory_->Create(e_header, 1, next_trigger_1, GameTriggerHandlerPtr(trg_hnd0)));
     
    //EventGroup #1
    GameTriggerHandler *trg_hnd1 = new GameTriggerHandler();

    //Event 1
    combat_header.angle = -M_PI_2;
    combat_header.is_enemy = true;
    combat_header.hit_point = 100;
    combat_header.x = 100;
    combat_header.y = 1000;
    combat_header.sprite_name = "";
    GameTrigger *trg1 = new GameTrigger(this);
    GameAction *act1 = MakeCreateObjectAction(combat_header);
    trg1->set_action(act1);
    trg1->set_condition(new NullCondition);
    trg_hnd1->AddTrigger(trg1);
    
    combat_header.x = 300;
    //Event 2
    GameTrigger *trg2 = new GameTrigger(this);
    GameAction *act2 = MakeCreateObjectAction(combat_header);
    trg2->set_action(act2);
    trg2->set_condition(new SpecificDestroyCondition);
    trg_hnd1->AddTrigger(trg2);
    NextTriggersPtr next_trigger_2(new NextTriggers());
    next_trigger_2->push_back(3);

    world_->AddObject(factory_->Create(e_header, 2, next_trigger_2, GameTriggerHandlerPtr(trg_hnd1)));
    
    
    //EventGroup #2
    //Event 1
    GameTriggerHandler *trg_hnd2 = new GameTriggerHandler();

    TestBulletObjectHeader bullet_header;
    bullet_header.x = 600;
    bullet_header.y = 1000;
    bullet_header.angle_rad = 0;
    bullet_header.speed = 10;
    bullet_header.damage = 10;
    GameTrigger *trg3 = new GameTrigger(this);
    GameAction *act3 = MakeCreateObjectAction(bullet_header);
    trg3->set_action(act3);
    trg3->set_condition(new NullCondition);
    trg_hnd2->AddTrigger(trg3);
    NextTriggersPtr next_trigger_3(new NextTriggers());//Empty

    world_->AddObject(factory_->Create(e_header, 3, next_trigger_3, GameTriggerHandlerPtr(trg_hnd2)));

    //1번부터 시작하라는 메시지를 보냄.
    BeginTriggerMessage begin_msg = BeginTriggerMessage::Create(1);
    world_->OnMessage(&begin_msg);

    return true;
}

void GameStage::Update(float dt) {

}