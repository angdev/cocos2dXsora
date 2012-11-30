// Ŭnicode please
#include "stdafx.h"
#include "sora/typelist.h"

#include "sora/data_define.h"
#include "sora/stage_header.h"
#include "sora/data_helper.h"

#include <typeinfo>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace sora;

typedef TYPELIST_3(
	TerrainHeader,
	CheeseHeader,
	PolyBlockHeader
) SampleTypeList;

namespace sora_test {;

TEST_CLASS(TypeListTest) {
public:
    TEST_METHOD(test) {
        //typedef TYPELIST_1(int) type1;
        typedef TYPELIST_1(CheeseHeader) type1;
        typedef TYPELIST_2(int, char) type2;
        type1 a;
        type2 b;
    }

    TEST_METHOD(TypeListLength) {
        //XXX
        //Assert::AreEqual(1, (int)(TypeListLength<TYPELIST_1(CheeseHeader)>::value));
        //Assert::AreEqual(3, (int)(TypeListLength<SampleTypeList>::value));
    }

    TEST_METHOD(TypeList, TypeListTypeAt) {
        typedef TYPELIST_3(int, char, float) type3;
        Assert::IsTrue(typeid(int) == typeid(TypeListTypeAt<type3, 0>::Result));
        Assert::IsTrue(typeid(char) == typeid(TypeListTypeAt<type3, 1>::Result));
        Assert::IsTrue(typeid(float) == typeid(TypeListTypeAt<type3, 2>::Result));
    }
    /*
    template<typename TList>
    class EntityTypeGetter;

    template<typename T1, typename T2>
    struct EntityTypeGetter< TypeList<T1, T2> > {
    typedef T1 LeftNode;
    typedef T2 RightNode;

    };

    template<class T1>
    struct EntityTypeGetter< TypeList<T1, NullType> > {
    typedef T1 LeftNode;
    };

    TEST(TypeList, a)
    {


    int a = EntityTypeGetter<SampleTypeList>::LeftNode::ClassHash();
    printf("%d\n", a);
    }
    */
};

}

