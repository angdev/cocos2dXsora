// Ŭnicode please
#pragma once

struct b2Vec2;

SR_NS_BEGIN(sora);

const float kUnitToMeterRatio = 32.0f;

class Unit {
public:
    enum {
        kWidth = 720,
        kHeight = 1280,
    };
public:
    static glm::vec2 ToUnitFromMeter(const b2Vec2 &mt_p);
    static float ToUnitFromMeter(float mt_length);

    static b2Vec2 ToMeterFromUnit(const glm::vec2 &ut_p);
    static float ToMeterFromUnit(float ut_length);

public:
    static Unit &GetInstance();
private:
    Unit();

    //픽셀 좌표계로써의 화면의 크기
    float px_width_;
    float px_height_;

    //mt->unit scale
    float mt_to_ut_scale_;
    //unit->pt scale
    //pt 좌표계가 따로 필요할지, 아니면 적절히 Unit를 전체에 대응되도록할지 알아서 정하기
    //float ut_to_px_scale_;

    //getter/setter
public:
    void set_px_width(float f) { px_width_ = f; }
    float pt_width() const { return px_width_; }

    void set_px_height(float f) { px_height_ = f; }
    float px_height() const { return px_height_; }

    void set_mt_to_ut_scale(float f) { mt_to_ut_scale_ = f; }
    float mt_to_ut_scale() const { return mt_to_ut_scale_; }
    float ut_to_mt_scale() const { return 1.0f / mt_to_ut_scale_; }
};

SR_NS_END()