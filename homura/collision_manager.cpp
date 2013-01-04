// Ŭnicode please
#include "stdafx.h"
#include "collision_manager.h"
#include "game_object.h"
#include "game_world.h"
#include "collision_handler.h"

#include "sora/template_lib.h"

using namespace std;
using namespace sora;

CollisionManager::CollisionManager(GameWorld *world, b2World *b2_world)
: game_world_(world), b2_world_(b2_world) {
    // 어떤 것을 충돌처리할지 적절히 결정하기
    AddHandler(new CollisionHandler_Bullet_PlayerPlane());
    AddHandler(new CollisionHandler_Player_Plane());
}

void CollisionManager::AddHandler(CollisionHandler_Object_Object *handler) {
    obj_handler_list_.push_back(handler);

    // 고속검색을 위해서 다른 자료구조로 한번더 저장하기
    const vector<CompTypeTuple> &type_tuple_list = handler->GetCompTypeTupleList();
    for(auto type_tuple : type_tuple_list) {
        obj_handler_map_.insert(make_pair(type_tuple, handler));
    }
}

CollisionManager::~CollisionManager() {
    sora::ClearSequenceContainer(obj_handler_list_);
}

void CollisionManager::Update() {
    vector<CollisionTuple> collision_list = GetCollisionList();
    for(auto collision : collision_list) {
        //collion->comp type tuple
        CompTypeTuple type_tuple_1(collision.obj_a()->Type(), collision.obj_b()->Type());
        CompTypeTuple type_tuple_2(collision.obj_b()->Type(), collision.obj_a()->Type());

        set<CollisionHandler_Object_Object*> handler_set;

        auto range_1 = obj_handler_map_.equal_range(type_tuple_1);
        for(auto it = range_1.first ; it != range_1.second ; ++it) {
            CollisionHandler_Object_Object *handler = it->second;
            handler_set.insert(handler);
        }
        auto range_2 = obj_handler_map_.equal_range(type_tuple_2);
        for(auto it = range_2.first ; it != range_2.second ; ++it) {
            CollisionHandler_Object_Object *handler = it->second;
            handler_set.insert(handler);
        }

        for(auto handler : handler_set) {
            handler->OnCollision(collision);
        }
    }
}

std::vector<CollisionTuple> CollisionManager::GetCollisionList() {
    vector<CollisionTuple> collision_tuple_list;

    //충돌정보 얻기
    for(b2Contact *c = b2_world_->GetContactList() ; c ; c = c->GetNext()) {
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
        b2Manifold *m = contact->GetManifold();

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
