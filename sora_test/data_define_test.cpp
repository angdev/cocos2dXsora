// Ŭnicode please
#include "stdafx.h"
#include "sora/data_define.h"
#include "sora/data_init.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace sora;
using namespace std;

namespace sora_test {;

bool init_SampleData_v1_run = init_SampleData_v1();
bool init_SampleData_v2_run = init_SampleData_v2();

TEST_CLASS(BaseDataTest) {
public:
    TEST_CLASS_INITIALIZE(class_init) {
        init_generated_data();
    }

    TEST_METHOD(gen) {
	    ClassDict &dict = ClassDict::GetInstance();
	
	    //코드로 수동생성한거 적절히 연결
	    ClassAttribute *data_a = dict.Get("SampleData_v1");
	    Assert::AreEqual(true, data_a != NULL);
	    Assert::AreEqual("SampleData", data_a->base_name().c_str());
	    Assert::AreEqual("SampleData_v1", data_a->class_name().c_str());
	    Assert::AreEqual(1, data_a->version());
	    //int 1개
	    Assert::AreEqual(1, data_a->Count());
	    auto it = data_a->Begin();
	    auto endit = data_a->End();
	    for( ; it != endit ; ++it) {
		    const Attribute &attr = *it;
		    if(attr.name() == "int_value") {
			    Assert::AreEqual((int)kAttributeInt, (int)attr.type());
		    }
	    }

	    ClassAttribute *data_b = dict.Get(dict.GetLatestClassName("SampleData"));
	    Assert::AreEqual("SampleData", data_b->base_name().c_str());
	    Assert::AreEqual("SampleData_v2", data_b->class_name().c_str());
	    Assert::AreEqual(2, data_b->version());
	    //float + string enum
	    Assert::AreEqual(2, data_b->Count());
	    it = data_b->Begin();
	    endit = data_b->End();
	    for( ; it != endit ; ++it) {
		    const Attribute &attr = *it;
		    if(attr.name() == "float_value") {
			    Assert::AreEqual((int)kAttributeFloat, (int)attr.type());
		    } else if(attr.name() == "str_enum") {
			    Assert::AreEqual((int)kAttributeStringEnum, (int)attr.type());
			    Assert::AreEqual(2, attr.GetEnumCount());
			    Assert::AreEqual(true, attr.IsValidStrEnum("a"));
			    Assert::AreEqual(true, attr.IsValidStrEnum("b"));
			    Assert::AreEqual(false, attr.IsValidStrEnum("c"));
		    }
	    }
    }


    TEST_METHOD(macro_gen) {
	    //bool init_DemoStruct_v1_run = init_DemoStruct_v1();
	    //bool init_DemoStruct_v2_run = init_DemoStruct_v2();
        //init_generated_data();

	    //매크로 기반으로 생성된거 적절히 잇나 테스트
	    ClassDict &dict = ClassDict::GetInstance();

	    ClassAttribute *data_a = dict.Get("DemoStruct_v1");
	    Assert::AreEqual(true, data_a != NULL);
	    Assert::AreEqual("DemoStruct", data_a->base_name().c_str());
	    Assert::AreEqual("DemoStruct_v1", data_a->class_name().c_str());
	    Assert::AreEqual(1, data_a->version());
	    //int
	    Attribute *int_var = data_a->Get("a");
	    Assert::AreEqual(true, int_var != NULL);
	    Assert::AreEqual((int)kAttributeInt, (int)int_var->type());

	    //float
	    Attribute *float_var = data_a->Get("b");
	    Assert::AreEqual(true, float_var != NULL);
	    Assert::AreEqual((int)kAttributeFloat, (int)float_var->type());

	    //bool
	    Attribute *bool_var = data_a->Get("c");
	    Assert::AreEqual(true, bool_var != NULL);
	    Assert::AreEqual((int)kAttributeBool, (int)bool_var->type());

	    //string
	    Attribute *string_var = data_a->Get("d");
	    Assert::AreEqual(true, string_var != NULL);
	    Assert::AreEqual((int)kAttributeString, (int)string_var->type());

	    //vec2. 은 공식지원에서 제외
	    Attribute *vec2_var = data_a->Get("e");
	    Assert::AreEqual(true, vec2_var == NULL);

	    //string list
	    Attribute *str_list_var = data_a->Get("f");
	    Assert::AreEqual(true, str_list_var != NULL);
	    Assert::AreEqual((int)kAttributeStringList, (int)str_list_var->type());
	
	    //vec2 list. 은 공식지원에서 제외. 그래서 검색해도 안뜬다
	    Attribute *vec2_list_var = data_a->Get("g");
	    Assert::AreEqual(true, vec2_list_var == NULL);


	    ClassAttribute *data_b = dict.Get(dict.GetLatestClassName("DemoStruct"));
	    Assert::AreEqual(true, data_b != NULL);
	    Assert::AreEqual("DemoStruct", data_b->base_name().c_str());
	    Assert::AreEqual("DemoStruct_v2", data_b->class_name().c_str());
	    Assert::AreEqual(2, data_b->version());

	    //string enum
	    Attribute *str_enum_var = data_b->Get("c");
	    Assert::AreEqual(true, str_enum_var != NULL);
	    Assert::AreEqual((int)kAttributeStringEnum, (int)str_enum_var->type());
	    Assert::AreEqual(3, str_enum_var->GetEnumCount());
	    Assert::AreEqual(true, str_enum_var->IsValidStrEnum("1"));
	    Assert::AreEqual(true, str_enum_var->IsValidStrEnum("2"));
	    Assert::AreEqual(true, str_enum_var->IsValidStrEnum("3"));
    }

    TEST_METHOD(constuct) {
	    DemoStruct_v1 data_a;
	    Assert::AreEqual(BaseData::HashFunction::Hash("DemoStruct_v1"), data_a.class_hash);
	    Assert::AreEqual(BaseData::HashFunction::Hash("DemoStruct"), data_a.base_hash);
	    Assert::AreEqual(DemoStruct_v1::ClassHash(), data_a.class_hash);
	    Assert::AreEqual(DemoStruct_v1::BaseHash(), data_a.base_hash);
	    ClassAttribute *attr = ClassDict::GetInstance().Get(data_a.class_hash);
	    Assert::AreEqual("DemoStruct_v1", attr->class_name().c_str());
    }

};
}

