// Ŭnicode please
#include "stdafx.h"
#include "sora/gb2shape_cache.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace cocos2d;

namespace sora_test {;
TEST_CLASS(GB2ShapeCacheTest) {
public:
    TEST_METHOD(GB2ShapeCache_test){
        //CCFileUtils::sharedFileUtils()->setResourceDirectory("iphone");
        GB2ShapeCache *cache = GB2ShapeCache::sharedShapeCache();
        cache->addShapesWithFile("physics_test.plist");
    } 
};

}   // namespace sora_test
