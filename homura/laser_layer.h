// Ŭnicode please
#ifndef __LASER_LAYER_H__
#define __LASER_LAYER_H__

#include "logic_component.h"

class LaserLayer : public LogicComponent {
public:
    LaserLayer(GameObject *obj, cocos2d::CCNode *parent);
    virtual ~LaserLayer();

public:
    virtual CompType type() const;
    virtual void Update(float dt);
    virtual void InitMsgHandler();

    void OnRequestRenderLaserMessage(RequestRenderLaserMessage *msg);
    void OnStopRenderLaserMessage(StopRenderLaserMessage *msg);
    void OnDestroyMessage(DestroyMessage *msg);

private:
    void StartDraw();

private:
    struct LaserRenderState {
        //적, 아군?
        bool is_enemy_type;
        bool now_rendering;
        glm::vec2 end_point;
        cocos2d::CCSprite *sprite;
        GameObjectPtr obj;
    };
    std::map<int, LaserRenderState> state_table_;
    cocos2d::CCNode *parent_;
};

#endif