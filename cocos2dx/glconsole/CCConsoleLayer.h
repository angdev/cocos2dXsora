// Ŭnicode please
#pragma once

#include "platform/CCPlatformMacros.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include "layers_scenes_transitions_nodes/CCLayer.h"

class GLFont;

namespace cocos2d {;

class CCConsoleLayer : public CCLayer {
public:
	CCConsoleLayer();
	~CCConsoleLayer();

	CREATE_FUNC(CCConsoleLayer);
	
	bool init();
	void draw();
	void update(float dt);
private:
	GLFont *font_;
    bool debug_kbd_;
};

}	//namespace cocos2d

#endif