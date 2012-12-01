// Ŭnicode please
#include "stdafx.h"
#include "debug_config.h"
#include "game_console.h"

DebugConfig::DebugConfig()
{
	//0으로 강제 설정. 모든 설정은 일단 꺼놓는다
	memset(this, 0, sizeof(DebugConfig));
	
	set_b2_debug_draw(true);
    set_touch_debug_draw(true);
}
