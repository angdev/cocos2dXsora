// Ŭnicode please
#pragma once

#include "platform/CCPlatformConfig.h"
#include "glconsole/GLConsole.h"
#include <cstdlib>

void ConsoleLogLine(const char *line) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
  GLConsole *console = GetConsole();
  if(console != NULL) {
    console->EnterLogLine(line);
  }
#endif
}