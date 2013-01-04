// Ŭnicode please
#ifndef __GAME_INFO_LAYER_H__
#define __GAME_INFO_LAYER_H__

#include "message_handler.h"

class GameWorld;

//메시지 처리하는 부분 따로 분리해서 상속을 받으면 깔끔하지 않을까?
//아니면 매크로..
//이런 레이어가 많이 지려나

//그나저나 게임 정보는 따로 분리하는게 좋으려나.
//일단 내버려두자
//월드에서 접근 가능하니 ㅇㅅㅇ
class GameInfoLayer : public cocos2d::CCLayer {
public:
    GameInfoLayer(GameWorld *world);
    virtual ~GameInfoLayer();
    bool init();

    void update();

public:
    //message
    void OnMessage(const GameMessage *msg);
    void OnDestroyMessage(DestroyMessage *msg);

public:
    int score() { return score_; }

private:
    template<typename T, typename MsgT>
    void RegisterMsgFunc(T *instance, void (T::*mem_fn)(MsgT*)) {
        msg_handler_.RegisterMessageFunc(instance, mem_fn);
    }

    void CalculateScore(CompType type);

private:
    MessageHandler msg_handler_;
    GameWorld *world_;

    int score_;

    cocos2d::CCLabelTTF *score_label_;
};

#endif