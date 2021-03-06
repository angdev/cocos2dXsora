﻿// Ŭnicode please
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

const float kLaserBeginInterval = 0.3f;
const float kLaserEndInterval = 0.2f;
//일단 무조건 같아야 제대로 돌아감
const float kLaserDestroyInterval = kLaserEndInterval;

LaserLayer::LaserLayer(GameWorld *world)
: world_(world),
friend_sprite_(nullptr),
enemy_sprite_(nullptr) {
}
LaserLayer::~LaserLayer() {
    friend_dict_.clear();
    enemy_dict_.clear();
}


bool LaserLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }

	scheduleUpdate();

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
    
    //set shader
    CCGLProgram *prog = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
    this->setShaderProgram(prog);

    return true;
}

LaserRenderState *LaserLayer::GetLaserState(int obj_id) {
    auto friend_found = friend_dict_.find(obj_id);
    if(friend_found != friend_dict_.end()) {
        return &friend_found->second;
    }
    auto enemy_found = enemy_dict_.find(obj_id);
    if(enemy_found != enemy_dict_.end()) {
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
        state.obj_id = msg->id;
        state.start_point = msg->start_point;
        state.end_point = msg->end_point;
        (*laser_dict)[msg->id] = state;

    } else {
        //있으면 정보 갱신
        prev->start_point = msg->start_point;
        prev->end_point = msg->end_point;
    }
}

void ProcessStopRenderLaserMessage(StopRenderLaserMessage *msg, LaserLayer::LaserStateDict &dict) {
	auto found = dict.find(msg->id);
	if(found != dict.end()) {
		if(found->second.remain_time > kLaserEndInterval) {
			found->second.remain_time = kLaserEndInterval;
		}
	}
}
void LaserLayer::OnStopRenderLaserMessage(StopRenderLaserMessage *msg) {
	ProcessStopRenderLaserMessage(msg, friend_dict_);
	ProcessStopRenderLaserMessage(msg, enemy_dict_);
}
void ProcessDestroyMessage(DestroyMessage *msg, LaserLayer::LaserStateDict &dict) {
	auto found = dict.find(msg->obj_id);
	if(found != dict.end()) {
		if(found->second.remain_time > kLaserDestroyInterval) {
			found->second.remain_time = kLaserDestroyInterval;
		}
	}
}

void LaserLayer::OnDestroyMessage(DestroyMessage *msg) {
	ProcessDestroyMessage(msg, friend_dict_);
	ProcessDestroyMessage(msg, enemy_dict_);
}

void LaserLayer::draw() {
    if(friend_dict_.empty() && enemy_dict_.empty()) {
        return;
    }

    CC_NODE_DRAW_SETUP();
	
    if(friend_dict_.empty() == false) {
        vector<LaserLine> line_list = GetLaserLineList(friend_dict_);
        DrawLaserList(friend_sprite_, line_list);
        CC_INCREMENT_GL_DRAWS(1);
    }
    if(enemy_dict_.empty() == false) {
        vector<LaserLine> line_list = GetLaserLineList(enemy_dict_);
        DrawLaserList(enemy_sprite_, line_list);
        CC_INCREMENT_GL_DRAWS(1);
    }
}

void LaserLayer::Update(float dt, LaserStateDict *laser_state_dict) {
	//남은시간이 0이하면 삭제
	vector<int> dead_key_list;

	/*
	for(auto it : laser_state_dict) {
		LaserRenderState &state = it.second;
		state.remain_time -= dt;
		state.elapsed_time += dt;

		if(state.remain_time <= 0) {
			dead_key_list.push_back(it.first);
		}
	}
	*/
	auto it = laser_state_dict->begin();
	auto end = laser_state_dict->end();
	for( ; it != end ; ++it) {
		LaserRenderState &state = it->second;
		//printf("-->%d: %f\n", state.obj_id, state.elapsed_time);

		state.remain_time -= dt;
		state.elapsed_time += dt;

		//printf("%f\n", state.elapsed_time);

		if(state.remain_time <= 0) {
			dead_key_list.push_back(it->first);
		}
	}

	for(int key : dead_key_list) {
		laser_state_dict->erase(key);
		//printf("layser destroy :%d\n", key);
	}
}
void LaserLayer::update(float dt) {
	Update(dt, &enemy_dict_);
	Update(dt, &friend_dict_);
}

