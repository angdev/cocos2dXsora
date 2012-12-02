// Ŭnicode please
#include "sora_stdafx.h"
#include "unit.h"

#if SR_USE_PCH == 0
#include "Box2D/Box2D.h"
#endif

SR_NS_BEGIN(sora);

glm::vec2 Unit::ToUnitFromMeter(const b2Vec2 &mt_p) {
    float ut_x = ToUnitFromMeter(mt_p.x);
    float ut_y = ToUnitFromMeter(mt_p.y);
    return glm::vec2(ut_x, ut_y);
}

float Unit::ToUnitFromMeter(float mt_length) {
    Unit &unit = GetInstance();
    float ut_length = mt_length * unit.mt_to_ut_scale();
	return ut_length; 
}

b2Vec2 Unit::ToMeterFromUnit(const glm::vec2 &ut_p) {
    float mt_x = ToMeterFromUnit(ut_p.x);
    float mt_y = ToMeterFromUnit(ut_p.y);
    return b2Vec2(mt_x, mt_y);
}

float Unit::ToMeterFromUnit(float ut_length) {
    Unit &unit = GetInstance();
    float mt_length = ut_length * unit.ut_to_mt_scale();
    return mt_length;
}

Unit &Unit::GetInstance() {
    static Unit ctx;
    return ctx;
}
Unit::Unit()
: px_width_(720), 
px_height_(1280),
mt_to_ut_scale_(kUnitToMeterRatio) {
}

SR_NS_END(sora)