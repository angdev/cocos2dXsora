// Ŭnicode please
#ifndef __ACTION_TIMER_H__
#define __ACTION_TIMER_H__

template <typename T>
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
    ActionTimer(T *subject, float cool_down, float duration);
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
    
    typedef void (T::*MemFuncVoid)(void);
    void RegisterBeginAction(MemFuncVoid func);
    void RegisterEndAction(MemFuncVoid func);
    typedef void (T::*MemFuncFloat)(float);
    void RegisterActiveAction(MemFuncFloat func);

private:
    //쿨타임
    const float cool_down_;
    //지속 시간
    const float duration_;
    TimerState state_;
    float timer_;

private:
    //타이머의 주체
    T *subject_;
    MemFuncVoid begin_func_;
    MemFuncFloat active_func_;
    MemFuncVoid end_func_;
};

template <typename T>
ActionTimer<T>::ActionTimer(T *subject, float cool_down, float duration = 0.0f) 
    : subject_(subject),
    cool_down_(cool_down),
    duration_(duration),
    timer_(0.0f),
    state_(kAvailable),
    begin_func_(NULL),
    active_func_(NULL),
    end_func_(NULL)
{

}

template <typename T>
ActionTimer<T>::~ActionTimer() {

}

template <typename T>
void ActionTimer<T>::Update(float dt) {
    if(state_ == kAvailable) {
        return;
    }

    else if(state_ == kActive) {
        timer_ += dt;
        if(duration_ < 0) {
            return;
        }

        //active func call
        if(active_func_ != NULL) {
            (subject_->*active_func_)(dt);
        }

        if(timer_ > duration_) {
            timer_ = 0.0f;
            state_ = kInactive;
            //마무리 정리 함수 호출
            if(end_func_ != NULL) {
                (subject_->*end_func_)();
            }
        }
    }

    else if(state_ == kInactive) {
        state_ = kUnavailable;
    }

    else if(state_ == kUnavailable) {
        timer_ += dt;
        if(timer_ > cool_down_) {
            timer_ = 0.0f;
            state_ = kAvailable;
        }
    }
}

template <typename T>
bool ActionTimer<T>::IsAvailable() {
    return (state_ == kAvailable);
}

template <typename T>
bool ActionTimer<T>::IsActive() {
    return (state_ == kActive);
}

template <typename T>
bool ActionTimer<T>::IsInactive() {
    return (state_ == kInactive);
}

template <typename T>
void ActionTimer<T>::SetInactive() {
    state_ = kInactive;
    timer_ = 0.0f;
}

template <typename T>
float ActionTimer<T>::GetTime() {
    return timer_;
}

template <typename T>
void ActionTimer<T>::Action() {
    if(state_ == kAvailable) {
        state_ = kActive;
        if(begin_func_ != NULL) {
            (subject_->*begin_func_)();
        }
    }
}

template <typename T>
void ActionTimer<T>::RegisterBeginAction(MemFuncVoid func) {
    begin_func_ = func;
}

template <typename T>
void ActionTimer<T>::RegisterActiveAction(MemFuncFloat func) {
    active_func_ = func;
}

template <typename T>
void ActionTimer<T>::RegisterEndAction(MemFuncVoid func) {
    end_func_ = func;
}

#endif