void LaserLayer::DrawLaserList(cocos2d::CCSprite *sprite, const std::vector<LaserLine> &line_list) {
    //라인 정보를 삼각형으로 변환
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );

    ccGLBindTexture2D( sprite->getTexture()->getName() );

    //선1개 == 삼각형 2개
    //점4개 + 인덱스 6개로  triangles를 구성하기
    struct Vertex {
        float x, y;
        float s, t;
    };
    std::vector<Vertex> vert_list(4 * line_list.size());
    std::vector<unsigned short> index_list(6 * line_list.size());

    //3 2
    //
    //0 1

    float tex_width = sprite->getTexture()->getContentSize().width;
    float tex_height = sprite->getTexture()->getContentSize().height;

    for(int i = 0 ; i < line_list.size() ; ++i) {
        const LaserLine &line = line_list[i];
        int base_idx = 4 * i;
		
		float laser_width = tex_width;
		
		if(line.state.remain_time > 100) {
			//레이저가 얇은상태에서 굵어지는 과정인 경우
			float s = line.state.elapsed_time / kLaserBeginInterval;
			if(s > 1) {
				s = 1;
			}
			//printf("%d: %f\n", line.state.obj_id, s);
			laser_width *= s;
		} else {
			//레이저가 굵은상태에서 얇아지는 과정인 경우 
			float s = line.state.remain_time / kLaserEndInterval;
			if(s < 0) {
				s = 0;
			}
			//printf("%d: %f\n", line.state.obj_id, s);
			laser_width *= s;
		}
		

        Vertex &left_bottom = vert_list[base_idx + 0];
        Vertex &right_bottom = vert_list[base_idx + 1];
        Vertex &right_top = vert_list[base_idx + 2];
        Vertex &left_top = vert_list[base_idx + 3];

        float left = line.bottom.x - laser_width / 2.0f;
        float right = line.bottom.x + laser_width / 2.0f;
        float bottom = line.bottom.y;
        float top = line.top.y;

        left_bottom.x = left;
        left_bottom.y = bottom;
        left_bottom.s = 0;
        left_bottom.t = 0;

        right_bottom.x = right;
        right_bottom.y = bottom;
        right_bottom.s = 1;
        right_bottom.t = 0;

        left_top.x = left;
        left_top.y = top;
        left_top.s = 0;
        left_top.t = 1;

        right_top.x = right;
        right_top.y = top;
        right_top.s = 1;
        right_top.t = 1;
    }

    for(int i = 0 ; i < line_list.size() ; ++i) {
        int index_base_idx = 6 * i;
        int vert_base_idx = 4 * i;

        index_list[index_base_idx + 0] = vert_base_idx + 0;
        index_list[index_base_idx + 1] = vert_base_idx + 1;
        index_list[index_base_idx + 2] = vert_base_idx + 2;

        index_list[index_base_idx + 3] = vert_base_idx + 0;
        index_list[index_base_idx + 4] = vert_base_idx + 2;
        index_list[index_base_idx + 5] = vert_base_idx + 3;
    }

    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vert_list[0].x);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vert_list[0].s);

    glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);
}

std::vector<LaserLine> LaserLayer::GetLaserLineList(const LaserStateDict &dict) {
    vector<LaserLine> retval;
    retval.reserve(dict.size());

    for(auto iter : dict) {
        const LaserRenderState &state = iter.second;
        glm::vec2 start_pos = state.start_point;
        glm::vec2 end_pos = state.end_point;

		auto line = LaserLine(start_pos, end_pos);
		line.state = state;
		retval.push_back(line);
    }
    return retval;
}