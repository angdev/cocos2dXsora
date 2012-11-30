// Ŭnicode please
#include "stdafx.h"
#include "sora/struct_version_converter.h"
#include "sora/data_define.h"

#include "sora/data_init.h"

using namespace sora;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace sora_test {;
TEST_CLASS(StructVersionConverterTest) {
public:
    TEST_CLASS_INITIALIZE(class_init) {
        init_generated_data();
    }

    TEST_METHOD(ConvertToLatest) {
        {
            //v1->v2
            BaseDataPtr orig(new VCSDemoHeader_v1());
            DataHelper::ResetGameHeader(orig.get());
            VCSDemoHeader_v1 *orig_ptr = static_cast<VCSDemoHeader_v1*>(orig.get());
            orig_ptr->ivalue = 10;

            //result를 최신버전으로 변환하기
            BaseDataPtr result = StructVersionConverter::ConvertToLatest(orig);

            Assert::AreEqual(orig->base_hash, result->base_hash);
            Assert::AreNotEqual(orig->class_hash, result->class_hash);
            Assert::IsFalse(DataHelper::Equal(orig.get(), result.get()));
        }
        {
            //v2->v2
            BaseDataPtr orig(new VCSDemoHeader_v2());
            DataHelper::ResetGameHeader(orig.get());
            VCSDemoHeader_v2 *orig_ptr = static_cast<VCSDemoHeader_v2*>(orig.get());
            orig_ptr->fvalue = 10.0f;

            //result를 최신버전으로 변환하기
            BaseDataPtr result = StructVersionConverter::ConvertToLatest(orig);

            Assert::AreEqual(orig->base_hash, result->base_hash);
            Assert::AreEqual(orig->class_hash, result->class_hash);
            Assert::IsTrue(DataHelper::Equal(orig.get(), result.get()));
        }
    } 
};
}

