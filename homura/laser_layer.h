// Ŭnicode please
#ifndef __LASER_LAYER_H__
#define __LASER_LAYER_H__

#include "message_handler.h"

struct LaserRenderState {
    glm::vec2 end_point;
    GameObjectPtr obj;
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
};

class LaserLayer : public cocos2d::CCLayer {
public:
    LaserLayer(GameWorld *world);
    virtual ~LaserLayer();

    bool init();
    void draw();

    //message
    void OnMessage(const GameMessage *msg);
private:
    void OnRequestRenderLaserMessage(RequestRenderLaserMessage *msg);
    void OnStopRenderLaserMessage(StopRenderLaserMessage *msg);
    void OnDestroyMessage(DestroyMessage *msg);
    
private:
    typedef std::unordered_map<int, LaserRenderState> LaserStateDict;
    LaserStateDict friend_dict_;
    LaserStateDict enemy_dict_;
    LaserRenderState *GetLaserState(int obj_id);

    MessageHandler msg_handler_;
    GameWorld *world_;

    template<typename T, typename MsgT>
	void RegisterMsgFunc(T *instance, void (T::*mem_fn)(MsgT*)) {
		msg_handler_.RegisterMessageFunc(instance, mem_fn);
	}

    //렌더링에 사용할 이미지 텍스쳐를 미리 잡아놓기
    //이거+썡 gl로 렌더링을 처리하자
    cocos2d::CCSprite *friend_sprite_;
    cocos2d::CCSprite *enemy_sprite_;

    std::vector<LaserLine> GetLaserLineList(const LaserStateDict &dict);
    void DrawLaserList(cocos2d::CCSprite *sprite, const std::vector<LaserLine> &line_list);
    glm::vec2 GetObjectPosition(const LaserRenderState &state) const;

    
};

#endif