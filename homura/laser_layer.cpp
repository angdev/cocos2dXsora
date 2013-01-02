// Ŭnicode please
#include "stdafx.h"
#include "laser_layer.h"

#include "game_object.h"
#include "game_world.h"
#include "game_stage.h"

#include "phy_component.h"
#include "character_component.h"

#include "sora/unit.h"

USING_NS_CC;
using namespace sora;

LaserLayer::LaserLayer(GameWorld *world)
: world_(world),
friend_sprite_(nullptr),
enemy_sprite_(nullptr) {
}
LaserLayer::~LaserLayer() {
}

void LaserLayer::OnMessage(const GameMessage *msg) {
    msg_handler_.HandleMsg(msg);
}

bool LaserLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }

    RegisterMsgFunc(this, &LaserLayer::OnRequestRenderLaserMessage);
    RegisterMsgFunc(this, &LaserLayer::OnStopRenderLaserMessage);
    RegisterMsgFunc(this, &LaserLayer::OnDestroyMessage);

    //텍스쳐로써 필요하다. 단순히 할당만해서 적저맇 잡고잇으면 된다
    friend_sprite_ = CCSprite::create("laser_friend.png");
    enemy_sprite_ = CCSprite::create("laser_enemy.png");
    SR_ASSERT(friend_sprite_ != nullptr);
    SR_ASSERT(enemy_sprite_ != nullptr);
    this->addChild(friend_sprite_);
    this->addChild(enemy_sprite_);
    friend_sprite_->setVisible(false);
    enemy_sprite_->setVisible(false);
        

    return true;
}

LaserRenderState *LaserLayer::GetLaserState(int obj_id) {
    auto friend_found = friend_dict_.find(obj_id);
    if(friend_found != friend_dict_.end()) {
        return &friend_found->second;
    }
    auto enemy_found = enemy_dict_.find(obj_id);
    if(enemy_found != enemy_dict_.find(obj_id)) {
        return &enemy_found->second;
    }
    return nullptr;
}

void LaserLayer::OnRequestRenderLaserMessage(RequestRenderLaserMessage *msg) {
    //자기도 그려달라 이런 식으로 요청하는 녀석들을 맵에 넣는다
    LaserRenderState *prev = GetLaserState(msg->id);
    if(prev == nullptr) {
        //없으면 생성
        LaserStateDict *laser_dict = nullptr;
        //적용인지 아군용인지 확인하기
        GameObjectPtr obj = world_->FindObject(msg->id);
        LogicComponent *logic_comp = obj->logic_comp();
        SR_ASSERT(logic_comp->type() == kCompLaserPlane);
        CharacterComponent *character = static_cast<CharacterComponent*>(logic_comp);

        if(character->is_enemy()) {
            laser_dict = &enemy_dict_;
        } else {
            laser_dict = &friend_dict_;
        }

        LaserRenderState state;
        state.obj = obj;
        state.end_point = msg->end_point;
        (*laser_dict)[msg->id] = state;

    } else {
        //있으면 정보 갱신
        prev->end_point = msg->end_point;
    }
}
void LaserLayer::OnStopRenderLaserMessage(StopRenderLaserMessage *msg) {
    friend_dict_.erase(msg->id);
    enemy_dict_.erase(msg->id);
}
void LaserLayer::OnDestroyMessage(DestroyMessage *msg) {
    friend_dict_.erase(msg->obj_id);
    enemy_dict_.erase(msg->obj_id);
}

void LaserLayer::draw() {
    if(friend_dict_.empty() == false) {
        vector<LaserLine> line_list = GetLaserLineList(friend_dict_);
        DrawLaserList(friend_sprite_, line_list);
    }
    if(enemy_dict_.empty() == false) {
        vector<LaserLine> line_list = GetLaserLineList(enemy_dict_);
        DrawLaserList(enemy_sprite_, line_list);
    }
}

glm::vec2 LaserLayer::GetObjectPosition(const LaserRenderState &state) const {
    PhyBodyInfo body_info;
    RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&body_info);

    state.obj->OnMessage(&body_info_msg);
    SR_ASSERT(body_info_msg.is_ret && "laser layer body info error");
    glm::vec2 body_pos(Unit::ToUnitFromMeter(body_info.x), Unit::ToUnitFromMeter(body_info.y));
    return body_pos;
}

void LaserLayer::DrawLaserList(cocos2d::CCSprite *sprite, const std::vector<LaserLine> &line_list) {
    printf("%d!!!!!!!\n", line_list.size());
}

std::vector<LaserLine> LaserLayer::GetLaserLineList(const LaserStateDict &dict) {
    vector<LaserLine> retval;
    retval.reserve(dict.size());

    for(auto iter : dict) {
        const LaserRenderState &state = iter.second;
        glm::vec2 start_pos = GetObjectPosition(state);
        glm::vec2 end_pos = state.end_point;
        retval.push_back(LaserLine(start_pos, end_pos));
    }
    return retval;
}