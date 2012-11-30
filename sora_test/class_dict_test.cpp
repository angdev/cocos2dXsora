// Ŭnicode please
#include "stdafx.h"
#include "CppUnitTest.h"

#include "sora/class_dict.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace sora;

namespace sora_test {;
TEST_CLASS(ClassDictTest) {
public:
    TEST_METHOD(IsClassExist_IsAttributeExist_ETC) {
        ClassDict dict;
	    ClassAttribute sample1("sample", "sample1", 1);
	    sample1.Add(Attribute::Int("a", 1));
        Assert::AreEqual(true, dict.Add(sample1));

	    ClassAttribute sample2("sample", "sample2", 2);
	    sample2.Add(Attribute::Int("b", 1));
	    sample2.Add(Attribute::Int("c", 1));
	    Assert::AreEqual(true, dict.Add(sample2));

	    Assert::AreEqual(true, dict.IsClassExist("sample1"));
	    Assert::AreEqual(true, dict.IsClassExist("sample2"));
	    Assert::AreEqual(false, dict.IsClassExist("not_exist"));

	    //없는 클래스 이름
	    ClassAttribute *attr = NULL;
	    attr = dict.Get("safdsfdmple1");
        Assert::AreEqual((int)nullptr, (int)attr);

	    //잇는거 찾기
	    attr = dict.Get("sample1");
	    Assert::AreEqual(true, attr != NULL);

        //TODO
	    Assert::AreEqual("sample1", attr->class_name().c_str());
    }
    TEST_METHOD(VersionInfo) {
	    ClassDict dict;
	    dict.SetClassVersionInfo("class", "class_v1", 1);
	    dict.SetClassVersionInfo("class", "class_v2", 2);
	    dict.SetClassVersionInfo("base", "base_v2", 2);
    
	    //기반이름으로 검색
        Assert::AreEqual(true, dict.IsBaseClassExist("class"));
	    Assert::AreEqual(false, dict.IsBaseClassExist("clasfdsfds"));
    
	    //최신버전의 클래스 이름 얻기
	    Assert::AreEqual("class_v2", dict.GetLatestClassName("class").c_str());
	    Assert::AreEqual("", dict.GetLatestClassName("fdjsl").c_str());
    
	    //특정 버전의 클래스 이름 얻기
	    Assert::AreEqual("class_v2", dict.GetName("class", 2).c_str());
	    Assert::AreEqual("", dict.GetName("class", 3).c_str());
	    Assert::AreEqual("", dict.GetName("fsdfd", 3).c_str());
    
	    //기반 클래스 목록
	    set<string> base_name_set = dict.GetBaseClassList();
	    Assert::AreEqual(2, (int)base_name_set.size());
	    Assert::AreEqual(true, base_name_set.find("class") != base_name_set.end());
	    Assert::AreEqual(true, base_name_set.find("base") != base_name_set.end());

	    //해당 이름이 최신 버전인가
	    Assert::AreEqual(true, dict.IsLatestClassName("class_v2"));
	    Assert::AreEqual(false, dict.IsLatestClassName("class_v1"));
    }
};

}

