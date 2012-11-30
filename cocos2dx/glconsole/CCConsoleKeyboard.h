// Ŭnicode please
#pragma once

#include "platform/CCPlatformMacros.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include <string>
#include <list>

namespace cocos2d {;
struct CCKeyCmd {
	int state;	//presse / release
	int keycode;
	bool special_key;
	std::string ToString() const;
};

class CCConsoleKeyboard {
public:
	enum {
		kRelease = 0,
		kPress,
		kIgnore,
	};
	enum {
		//표준문자로 표현되지 않는 특수키
		kKeyCtrl,
		kKeyShift,
		kKeyAlt,
		kKeyLeft,
		kKeyRight,
		kKeyDown,
		kKeyUp,
		kKeyPageUp,
		kKeyPageDown,
		kKeyHome,
		kKeyEnd,
		kKeyEnter,
		kKeyBackspace,
		kKeyTab,
		kKeyDelete,
	};
public:
	CCConsoleKeyboard();
	~CCConsoleKeyboard();
	static CCConsoleKeyboard &GetInstance();

	void Flush();
	bool IsEmpty() const;
	CCKeyCmd Pop();
	CCKeyCmd Top() const { return key_cmd_list_.front(); }

	void Push(const CCKeyCmd &cmd);
	void KeyDown(int key_code);
	void KeyUp(int key_code);
	void CharKey(int key_code);
	static bool IsSpecialKey(int key_code);

	int IsKeyDown(int keycode) const { return key_state_[keycode]; }

private:
	std::list<CCKeyCmd> key_cmd_list_;
	char key_state_[256];
};
}	//namespace cocos2d

#endif
