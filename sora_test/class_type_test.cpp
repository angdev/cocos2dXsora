// Ŭnicode please
#include "stdafx.h"
#include "sora/class_type.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace sora_test {;

class TypeTest {
    SR_SUPER_CLASS();
};
class TypeTest1 : public TypeTest {
    SR_CHILD_CLASS("type_test_1");
};
class TypeTest2 : public TypeTest {
    SR_CHILD_CLASS("type_test_2");
};

TEST_CLASS(ClassTypeTest) {
public:
    TEST_METHOD(run) {
        using std::string;

        TypeTest1 obj1;
        Assert::AreEqual(true, obj1.GetClassType() == TypeTest1::ClassType());
        Assert::AreEqual(true, obj1.GetClassName() == "type_test_1");
        Assert::AreEqual(true, obj1.GetClassName() == TypeTest1::ClassName());

        TypeTest2 obj2;
        const string &name1 = obj1.GetClassName();
        const string &name2 = obj2.GetClassName();
        int code1 = obj1.GetClassType();
        int code2 = obj2.GetClassType();
        Assert::AreEqual(true, name1 != name2);
        Assert::AreEqual(true, code1 != code2);

        const string &name3 = TypeTest1::ClassTypeToName(TypeTest2::ClassType());
        Assert::AreEqual(true, obj2.GetClassName() == name3);
        Assert::AreEqual(true, obj2.GetClassType() == TypeTest1::ClassNameToType(TypeTest2::ClassName()));

    }
};

class Type2Test {
    SR_SUPER_CLASS_2(Type2Test);
public:
    // virtual을 사용하지 않는 버전은 타입을 받아서 설정해줘야돌아간다
    Type2Test(int class_type) {
        set_class_type(class_type);
    }
};
class Type2Test1 : public Type2Test {
    SR_CHILD_CLASS_2("type2_test_1");
public:
    Type2Test1() : Type2Test(ClassType()) {}
};
class Type2Test2 : public Type2Test {
    SR_CHILD_CLASS_2("type2_test_2");
public:
    Type2Test2() : Type2Test(ClassType()) {}
};

TEST_CLASS(ClassType2Test) {
public:
    TEST_METHOD(test) {
        using std::string;

        Type2Test1 obj1;
        Assert::AreEqual(true, obj1.GetClassType() == Type2Test1::ClassType());
        Assert::AreEqual(true, obj1.GetClassName() == "type2_test_1");
        Assert::AreEqual(true, obj1.GetClassName() == Type2Test1::ClassName());

        Type2Test2 obj2;
        const string &name1 = obj1.GetClassName();
        const string &name2 = obj2.GetClassName();
        int code1 = obj1.GetClassType();
        int code2 = obj2.GetClassType();
        Assert::AreEqual(true, name1 != name2);
        Assert::AreEqual(true, code1 != code2);

        const string &name3 = Type2Test1::ClassTypeToName(Type2Test2::ClassType());
        Assert::AreEqual(true, obj2.GetClassName() == name3);
        Assert::AreEqual(true, obj2.GetClassType() == Type2Test1::ClassNameToType(Type2Test2::ClassName()));
    } 
};

}   //namespace sora_test