// Ŭnicode please
#include "CCConsoleKeyboard.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <string>
#include <sstream>

using namespace std;

namespace cocos2d {;

std::string CCKeyCmd::ToString() const
{
	string key_name(1, keycode);
	switch(keycode) {
	case CCConsoleKeyboard::kKeyCtrl:
		key_name = "Ctrl";
		break;
	case CCConsoleKeyboard::kKeyShift:
		key_name = "Shift";
		break;
	case CCConsoleKeyboard::kKeyAlt:
		key_name = "Alt";
		break;
	case CCConsoleKeyboard::kKeyLeft:
		key_name = "Left";
		break;
	case CCConsoleKeyboard::kKeyRight:
		key_name = "Right";
		break;
	case CCConsoleKeyboard::kKeyDown:
		key_name = "Down";
		break;
	case CCConsoleKeyboard::kKeyUp:
		key_name = "Up";
		break;
	case CCConsoleKeyboard::kKeyPageUp:
		key_name = "PageUp";
		break;
	case CCConsoleKeyboard::kKeyPageDown:
		key_name = "PageDown";
		break;
	case CCConsoleKeyboard::kKeyHome:
		key_name = "Home";
		break;
	case CCConsoleKeyboard::kKeyEnd:
		key_name = "End";
		break;
	case CCConsoleKeyboard::kKeyEnter:
		key_name = "Enter";
		break;
	case CCConsoleKeyboard::kKeyTab:
		key_name = "Tab";
		break;
	case CCConsoleKeyboard::kKeyBackspace:
		key_name = "Backspace";
		break;
	case CCConsoleKeyboard::kKeyDelete:
		key_name = "Delete";
		break;
	default:
		break;
	}

	string state_str;
	switch(state) {
	case CCConsoleKeyboard::kPress:
		state_str = "Down";
		break;
	case CCConsoleKeyboard::kRelease:
		state_str = "Up";
		break;
	case CCConsoleKeyboard::kIgnore:
		state_str = "Char";
		break;
	default:
		break;
	}
	ostringstream oss;
	oss << key_name << "[" << state_str << "]";
	return oss.str();
}

int special_key_list[] = {
	CCConsoleKeyboard::kKeyCtrl,
	CCConsoleKeyboard::kKeyShift,
	CCConsoleKeyboard::kKeyAlt,
	CCConsoleKeyboard::kKeyLeft,
	CCConsoleKeyboard::kKeyRight,
	CCConsoleKeyboard::kKeyDown,
	CCConsoleKeyboard::kKeyUp,
	CCConsoleKeyboard::kKeyPageUp,
	CCConsoleKeyboard::kKeyPageDown,
	CCConsoleKeyboard::kKeyHome,
	CCConsoleKeyboard::kKeyEnd,
};


CCConsoleKeyboard::CCConsoleKeyboard()
{
	int key_count = sizeof(key_state_) / sizeof(key_state_[0]);
	for(int i = 0 ; i < key_count ; i++) {
		key_state_[i] = kRelease;
	}

}

CCConsoleKeyboard::~CCConsoleKeyboard()
{
}

CCConsoleKeyboard &CCConsoleKeyboard::GetInstance()
{
	static CCConsoleKeyboard ctx;
	return ctx;
}

void CCConsoleKeyboard::Flush()
{
	key_cmd_list_.clear();
}
bool CCConsoleKeyboard::IsEmpty() const
{
	return key_cmd_list_.empty();
}

CCKeyCmd CCConsoleKeyboard::Pop()
{
	CCKeyCmd cmd = key_cmd_list_.front();
	key_cmd_list_.pop_front();
	return cmd;
}
void CCConsoleKeyboard::Push(const CCKeyCmd &cmd)
{
	key_cmd_list_.push_back(cmd);
}

void CCConsoleKeyboard::CharKey(int key_code)
{
	CCKeyCmd cmd;
	cmd.state = kIgnore;
	cmd.keycode = key_code;
	cmd.special_key = false;
	Push(cmd);
}
void CCConsoleKeyboard::KeyDown(int key_code)
{
	char prev_state = key_state_[key_code];
	if(prev_state == kRelease) {
		key_state_[key_code] = kPress;
		bool is_special = IsSpecialKey(key_code);
		CCKeyCmd cmd;
		cmd.state = kPress;
		cmd.keycode = key_code;
		cmd.special_key = is_special;
		Push(cmd);
	}
}
void CCConsoleKeyboard::KeyUp(int key_code)
{
	char prev_state = key_state_[key_code];
	if(prev_state == kPress) {
		key_state_[key_code] = kRelease;
		bool is_special = IsSpecialKey(key_code);
		CCKeyCmd cmd;
		cmd.state = kRelease;
		cmd.keycode = key_code;
		cmd.special_key = is_special;
		Push(cmd);
	}
}
bool CCConsoleKeyboard::IsSpecialKey(int key_code)
{
	int key_count = sizeof(special_key_list) / sizeof(special_key_list[0]);
	for(int i = 0 ; i < key_count ; i++) {
		if(key_code == special_key_list[i]) {
			return true;
		}
	}
	return false;
}

}	//namespace cocos2d

#endif