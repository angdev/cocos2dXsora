// Ŭnicode please
#include "stdafx.h"
#include <string>
#include "sora/property.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace sora_test {;

class PropertySampleRaw {
public:
    int get_a() {
        return a.value * 2;
    }
    int set_a(const int &a) {
        return a * 2;
    }
public:
    //normal form
    class {
    public:
        int &operator=(const int &i) {
            unsigned int offset = offsetof(PropertySampleRaw, a);
            PropertySampleRaw *parent = (PropertySampleRaw*)((unsigned int)(this) - offset);
            value = parent->set_a(i);
            return value;
        }
        operator int() const {
            unsigned int offset = offsetof(PropertySampleRaw, a);
            PropertySampleRaw *parent = (PropertySampleRaw*)((unsigned int)(this) - offset);
            return parent->get_a();
        }
        int value;
    } a;

    //legacy style
    class {
    public:
        float &operator=(const float &i) { return value = i; }
        operator float() const { return value; }
        float value;
    } b;
};

TEST_CLASS(PropertyTest) {
public:
    TEST_METHOD(PropertyTest_test) {
        PropertySampleRaw p1;
        p1.a = 1;   //set하면 2배가 저장
        Assert::AreEqual(4, (int)p1.a);   //get할때 다시 2배
        p1.b = 2;
        Assert::AreEqual(2, (int)p1.b);
    }
};

class PropertySample {
public:
    PropertySample() : c(0) {}

    int get_c() {
        return c.value * 3;
    }
    int set_c(const int &c) {
        return c * 3;
    }

    std::string get_str() {
        return std::string("a");
    }
    std::string set_str(const std::string &str) {
        return str + "a";
    }
    int get_only() {
        return 1;
    }
    int set_only(const int &c) {
        return 2;
    }
public:
    SR_PROPERTY(int, c, PropertySample, get_c, set_c);
    SR_PROPERTY(std::string, d, PropertySample, get_str, set_str);
    SR_PROPERTY_GET(int, get, PropertySample, get_only);
    SR_PROPERTY_SET(int, set, PropertySample, set_only);
};

TEST_CLASS(Property_1) {
public:
    TEST_METHOD(Property_1_test) {
        PropertySample p1;
        //기본값 설정 지원
        Assert::AreEqual(0, (int)p1.c);
        p1.c = 1;   //set하면 3배가 저장
        Assert::AreEqual(9, (int)p1.c);   //get할때 다시 3배

        //객체에 대해서 테스트
        p1.d = "msg";
        std::string tmp = p1.d;
        Assert::AreEqual(true, tmp == "a");
        Assert::AreEqual(1, (int)p1.get);
    }
};

}   //namespace end