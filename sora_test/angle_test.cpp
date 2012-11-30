#include "stdafx.h"
#include "CppUnitTest.h"

#include "sora/angle.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace sora;

namespace sora_test {;
TEST_CLASS(AngleDegTest) {
public:
    TEST_METHOD(Construct) {
        {
            AngleDeg angle(0);
            Assert::AreEqual(0.0f, angle.ToPlusMinus180());
        }
        {
            AngleDeg angle(-30);
            Assert::AreEqual(-30.0f, angle.ToPlusMinus180());
        }
        {
            AngleDeg angle(+60);
            Assert::AreEqual(+60.0f, angle.ToPlusMinus180());
        }
        {
            AngleDeg angle(-180);
            Assert::AreEqual(-180.0f, angle.ToPlusMinus180());
        }
        {
            AngleDeg angle(180);
            Assert::AreEqual(-180.0f, angle.ToPlusMinus180());
        }
        {
            AngleDeg angle(189);
            Assert::AreEqual(-171.0f, angle.ToPlusMinus180());
        }
        {
            AngleDeg angle(-200);
            Assert::AreEqual(160.0f, angle.ToPlusMinus180());
        }
        {
            AngleDeg angle(-600);
            Assert::AreEqual(120.0f, angle.ToPlusMinus180());
        }
    }
};
}