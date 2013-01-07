// Ŭnicode please
#include "stdafx.h"
#include "game_info_layer.h"

#include "game_world.h"
#include "game_object.h"
#include "ai_component.h"

USING_NS_CC;

GameInfoLayer::GameInfoLayer(GameWorld *world) : world_(world), score_(0), player_hit_point_(0), player_max_hit_point_(100) {

}

GameInfoLayer::~GameInfoLayer() {

}

bool GameInfoLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }

    //메시지 핸들러 등록
    RegisterMsgFunc(this, &GameInfoLayer::OnDestroyMessage);
    
    //score : 이거 초기화
    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF *score_text_label = CCLabelTTF::create("Score : ", "Arial", 30);
    score_text_label->setAnchorPoint(ccp(0, 0.5));
    score_text_label->setPosition(ccp(20, win_size.height-GAME_INFO_UI_PADDING_Y));
    this->addChild(score_text_label);

    //실제 점수 표시해주는 label 초기화
    //dimension 제대로 정의할 필요가 있음.
    score_label_ = CCLabelTTF::create("0", "Arial", 30, CCSizeMake(200, 30), kCCTextAlignmentLeft);
    score_label_->setAnchorPoint(ccp(0, 0.5));
    score_label_->setPosition(ccp(score_text_label->getContentSize().width + score_text_label->getPositionX(),
        win_size.height-GAME_INFO_UI_PADDING_Y));
    this->addChild(score_label_);

    return true;
}

void GameInfoLayer::OnDestroyMessage(DestroyMessage *msg) {
    StopRenderHitPointBar(msg->obj_id);

    //이 부분을 따로 빼야하지 않나 싶지만 걍 둔다.
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


    //점수 업데이트
    std::stringstream ss;
    ss << score_;
    score_label_->setString(ss.str().c_str());
}

void GameInfoLayer::DrawPlayerHitPointBar() {
    //glEnable(GL_LINE_SMOOTH);
    //glColor4ub(0, 0, 0, 0xff);
    glLineWidth(1.0f);

    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    float vertex_start_x = score_label_->getPositionX() + score_label_->getDimensions().width;
    //음.. 제대로 그려보고 수치 다 맞춰야할듯
    //기획하는 양반들한테 미루자
    float vertex_start_y  = win_size.height - GAME_INFO_UI_PADDING_Y + 10;

    //애초에 UI 짤 때 화면 비율로 해야하는데 그냥 때려박자
    float hp_bar_length = player_hit_point_ / player_max_hit_point_ * 350;
    ccDrawSolidRect(ccp(vertex_start_x, vertex_start_y), 
        ccp(vertex_start_x + hp_bar_length, vertex_start_y - 30), ccc4f(0, 1, 0, 1));
}


void GameInfoLayer::draw() {
    DrawPlayerHitPointBar();

    for(auto pair : hp_bar_dict_) {
        DrawHitPointBar(pair.second);
    }
}

void GameInfoLayer::RequestRenderHitPointBar(int id, const glm::vec2 &pos, float hp_ratio) {
    auto found = hp_bar_dict_.find(id);
    if(found != hp_bar_dict_.end()) {
        //상태 업데이트
        found->second.hp_ratio = hp_ratio;
        found->second.obj_pos = pos;
        return;
    }

    HPBarRenderState state;
    state.hp_ratio = hp_ratio;
    state.obj_pos = pos;
    hp_bar_dict_.insert(std::make_pair(id, state));
}

void GameInfoLayer::StopRenderHitPointBar(int id){
    hp_bar_dict_.erase(id);
}

void GameInfoLayer::DrawHitPointBar(const HPBarRenderState &state) {
    //일단 안쪽 체력바 그리고
    //바깥쪽 테두리 그린다
    //기체 크기를 받아와서 적당히 아래쪽에 그리긴 해야하는데 그런건 없다
    
    float vertex_start_x = state.obj_pos.x - (HP_BAR_WIDTH)/2.0f;
    float vertex_start_y = state.obj_pos.y - (HP_BAR_HEIGHT)/2.0f - 80;

    //체력 그리기
    ccDrawSolidRect(ccp(vertex_start_x, vertex_start_y), 
        ccp(vertex_start_x + HP_BAR_WIDTH * state.hp_ratio, vertex_start_y - HP_BAR_HEIGHT), ccc4f(0, 0.7f, 0, 0.5f));

    //테두리 그리기
    ccDrawRect(ccp(vertex_start_x, vertex_start_y), ccp(vertex_start_x+HP_BAR_WIDTH, vertex_start_y-HP_BAR_HEIGHT));

}
