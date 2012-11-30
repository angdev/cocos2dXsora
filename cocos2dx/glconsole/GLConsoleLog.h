// Ŭnicode please
#pragma once

#include "platform/CCPlatformConfig.h"

void ConsoleLogLine(const char *line);

#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define CCLOGCONSOLE(...) do {} while (0)

#elif COCOS2D_DEBUG == 1
#define CCLOGCONSOLE(...) do { char buf[4096]; sprintf(buf, __VA_ARGS__); ConsoleLogLine(buf); } while(0)

#elif COCOS2D_DEBUG > 1
#define CCLOGCONSOLE(...) do { char buf[4096]; sprintf(buf, __VA_ARGS__); ConsoleLogLine(buf); } while(0)
#endif