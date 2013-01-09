// Ŭnicode please
#include "stdafx.h"
#include "character_component.h"
#include "game_object.h"
#include "phy_component.h"
#include "game_message.h"
#include "game_world.h"
#include "game_object_factory.h"
#include "game_stage.h"
#include "bullet_component.h"
#include "game_info_layer.h"
#include "aura_layer.h"

#include <random>
#include "sora/unit.h"

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

using namespace sora;

CharacterComponent::CharacterComponent(GameObject *obj, cocos2d::CCNode *layer) 
    : LogicComponent(obj), layer_(layer), unbeatable_(false) {
//    fsm_ = std::move(std::unique_ptr<CharacterFSM>(new CharacterFSM(this)));
}

CharacterComponent::~CharacterComponent() {

}

void CharacterComponent::Update(float dt) {
    
    if(!obj()->IsEnabled())
        return;

    DrawHitPointBar();

    //체력을 가지고 있으므로 이걸 처리
    if(hit_point_ <= 0) {
        //Destroy
        Destroy();
        //OnDestroy -> drawable comp에서 터짐 처리 -> logic comp에서 world에 삭제 요청 보냄.
        //우선순위는 drawable > logic
    }

    //캐릭터 컴포넌트를 가지는 녀석들은 플레이어 객체에 회복을 요청할 수 있다.
    //플레이어 빼고
    //이 부분 따로 빼야함 (쉴드 같은 녀석도 있어서)
    if(type() != kCompPlayer && type() != kCompShield) {
        RequestRecovery();
    }
}

void CharacterComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &CharacterComponent::OnCollideBulletMessage);
    RegisterMsgFunc(this, &CharacterComponent::OnDamageObjectMessage);
    RegisterMsgFunc(this, &CharacterComponent::OnCheckForcesNumberMessage);
    RegisterMsgFunc(this, &CharacterComponent::OnOutOfBoundMessage);
    RegisterMsgFunc(this, &CharacterComponent::OnCreateShieldMessage);
    RegisterMsgFunc(this, &CharacterComponent::OnFindNearestEnemyMessage);
}

void CharacterComponent::Destroy()
{
    GameWorld *world = obj()->world();
    world->RequestRemoveObject(world->FindObject(obj()->id()));
    obj()->ToggleEnable();

    DestroyMessage msg = DestroyMessage::Create(obj()->id());
    world->OnMessage(&msg);

    AfterDestroy();
}

void CharacterComponent::OnCollideBulletMessage(CollideBulletMessage *msg) {
    CollideBullet(msg);
}

void CharacterComponent::CollideBullet(CollideBulletMessage *msg) {
    GameObject *bullet = msg->bullet;
    BulletComponent *comp = static_cast<BulletComponent*>(bullet->logic_comp());
    if(comp->from_enemy() == is_enemy())
        return;
    set_hit_point(hit_point_ - comp->damage());
    
    comp->Destroy();
    //cocos2d::CCLog("%f", hit_point_);
}

//플레이어에게 회복 요청
void CharacterComponent::RequestRecovery() {
    b2Vec2 player_pos;
    RequestPlayerPositionMessage req_player_pos_msg = RequestPlayerPositionMessage::Create(&player_pos);
    obj()->world()->OnMessage(&req_player_pos_msg);

    if(!req_player_pos_msg.is_ret) {
        return;
    }

    PhyBodyInfo obj_body_info;
    RequestPhyBodyInfoMessage req_body_info_msg = RequestPhyBodyInfoMessage::Create(&obj_body_info);
    obj()->OnMessage(&req_body_info_msg);

    assert(req_body_info_msg.is_ret);

    //거리 계산
    float distance = glm::distance(glm::vec2(player_pos.x, player_pos.y), glm::vec2(obj_body_info.x, obj_body_info.y));
    if(distance > 3.0 /* 플레이어 정보 만들어야 함 */) { //단위는 미터
        //그리기 그만 두기
        obj()->world()->aura_layer->StopRenderAura(obj()->id());

        return;
    }

    RequestRecoveryMessage req_recovery_msg = RequestRecoveryMessage::Create(this);
    obj()->world()->OnMessage(&req_recovery_msg);

    //그리기는 플레이어에 있음
    
    return;
}

