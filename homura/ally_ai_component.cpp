// Ŭnicode please
#include "stdafx.h"
#include "ally_ai_component.h"
#include "game_object.h"
#include "game_world.h"

#include "character_component.h"

#include "sora/unit.h"

using namespace sora;

AllyAIComponent::AllyAIComponent(GameObject *obj) : AIComponent(obj), state_(kAllyNormalState) {

}

AllyAIComponent::~AllyAIComponent() {

}

void AllyAIComponent::Update(float dt) {
    if(state_ == kAllyFallState) {
        //Test Move
        b2Vec2 move_vec(sora::Unit::ToUnitFromMeter(0.1f), 0);
        MoveByMessage msg = MoveByMessage::Create(Unit::ToUnitFromMeter(move_vec), 1);
        obj()->OnMessage(&msg);

        //CCLOG("%f", char_comp()->hit_point());
        //낙오 상태에서는 체력이 줄어 있는데 최대 체력이 되면 부활
        if(char_comp()->max_hit_point() - char_comp()->hit_point() < 1.0f) {
            state_ = kAllyNormalState;

            //편대에 가입
            RequestJoinFormationMessage msg = RequestJoinFormationMessage::Create(char_comp()->obj()->id());
            obj()->world()->OnMessage(&msg);
        }

        //TODO: 천천히 이동
    }

    else if(state_ == kAllyArrestState) {
        //묶여있는 상태
        //master가 죽었는지 확인 == 체인이 존재하는지 확인
        CheckConnectedChainMessage check_msg = CheckConnectedChainMessage::Create(obj()->id());
        obj()->world()->OnMessage(&check_msg);

        //파괴되었으면 상태 전환
        if(check_msg.checked == false) {

            state_ = kAllyNormalState;

            //편대에 가입
            RequestJoinFormationMessage msg = RequestJoinFormationMessage::Create(obj()->id());
            obj()->world()->OnMessage(&msg);
        }
    }

    else if(state_ == kAllyNormalState) {
        AttackMessage atk_msg = AttackMessage::Create(0);
        obj()->OnMessage(&atk_msg);
    }

    else {
        assert(false && "state error");
    }
}

void AllyAIComponent::InitMsgHandler() {
    AIComponent::InitMsgHandler();
}