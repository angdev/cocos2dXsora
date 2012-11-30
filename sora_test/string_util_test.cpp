// Ŭnicode please
#include "stdafx.h"
#include "sora/string_util.h"

using namespace std;
using namespace sora;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace sora_test {;
TEST_CLASS(StringUtilTest) {
public:
    TEST_METHOD(split) {
        string s1 = "1,2";
        vector<string> result1;
        StringUtil::Split(s1, result1, string(","));
        Assert::AreEqual(2u, result1.size());
        Assert::AreEqual("1", result1[0].c_str());
        Assert::AreEqual("2", result1[1].c_str());

        string s2 = "1,2,3,4";
        vector<string> result2;
        StringUtil::Split(s2, result2, string(","));
        Assert::AreEqual(4u, result2.size());
        Assert::AreEqual("1", result2[0].c_str());
        Assert::AreEqual("2", result2[1].c_str());
        Assert::AreEqual("3", result2[2].c_str());
        Assert::AreEqual("4", result2[3].c_str());
    }

    TEST_METHOD(ToUpper) {
        string s1 = "asdf";
        string s2 = StringUtil::ToUpper(s1);
        Assert::AreEqual("ASDF", s2.c_str());
    }
    TEST_METHOD(ToLower) {
        string s1 = "ASdf";
        string s2 = StringUtil::ToLower(s1);
        Assert::AreEqual("asdf", s2.c_str());
    }

    TEST_METHOD(SplitLine) {
        vector<char*> line_list;

        //널 문자열
        line_list = StringUtil::SplitLine((char*)NULL);
        Assert::AreEqual(0u, line_list.size());

        //빈 문자열
        line_list = StringUtil::SplitLine((char*)"");
        Assert::AreEqual(0u, line_list.size());

        //한줄
        line_list = StringUtil::SplitLine((char*)"12");
        Assert::AreEqual(1u, line_list.size());
        Assert::AreEqual("12", line_list[0]);

        //한줄+마지막에 개행 여러개
        char str1[] = "12\n\r\n";
        line_list = StringUtil::SplitLine(str1);
        Assert::AreEqual(1u, line_list.size());
        Assert::AreEqual("12", line_list[0]);

        //여러줄+개행섞기
        char str2[] = "12\n\r\nabc\n\ref";
        line_list = StringUtil::SplitLine(str2);
        Assert::AreEqual(3u, line_list.size());
        Assert::AreEqual("12", line_list[0]);
        Assert::AreEqual("abc", line_list[1]);
        Assert::AreEqual("ef", line_list[2]);
    }
};
}
