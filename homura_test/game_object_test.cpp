// Ŭnicode please
#include "stdafx.h"
#include "homura/game_object.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace sora_test {;
TEST_CLASS(GameObjectTest) {
public:
    TEST_METHOD(id) {
	    //world가 null이라도 일단 동작은 하도록해서
	    //테스트가 쉽게 했다
	    GameObject obj1(NULL);
	    GameObject obj2(NULL);
	    GameObject obj3(NULL);
	    Assert::AreEqual(obj1.id() + 1, obj2.id());
	    Assert::AreEqual(obj2.id() + 1, obj3.id());
    }
};
}