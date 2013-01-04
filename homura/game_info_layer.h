// Ŭnicode please
#ifndef __GAME_INFO_LAYER_H__
#define __GAME_INFO_LAYER_H__

#include "message_handleable.h"

class GameWorld;

#define GAME_INFO_UI_PADDING_Y 50

//메시지 처리하는 부분 따로 분리해서 상속을 받으면 깔끔하지 않을까?
//아니면 매크로..
//이런 레이어가 많이 지려나

//그나저나 게임 정보는 따로 분리하는게 좋으려나.
//일단 내버려두자
//월드에서 접근 가능하니 ㅇㅅㅇ
class GameInfoLayer : public cocos2d::CCLayer, public MessageHandleable {
public:
    GameInfoLayer(GameWorld *world);
    virtual ~GameInfoLayer();
    bool init();

    void draw();

public:
    //message
    void OnDestroyMessage(DestroyMessage *msg);

public:
    int score() { return score_; }
    void set_player_hit_point(float player_hit_point) { player_hit_point_ = player_hit_point; }
    void set_player_max_hit_point(float player_max_hit_point) { player_max_hit_point_ = player_max_hit_point; }

private:
    void CalculateScore(CompType type);
    void DrawPlayerHitPointBar();

private:
    GameWorld *world_;

    int score_;
    float player_hit_point_;
    float player_max_hit_point_;

    cocos2d::CCLabelTTF *score_label_;
};

#endif