// Ŭnicode please
#ifndef __ACTION_TIMER_H__
#define __ACTION_TIMER_H__

class ActionTimer {

    enum TimerState {
        kAvailable,     //액션이 사용 가능한 상태
        kActive,        //액션이 사용 중인 상태
        kInactive,      //액션이 사용 중에서 막 해제된 상태 -> 직후 바로 unavailable로 바뀜
        kUnavailable,   //액션이 사용 불가능한 상태
    };

public:
    //duration이 음수이면 무한히 진행됨.
    //SetInactive로 끌 수 있음.
    ActionTimer(float cool_down, float duration);
    virtual ~ActionTimer();

public:
    void Update(float dt);
    bool IsAvailable();
    bool IsActive();
    bool IsInactive();
    void SetInactive();
    float GetTime();
    //액션을 사용함 : 타이머 상태가 사용 중인 상태로 변경됨
    void Action();

private:
    //쿨타임
    const float cool_down_;
    //지속 시간
    const float duration_;
    TimerState state_;
    float timer_;
};

#endif