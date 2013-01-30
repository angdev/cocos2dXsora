// Ŭnicode please
#include "stdafx.h"
#include "action_timer.h"

ActionTimer::ActionTimer(float cool_down, float duration = 0.0f) 
    : cool_down_(cool_down),
    duration_(duration),
    timer_(0.0f),
    state_(kAvailable) {

}

ActionTimer::~ActionTimer() {

}

void ActionTimer::Update(float dt) {
    if(state_ == kAvailable) {
        return;
    }

    else if(state_ == kActive) {
        timer_ += dt;
        if(duration_ < 0) {
            return;
        }
        if(timer_ > duration_) {
            timer_ = 0.0f;
            state_ = kInactive;
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

bool ActionTimer::IsAvailable() {
    return (state_ == kAvailable);
}

bool ActionTimer::IsActive() {
    return (state_ == kActive);
}

bool ActionTimer::IsInactive() {
    return (state_ == kInactive);
}

void ActionTimer::SetInactive() {
    state_ = kInactive;
    timer_ = 0.0f;
}

float ActionTimer::GetTime() {
    return timer_;
}

void ActionTimer::Action() {
    if(state_ == kAvailable) {
        state_ = kActive;
    }
}