void CharacterComponent::OnDamageObjectMessage(DamageObjectMessage *msg) {
    //CCLOG("%f", hit_point_);
    set_hit_point(hit_point_ - msg->damage);
}

void CharacterComponent::OnCreateShieldMessage(CreateShieldMessage *msg) {
    if(is_enemy() == msg->from_enemy && type() != kCompShield) {
        //쉴드를 생성
        
        PhyBodyInfo body_info;
        RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&body_info);
        obj()->OnMessage(&body_info_msg);

        assert(body_info_msg.is_ret);

        //팩토리를 가지고 메세지를 받아서 생성해주는 녀석으로 분리?
        GameObjectFactory factory(obj()->world());
        ShieldHeader header;
        header.x = body_info.x;
        header.y = body_info.y;
        header.hit_point = 100; //일단 고정
        header.target_id = obj()->id();
        header.duration = 3.0f;
        //너무 파고드는게 많은 것 같다
        obj()->world()->AddObject(factory.Create(header, layer()));

        //쉴드 생성하면 일단 쉴드 받는 놈은 무적 상태로 만들고
        //쉴드 없어질 때 무적 해제
        set_unbeatable(true);
    }
}

//이거 AI로 옮겨야함
void CharacterComponent::OnCheckForcesNumberMessage(CheckForcesNumberMessage *msg) {
    if(msg->is_enemy == is_enemy()) {
        msg->forces_number++;
    }
}

void CharacterComponent::OnOutOfBoundMessage(OutOfBoundMessage *msg) {
    HandleOutOfBound(msg);
}

void CharacterComponent::HandleOutOfBound(OutOfBoundMessage *msg) {
    //기본적으로는 파괴한다
    //근데 Destroy 호출하면 요란하게 죽으니까 그냥 조용히 제거한다
    GameWorld *world = obj()->world();
    world->RequestRemoveObject(world->FindObject(obj()->id()));
    DestroyMessage destroy_msg = DestroyMessage::Create(obj()->id());
    world->OnMessage(&destroy_msg);

    //혹시 체인에 연결되어 있으면 상대도 같이 없앤다
    RemoveChainPartnerMessage remove_msg = RemoveChainPartnerMessage::Create(obj()->id());
    world->OnMessage(&remove_msg);
}


//이전 코드를 위한 임시 구현
bool CharacterComponent::is_enemy() {
    IsEnemyMessage msg = IsEnemyMessage::Create();
    obj()->OnMessage(&msg);

    return msg.is_enemy;
}

void CharacterComponent::set_hit_point(float hit_point) {
    if(unbeatable_)
        return;
        
    if(hit_point <= max_hit_point_)
        hit_point_ = hit_point;
    else
        hit_point_ = max_hit_point_;
}

void CharacterComponent::DrawHitPointBar() {
    if(!obj()->IsEnabled())
        return;

    glm::vec2 pos = Unit::ToUnitFromMeter(obj()->phy_comp()->main_body()->GetPosition());
    obj()->world()->game_info_layer->RequestRenderHitPointBar(obj()->id(), pos, hit_point_/max_hit_point_);
}

void CharacterComponent::OnFindNearestEnemyMessage(FindNearestEnemyMessage *msg) {
    if(msg->is_enemy == is_enemy()) {
        return;
    }

    b2Vec2 pos = obj()->phy_comp()->main_body()->GetPosition();
    float distance = (msg->ori_pos - pos).Length();
    if(distance < msg->min_distance) {
        msg->min_distance = distance;
        msg->id = obj()->id();
    }
}

//CharacterComponent
