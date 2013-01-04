// Ŭnicode please
#include "stdafx.h"
#include "game_info_layer.h"

#include "game_world.h"
#include "game_object.h"
#include "ai_component.h"

USING_NS_CC;

GameInfoLayer::GameInfoLayer(GameWorld *world) : world_(world), score_(0) {

}

GameInfoLayer::~GameInfoLayer() {

}

bool GameInfoLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }

    //메시지 핸들러 등록
    RegisterMsgFunc(this, &GameInfoLayer::OnDestroyMessage);
    
    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF *score_label = CCLabelTTF::create("Score : ", "Arial", 30);
    score_label->setAnchorPoint(ccp(0, 0.5));
    score_label->setPosition(ccp(20, win_size.height-50));
    this->addChild(score_label);

    return true;
}

void GameInfoLayer::OnMessage(const GameMessage *msg) {
    msg_handler_.HandleMsg(msg);
}

void GameInfoLayer::OnDestroyMessage(DestroyMessage *msg) {
    //일단 기체가 죽을 때만 점수를 올리도록 하지만 어떻게 될지는 모른다
    
    //누가 죽었는지 보고 점수 계산
    GameObjectPtr obj = world_->FindObject(msg->obj_id);
    SR_ASSERT(obj != NULL && "game info layer : game obj is destroied before handling message");
    
    //적을 판별하는 메시지가 있는데 생각해보니까 그냥 ai comp 불러와보면 그냥 알 수 있다..
    AIComponent *ai = obj->ai_comp();
    if(ai == NULL || !ai->IsEnemy()) {
        return;
    }

    CalculateScore(obj->Type());
}

void GameInfoLayer::CalculateScore(CompType type) {
    switch(type) {
    case kCompLaserPlane:
        score_ += 50000;
        break;
    case kCompCombatPlane:
        score_ += 15000;
        break;
    }

    CCLOG("current score : %d", score_);
}

//일단 매 프레임 업데이트가 되도록 만들긴 했는데
//정보 수정이 들어올 때마다 업데이트하는게 적절할듯
void GameInfoLayer::update(float dt) {
    CCLOG("!");
}