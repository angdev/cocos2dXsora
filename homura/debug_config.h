// Ŭnicode please
#pragma once

#include "singleton.h"

class DebugConfig : public sora::Singleton<DebugConfig> {
public:
	DebugConfig();
	~DebugConfig() {}

	//macro template
public:
	static int xxx() { return GetInstance().xxx_; }
	static void set_xxx(int value) { GetInstance().xxx_ = value; }
private:
	int xxx_;	//first value

#define VAR_GEN(TYPE, NAME)	\
public:	\
	static TYPE NAME() { return GetInstance().NAME##_; }	\
	static void set_##NAME(TYPE value) { GetInstance().NAME##_ = value; }	\
private:	\
	TYPE NAME##_;

	VAR_GEN(bool, b2_debug_draw);
	VAR_GEN(bool, touch_debug_draw);

	//실제로 보여지는 레이어 on/off

#undef VAR_GEN
};
