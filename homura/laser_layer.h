﻿// Ŭnicode please
#ifndef __LASER_LAYER_H__
#define __LASER_LAYER_H__

#include "message_handleable.h"

struct LaserRenderState {
	LaserRenderState():
		obj_id(0), elapsed_time(0), remain_time(9999999) {
	}

    int obj_id;
    glm::vec2 start_point;
    glm::vec2 end_point;

	float elapsed_time;
	float remain_time;
};

struct LaserLine {
    LaserLine(const glm::vec2 &a, const glm::vec2 &b) {
        if(a.y > b.y) {
            top = a;
            bottom = b;
        } else {
            top = b;
            bottom = a;
        }
    }
    glm::vec2 top;
    glm::vec2 bottom;

	LaserRenderState state;
};

class LaserLayer : public cocos2d::CCLayer, public MessageHandleable {
public:
	typedef std::unordered_map<int, LaserRenderState> LaserStateDict;
public:
    LaserLayer(GameWorld *world);
    virtual ~LaserLayer();

    bool init();
    void draw();
	void update(float dt);


private:
    void OnRequestRenderLaserMessage(RequestRenderLaserMessage *msg);
    void OnStopRenderLaserMessage(StopRenderLaserMessage *msg);
    void OnDestroyMessage(DestroyMessage *msg);
    
private:
    LaserStateDict friend_dict_;
    LaserStateDict enemy_dict_;
    LaserRenderState *GetLaserState(int obj_id);

    GameWorld *world_;

    //렌더링에 사용할 이미지 텍스쳐를 미리 잡아놓기
    //이거+썡 gl로 렌더링을 처리하자
    cocos2d::CCSprite *friend_sprite_;
    cocos2d::CCSprite *enemy_sprite_;

    std::vector<LaserLine> GetLaserLineList(const LaserStateDict &dict);
    void DrawLaserList(cocos2d::CCSprite *sprite, const std::vector<LaserLine> &line_list);
    //glm::vec2 GetObjectPosition(const LaserRenderState &state) const;

private:
	void Update(float dt, LaserStateDict *laser_state_dict);
  
};

#endif