// Ŭnicode please
#include "stdafx.h"
#include "sora/ani_reader.h"

using namespace std;
using namespace sora;
using namespace glm;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace sora_test {;
TEST_CLASS(AniReaderTest) {
public:
    TEST_METHOD(StrToMat3) {
        string str;
        mat3 m;
    
        str = "0.994 -0.099 -32.1 0.092 1.065 12.85 0.0 0.0 1.0";
        XmlAniReader::StrToMat3(str, &m);
        Assert::AreEqual(m[0][0], 0.994f);
        Assert::AreEqual(m[1][0], -0.099f);
        Assert::AreEqual(m[2][0], -32.1f);
        Assert::AreEqual(m[0][1], 0.092f);
        Assert::AreEqual(m[1][1], 1.065f); 
        Assert::AreEqual(m[2][1], 12.85f);
        Assert::AreEqual(m[0][2], 0.0f);
        Assert::AreEqual(m[1][2], 0.0f);
        Assert::AreEqual(m[2][2], 1.0f);

    }

    TEST_METHOD(StrToColor) {
        string str;
        AniColor4ub mul_color;
        AniColor4ub add_color;

        str = "";
        XmlAniReader::StrToColor(str, &mul_color, &add_color);
        for(int i = 0 ; i < 4 ; i++) {
            Assert::AreEqual((unsigned char)255, mul_color.data[i]);
            Assert::AreEqual((unsigned char)0, add_color.data[i]);
        }

        str = "1 1 1 1 0 0 0 0";
        XmlAniReader::StrToColor(str, &mul_color, &add_color);
        for(int i = 0 ; i < 4 ; i++) {
            Assert::AreEqual((unsigned char)255, mul_color.data[i]);
            Assert::AreEqual((unsigned char)0, add_color.data[i]);
        }
 
        str = "0.3 0.7 0.2 0.3 0.2 0.1 0.9 0.8";
        XmlAniReader::StrToColor(str, &mul_color, &add_color);
        Assert::IsTrue(AniColor4ub::Float(0.3f, 0.7f, 0.2f, 0.3f) == mul_color);
        Assert::IsTrue(AniColor4ub::Float(0.2f, 0.1f, 0.9f, 0.8f) == add_color);

        //mul은 add보다 우선순위를 가진다. mul로써 일단 해석하고 남는 필드를 add로 해석한다
        str = "0.3 0.7 0.2 0.3";
        XmlAniReader::StrToColor(str, &mul_color, &add_color);
        Assert::IsTrue(AniColor4ub::Float(0.3f, 0.7f, 0.2f, 0.3f) == mul_color);
        Assert::IsTrue(AniColor4ub::Float(0, 0, 0, 0) == add_color);
    }

    TEST_METHOD(StrToIntList) {
        string str = "1 433 23 563";
        vector<int> result = XmlAniReader::StrToIntList(str);
        Assert::AreEqual(4u, result.size());
        Assert::AreEqual(1, result[0]);
        Assert::AreEqual(433, result[1]);
        Assert::AreEqual(23, result[2]);
        Assert::AreEqual(563, result[3]);
    }
};
}

