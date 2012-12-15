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

        string path = "../sora_test/testdata/";
        string filename = "physics_test.plist";
        cache->addShapesWithFile(path + filename);

        // 가짜 box2d 객체 만들어서 할당 시도
        b2World world(b2Vec2(0, 0));

        b2BodyDef body_def;
        b2Body *body = world.CreateBody(&body_def);
        string body_name = "birdcage01";
        cache->addFixturesToBody(body, body_name.c_str());

        
    } 
};

}   // namespace sora_test
