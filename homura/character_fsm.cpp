// Ŭnicode please
#include "stdafx.h"
#include "character_fsm.h"
#include "character_component.h"
#include "game_object.h"
#include "game_message.h"
#include "game_world.h"

#include "sora/unit.h"

CharacterFSM::CharacterFSM(CharacterComponent *char_comp) : char_comp_(char_comp), ally_state_(kAllyNormalState), enemy_state_(kEnemyNormalState) {

}

CharacterFSM::~CharacterFSM() {

}

//일단 여기다 전부 구현하자
//적군이든 아군이든 적절히 나눠서
void CharacterFSM::Update(float dt) {
    char_comp_->is_enemy()? EnemyFSMUpdate(dt) : AllyFSMUpdate(dt);
}

void CharacterFSM::AllyFSMUpdate(float dt) {
    //굳이 state를 클래스로 나눌 정도로 할게 있는게 아니니.. 라고 생각했는데..
    //걍 쓴다
    
    //뻐킹 인스턴스
    if(ally_state_ == kAllyFallState) {
        //Test Move
        b2Vec2 move_vec(sora::Unit::ToUnitFromMeter(0.1f), 0);
        MoveMessage msg = MoveMessage::Create(move_vec);
        char_comp_->obj()->OnMessage(&msg);

        CCLOG("%f", char_comp_->hit_point());
        //낙오 상태에서는 체력이 줄어 있는데 최대 체력이 되면 부활
        if(char_comp_->max_hit_point() - char_comp_->hit_point() < 1.0f) {
            ally_state_ = kAllyNormalState;

            //편대에 가입
            RequestJoinFormationMessage msg = RequestJoinFormationMessage::Create(char_comp_->obj()->id());
            char_comp_->obj()->world()->OnMessage(&msg);
        }

        //TODO: 천천히 이동
    }

    else if(ally_state_ == kAllyArrestState) {
        //묶여있는 상태
        //master가 죽었는지 확인 == 체인이 존재하는지 확인
        CheckConnectedChainMessage check_msg = CheckConnectedChainMessage::Create(char_comp_->obj()->id());
        char_comp_->obj()->world()->OnMessage(&check_msg);

        //파괴되었으면 상태 전환
        if(check_msg.checked == false) {

            ally_state_ = kAllyNormalState;
            
            //편대에 가입
            RequestJoinFormationMessage msg = RequestJoinFormationMessage::Create(char_comp_->obj()->id());
            char_comp_->obj()->world()->OnMessage(&msg);
        }
    }
    else if(ally_state_ == kAllyNormalState) {
        //이 상태에서는 그냥 편대 컴포넌트의 제어를 받는다
    }
    else {
        assert(false && "ally state error");
    }
}

void CharacterFSM::EnemyFSMUpdate(float dt) {

}