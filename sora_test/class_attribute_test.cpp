// Ŭnicode please
#include "stdafx.h"
#include "sora/class_attribute.h"

using namespace std;
using namespace sora;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace sora_test {;

TEST_CLASS(AttributeTest) {
public:
    TEST_METHOD(DefaultType) {
	    //int형에 대한 기본 테스트
	    Attribute attr1 = Attribute::Int("name", 1);
	    Assert::AreEqual("name", attr1.name().c_str());
	    Assert::AreEqual(1, attr1.offset());
	    Assert::AreEqual((int)kAttributeInt, (int)attr1.type());
	    Assert::AreEqual(false, attr1.IsUseEnum());
	    Assert::AreEqual(0, attr1.GetEnumCount());
    }

    TEST_METHOD(StrEnumType) {
	    Attribute attr = Attribute::StringEnum("name", 1);
	    Assert::AreEqual((int)kAttributeStringEnum, (int)attr.type());
	    Assert::AreEqual(true, attr.IsUseEnum());

	    Assert::AreEqual(0, attr.GetEnumCount());
	    attr.AddStringEnum("a");
	    Assert::AreEqual(1, attr.GetEnumCount());
	    attr.AddStringEnum("b");
	    Assert::AreEqual(2, attr.GetEnumCount());
	    //중복 추가는 무시
	    attr.AddStringEnum("a");
	    Assert::AreEqual(2, attr.GetEnumCount());

	    Assert::AreEqual(true, attr.IsValidStrEnum("a"));
	    Assert::AreEqual(false, attr.IsValidStrEnum("xx"));
    }
};


TEST_CLASS(ClassAttributeTest) {
public:
    TEST_METHOD(run) {
	    ClassAttribute class_attr("sample1", "sample1", 0);
	    class_attr.Add(Attribute::Int("a", 1));
	    class_attr.Add(Attribute::Int("b", 1));

	    Assert::AreEqual(true, class_attr.IsAttributeExist("a"));
	    Assert::AreEqual(false, class_attr.IsAttributeExist("cc"));

	    Attribute *attr = NULL;
	    attr = class_attr.Get("a");
	    Assert::AreEqual(true, attr != NULL);
	    Assert::AreEqual("a", attr->name().c_str());

	    //올바르지 않은 데이터 이름
	    attr = class_attr.Get("sdffsdb");
        Assert::AreEqual((int)nullptr, (int)attr);	
    }
};
}