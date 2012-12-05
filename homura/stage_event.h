// Ŭnicode please
#ifndef __STAGE_EVENT_H__
#define __STAGE_EVENT_H__

//이벤트나 스테이지를 GameObject로 만들 수 있을텐데.
//그건 좀 그런가.

class StageEvent {
public:
    StageEvent();
    virtual ~StageEvent();

public:
    float start_time() const { return start_time_; }
    float end_time() const { return end_time_; }

private:
    float start_time_;
    float end_time_;
};

#endif