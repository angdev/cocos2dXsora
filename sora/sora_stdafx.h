// Ŭnicode please
// pch를 지원하지 않는 환경에서도 인클루드한게 통쨰로 인클루드 하는걸 방지하기위해서 이런식으로 처리했다
#include "arch.h"
// assert는 거의 전역에서 쓰니까
#include "assert_include.h"

#if SR_USE_PCH

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <climits>
#include <cassert>
#include <cstdarg>
#include <cstddef>

#include <iostream>
#include <sstream>

#include <typeinfo>

#include <memory>
#include <new>

#include <algorithm>

#include <vector>
#include <map>
#include <set>
#include <string>

#include "tinyxml/tinyxml.h"
#include <Box2D/Box2D.h>
#include "cocos2d.h"

//math
#include "glm/glm.hpp"

#include "assert_include.h"

#include "array_include.h"
#include "tuple_include.h"
#include "random_include.h"
#include "shared_ptr_include.h"
#include "unordered_map_include.h"

#include "globals.h"

#endif