// 클nicode please
#include "stdafx.h"
#include "sora/template_lib.h"

using namespace std;
using namespace sora;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class DummyClass {
};

namespace sora_test {;
TEST_CLASS(KKGetDictKeyListTest) {
public:
    TEST_METHOD(test) {
	    map<string, int> dict;
	    dict["a"] = 1;
	    dict["fdsfd"] = 2;
	    dict["432"] = 2;
	    vector<string> keySet;
	    GetDictKeyList(dict, keySet);
        Assert::AreEqual(3, (int)keySet.size());
	    vector<string>::iterator it = find(keySet.begin(), keySet.end(), string("a"));
        Assert::IsTrue(it != keySet.end());
	    it = find(keySet.begin(), keySet.end(), string("fdsfd"));
	    Assert::IsTrue(it != keySet.end());
	    it = find(keySet.begin(), keySet.end(), string("432"));
	    Assert::IsTrue(it != keySet.end());
    }
};

TEST_CLASS(KKEraseKeyFromAssocContainerTest) {
public:
    TEST_METHOD(test) {
	    map<int, DummyClass*> dict;
	    DummyClass *p1 = new DummyClass();
	    DummyClass *p2 = new DummyClass();
	    DummyClass *p3 = new DummyClass();
	    DummyClass *p4 = new DummyClass();
	    DummyClass *p5 = new DummyClass();
	    dict[0] = p1;
	    dict[1] = p2;
	    dict[2] = p3;
	    dict[3] = p4;
	    dict[4] = p5;
        Assert::AreEqual(5, (int)dict.size());
	
	    vector<int> keylist;
	    keylist.push_back(1);
	    keylist.push_back(3);
	    EraseKeyFromAssocContainer(keylist, dict);
	    Assert::AreEqual(3, (int)dict.size());
	    Assert::IsTrue(dict.end() == dict.find(1));
	    Assert::IsTrue(dict.end() == dict.find(3));
	
	    keylist.push_back(0);
	    keylist.push_back(1);
	    keylist.push_back(2);
	    keylist.push_back(4);
	    EraseKeyFromAssocContainer(keylist, dict);
	    Assert::AreEqual(0, (int)dict.size());
    }

};

TEST_CLASS(KKEraseSecondPtrFromAssocContainerTest) {
public:
    TEST_METHOD(test) {
	    map<int, DummyClass*> dict;
	    DummyClass *p1 = new DummyClass();
	    DummyClass *p2 = new DummyClass();
	    DummyClass *p3 = new DummyClass();
	    DummyClass *p4 = new DummyClass();
	    DummyClass *p5 = new DummyClass();
	    dict[0] = p1;
	    dict[1] = p2;
	    dict[2] = p3;
	    dict[3] = p4;
	    dict[4] = p5;
	
        Assert::AreEqual(5, (int)dict.size());
	
	    //2,4
	    vector<DummyClass*> target;
	    target.push_back(p2);
	    target.push_back(p4);
	    EraseSecondPtrFromAssocContainer(target, dict);
	    Assert::AreEqual(3, (int)dict.size());
	    Assert::IsTrue(dict.end() == dict.find(1));
	    Assert::IsTrue(dict.end() == dict.find(3));
	    Assert::AreEqual(0, (int)target.size());
	
	    //1,3,5
	    target.push_back(p1);
	    target.push_back(p3);
	    target.push_back(p5);
	    EraseSecondPtrFromAssocContainer(target, dict);
	    Assert::AreEqual(0, (int)dict.size());
    }
};

TEST_CLASS(IUErasePtrFromSeqContainerTest) {
public:
    TEST_METHOD(test) {
	    list<DummyClass*> dict;
	    DummyClass *p1 = new DummyClass();
	    DummyClass *p2 = new DummyClass();
	    DummyClass *p3 = new DummyClass();
	    DummyClass *p4 = new DummyClass();
	    DummyClass *p5 = new DummyClass();
	    dict.push_back(p1);
	    dict.push_back(p2);
	    dict.push_back(p3);
	    dict.push_back(p4);
	    dict.push_back(p5);
	
        Assert::AreEqual(5, (int)dict.size());
	
	    //2,4??젣
	    vector<DummyClass*> target;
	    target.push_back(p2);
	    target.push_back(p4);
	    ErasePtrFromSeqContainer(target, dict);
	    Assert::AreEqual(3, (int)dict.size());
        Assert::IsTrue(dict.end() == find(dict.begin(), dict.end(), p2));
	    Assert::IsTrue(dict.end() == find(dict.begin(), dict.end(), p4));
	    Assert::AreEqual(0, (int)target.size());
	
	    //1,3,5??젣
	    target.push_back(p1);
	    target.push_back(p3);
	    target.push_back(p5);
	    ErasePtrFromSeqContainer(target, dict);
	    Assert::AreEqual(0, (int)dict.size());
    }

};

TEST_CLASS(KKSafeDeleteTest) {
public:
    TEST_METHOD(test) {
	    int *a = new int(1);
	    SafeDelete(a);
	    Assert::AreEqual(true, NULL == a);

	    int *b = new int[10];
	    SafeDeleteArray(b);
	    Assert::AreEqual(true, NULL == b);
    }    
};
}

