// Ŭnicode please
#include "stdafx.h"
#include "phy_world.h"

#include "sora/unit.h"
#include "GLES-Render.h"
#include "collision_tuple.h"
#include "game_object.h"
#include "game_world.h"

//temp
#include "character_component.h"

using namespace std;
using namespace sora;
using namespace cocos2d;

PhyWorld::PhyWorld(GameWorld *game_world)
: game_world_(game_world) {
    Init();
}

PhyWorld::~PhyWorld() {

}

void PhyWorld::Init() {
    b2Vec2 gravity;
    //어차피 충돌처리만 사용할거니까 중력은 필요없다
    gravity.Set(0.0f, 0.0f);
    b2_world_ = std::move(unique_ptr<b2World>(new b2World(gravity)));
    // Do we want to let bodies sleep?
    b2_world_->SetAllowSleeping(true);
    b2_world_->SetContinuousPhysics(true);

    InitDebug(b2_world());
    //InitGround(b2_world());
}

void PhyWorld::InitDebug(b2World *world) {
    const float ptm_ratio = kUnitToMeterRatio;
    debug_draw_ = std::move(unique_ptr<GLESDebugDraw>(new GLESDebugDraw(ptm_ratio)));
    world->SetDebugDraw(debug_draw_.get());

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    debug_draw_->SetFlags(flags);

}

void PhyWorld::InitGround(b2World *world) {
    const float ptm_ratio = kUnitToMeterRatio;

     // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    float bottom = 0;
    float top = visibleSize.height;
    float left = 0;
    float right = visibleSize.width;

    // bottom
    groundBox.Set(b2Vec2(left/ptm_ratio, bottom/ptm_ratio), b2Vec2(right/ptm_ratio, bottom/ptm_ratio));
    groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(left/ptm_ratio, top/ptm_ratio), b2Vec2(right /ptm_ratio, top/ptm_ratio));
    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(left/ptm_ratio, top/ptm_ratio), b2Vec2(left/ptm_ratio, bottom/ptm_ratio));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(right/ptm_ratio, bottom/ptm_ratio), b2Vec2(right/ptm_ratio, top/ptm_ratio));
    groundBody->CreateFixture(&groundBox,0);
}

void PhyWorld::Update(float dt) {
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 0; //8
    int positionIterations = 4; //1

    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    b2_world_->Step(dt, velocityIterations, positionIterations);

    vector<CollisionTuple> collision_list = GetCollisionList();
    for(auto collision : collision_list) {
        HandleCollision(collision);
    }
}

std::vector<CollisionTuple> PhyWorld::GetCollisionList() {
    vector<CollisionTuple> collision_tuple_list;

    //충돌정보 얻기
    for(b2Contact *c = b2_world()->GetContactList() ; c ; c = c->GetNext()) {
		b2Contact *contact = c;

		if(contact->GetFixtureA()->GetBody()->GetUserData() == NULL) {
			continue;
		}
		if(contact->GetFixtureB()->GetBody()->GetUserData() == NULL) {
			continue;
		}
		if(contact->IsTouching() == false) {
			continue;
		}
	
		//같은 타입끼지의 충돌은 취급하지 않는다
		//풍선과 풍선의 충돌은 관심 없잖아?
		GameObject *obj1 = (GameObject*)contact->GetFixtureA()->GetBody()->GetUserData();
		GameObject *obj2 = (GameObject*)contact->GetFixtureB()->GetBody()->GetUserData();
		if(obj1 == NULL || obj2 == NULL) {
			continue;
		}

		//둘중하나라도 꺼저잇으면 로직패스
		if(obj1->IsEnabled() == false || obj2->IsEnabled() == false) {
			continue;
		}

		//물체 2개로 충돌 튜플 만들기. 충돌 튜플은 중복되면 안됨
		GameWorld *world = game_world_;
		GameObjectPtr objptr_a = world->FindObject(obj1->id());
		GameObjectPtr objptr_b = world->FindObject(obj2->id());
		b2Fixture *fixture_a = contact->GetFixtureA();
		b2Fixture *fixture_b = contact->GetFixtureB();
		b2WorldManifold manifold;
		contact->GetWorldManifold(&manifold);
		CollisionTuple collision_t(
			objptr_a, 
			objptr_b, 
			fixture_a, 
			fixture_b, 
			manifold, 
			world);
		collision_tuple_list.push_back(collision_t);
	}
    return collision_tuple_list;
}

void PhyWorld::HandleCollision(CollisionTuple &collision) {
    //테스트 객체 2개 충돌시 삭제
    if(collision.IsMatch(kCompNull, kCompNull)) {
        //먼저 생긴걸 지운다
        GameObjectPtr obj_a = collision.obj_a();
        GameObjectPtr obj_b = collision.obj_b();

        GameObjectPtr first = obj_a->id() < obj_b->id() ? obj_a : obj_b;

        //객체를 지우기. world에서 지우는 함수를 사용해서 바로 지우면
        //게임 로직이 붕괴할 가능성이 있다. update종료후 지워지도록한다
        game_world_->RequestRemoveObject(first);
    }

    //테스트 Null이 AI에 닿으면 AI 체력을 깎음.
    else if(collision.IsMatch(kCompNull, kCompAI)) {
        GameObjectPtr obj_a = collision.obj_a();

        GameObjectPtr obj_ai = (obj_a->Type() == kCompAI)? obj_a : collision.obj_b();
        static_cast<CharacterComponent*>(obj_ai->logic_comp())->ApplyDamage(1);
    }
}