// Ŭnicode please
#include "stdafx.h"
#include "character_fsm.h"
#include "character_component.h"
#include "game_object.h"
#include "game_message.h"
#include "game_world.h"

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
    
    switch(ally_state_) {
    case kAllyFallState:
        //낙오 상태에서는 체력이 줄어 있는데 최대 체력이 되면 부활
        if(char_comp_->max_hit_point() - char_comp_->hit_point() < 1.0f) {
            ally_state_ = kAllyNormalState;

            //편대에 가입
            RequestJoinFormationMessage msg = RequestJoinFormationMessage::Create(char_comp_->obj()->id());
            char_comp_->obj()->world()->OnMessage(&msg);
            break;
        }

        //TODO: 천천히 이동
        

        break;
    case kAllyArrestState:
        //묶여있는 상태

        break;
    case kAllyNormalState:
        //이 상태에서는 그냥 편대 컴포넌트의 제어를 받는다
        break;
    default:
        assert(false && "ally state error");
        break;
    }

}

void CharacterFSM::EnemyFSMUpdate(float dt) {

}