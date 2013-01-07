// Ŭnicode please
#ifndef __GAME_INFO_LAYER_H__
#define __GAME_INFO_LAYER_H__

#include "message_handleable.h"

class GameWorld;

#define GAME_INFO_UI_PADDING_Y 50
#define HP_BAR_WIDTH 120
#define HP_BAR_HEIGHT 10

struct HPBarRenderState {
    float hp_ratio;   //current hp / max hp
    glm::vec2 obj_pos;
};

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
    void RequestRenderHitPointBar(int id, const glm::vec2 &pos, float hp_ratio);
    void StopRenderHitPointBar(int id);

public:
    int score() { return score_; }
    void set_player_hit_point(float player_hit_point) { player_hit_point_ = player_hit_point; }
    void set_player_max_hit_point(float player_max_hit_point) { player_max_hit_point_ = player_max_hit_point; }

private:
    void CalculateScore(CompType type);
    void DrawPlayerHitPointBar();
    void DrawHitPointBar(const HPBarRenderState &state);

private:
    GameWorld *world_;

private:
    int score_;
    float player_hit_point_;
    float player_max_hit_point_;

    cocos2d::CCLabelTTF *score_label_;

private:
    typedef std::unordered_map<int, HPBarRenderState> HPBarStateDict;
    HPBarStateDict hp_bar_dict_;
};

#endif