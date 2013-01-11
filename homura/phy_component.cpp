// Ŭnicode please
#include "stdafx.h"
#include "phy_component.h"
#include "sora/unit.h"
#include "game_object.h"

#if SR_USE_PCH == 0
#include "Box2D/Box2D.h"
#endif

using namespace std;
using namespace sora;

USING_NS_CC;

SinglePhyComponent *PhyComponent::SinglePhy(GameObject *obj, b2Body *body) {
    return new SinglePhyComponent(obj, body);
}

////////////////////////////////////////
SinglePhyComponent::SinglePhyComponent(GameObject *obj, b2Body *body)
: PhyComponent(obj), max_speed_(20.0f), is_arrived_(false), is_moving_(false),
body_(body) {
    body->SetUserData(reinterpret_cast<void*>(obj));
    prev_pos_ = body->GetPosition();
    end_point_ = prev_pos_;
}

SinglePhyComponent::~SinglePhyComponent() {
    if(body_ != NULL) {
        b2World *world = body_->GetWorld();
        world->DestroyBody(body_);
        body_ = NULL;
    }
}

void SinglePhyComponent::Update(float dt) {

    b2Vec2 body_pos = body_->GetPosition();

    if(is_moving_) {
        b2Vec2 pos_diff = end_point_ - body_pos;
        float distance = pos_diff.Length();
        pos_diff.Normalize();
    
        if(distance < 1.0f) {
            //목표지 도착
            body_->SetLinearVelocity(b2Vec2_zero);
            is_arrived_ = true;
            if(obj()->Type() == kCompPlayer) {
                //방향 돌림
                body_->SetTransform(body_pos, M_PI_2);
            }
        }
        else if(distance < 10.0f) {
            pos_diff *= (distance);
            body_->SetLinearVelocity(pos_diff);
        }
        else {
            pos_diff *= max_speed_;
            body_->SetLinearVelocity(pos_diff);
        }
    }
    //CCLOG("%f %f", pos_diff.x, pos_diff.y);

    //경계 체크
    //이 부분 고쳐야 함. 이전 위치 정보를 어떻게 가져올 것인가?
    //바깥쪽으로 속도 벡터 같은게 향하고 있으면 나가는 걸로 처리
    //들어오는 놈은 안 막고 나가는 놈은 막는다
    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    b2Vec2 velocity = body_->GetLinearVelocity();
    glm::vec2 body_pos_px = Unit::ToUnitFromMeter(body_pos);
    if( (body_pos_px.x > win_size.width && velocity.x > 0)
        || (body_pos_px.x < 0 && velocity.x < 0)
        || (body_pos_px.y > win_size.height && velocity.y > 0)
        || (body_pos_px.y < 0 && velocity.y < 0) ) {
            OutOfBoundMessage msg = OutOfBoundMessage::Create(prev_pos_, body_pos);
            obj()->OnMessage(&msg);
    }

    prev_pos_ = body_pos;
}

void SinglePhyComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &SinglePhyComponent::OnMoveToMessage);
    RegisterMsgFunc(this, &SinglePhyComponent::OnMoveByMessage);
    RegisterMsgFunc(this, &SinglePhyComponent::OnRequestPhyBodyInfoMessage);
    RegisterMsgFunc(this, &SinglePhyComponent::OnSetAngleMessage);
    RegisterMsgFunc(this, &SinglePhyComponent::OnSetPhyBodyInfoMessage);
}

//메시지에 포함된 시간은 쓰지 말자
//지울 예정!
void SinglePhyComponent::OnMoveToMessage(MoveToMessage *msg) {
    //msg에 들어오는 값은 px임
    b2Vec2 body_pos = body_->GetPosition();
    b2Vec2 end_pos = Unit::ToMeterFromUnit(msg->vec);
    
    end_point_ = end_pos;

    b2Vec2 velocity_vec = end_pos - body_pos;
    //velocity_vec *= (1.0f / msg->duration);
    
    //body_->SetLinearVelocity(velocity_vec);
    body_->SetTransform(body_->GetPosition(), std::atan2(velocity_vec.y, velocity_vec.x));
    //body_->SetTransform(vec2, body_->GetAngle());
    //body_->SetAwake(true);
    is_arrived_ = false;
    is_moving_ = true;

}


void SinglePhyComponent::OnMoveByMessage(MoveByMessage *msg) {
    b2Vec2 body_pos = body_->GetPosition();
    b2Vec2 end_pos = body_pos + Unit::ToMeterFromUnit(msg->vec);

    end_point_ = end_pos;

    b2Vec2 velocity_vec = end_pos - body_pos;
    
    //velocity_vec *= (1.0f / msg->duration);

    //body_->SetLinearVelocity(velocity_vec);
    body_->SetTransform(body_->GetPosition(), std::atan2(velocity_vec.y, velocity_vec.x));
    is_arrived_ = false;
    is_moving_ = true;
}


void SinglePhyComponent::OnSetAngleMessage( SetAngleMessage *msg ) {
    body_->SetTransform(body_->GetPosition(), msg->angle);
}

void SinglePhyComponent::OnRequestPhyBodyInfoMessage(RequestPhyBodyInfoMessage *msg) {
    msg->phy_body_info->angle_rad = body_->GetAngle();

    b2Vec2 body_pos = body_->GetPosition();
    msg->phy_body_info->x = body_pos.x;
    msg->phy_body_info->y = body_pos.y;
    
    msg->is_ret = true;
}


void SinglePhyComponent::OnSetPhyBodyInfoMessage(SetPhyBodyInfoMessage *msg) {
    body_->SetTransform(b2Vec2(msg->info->x, msg->info->y), msg->info->angle_rad);
    body_->SetLinearVelocity(b2Vec2_zero);
}


void SinglePhyComponent::set_main_body(b2Body *body) {
    //remove prev
    if(body_ != NULL) {
        b2World *world = body_->GetWorld();
        world->DestroyBody(body_);
        body_ = NULL;
    }

    body_ = body;
    body_->SetUserData(obj());
}

//안 쓰는 코드 잠시 보관
void SinglePhyComponent::OnArrivingMoveMessage(ArrivingMoveMessage *msg) {
    b2Vec2 current_pos = body_->GetPosition();
    b2Vec2 target_pos = Unit::ToMeterFromUnit(msg->vec);

    b2Vec2 desired_vec = target_pos - current_pos;
    float distance = desired_vec.Length();
    desired_vec.Normalize();

    if(distance < 5.0f) {
        
    }
    else {
        desired_vec *= max_speed_;
    }
}
