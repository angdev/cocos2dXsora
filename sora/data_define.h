// Ŭnicode please
#pragma once

#include <string>
#include <vector>
#include "class_dict.h"
#include "string_hash.h"
#include "shared_ptr_include.h"

struct BaseData;
typedef std::shared_ptr<BaseData> BaseDataPtr;

struct BaseData {
	//클래스 1개를 해시용으로 고정
	typedef sora::RSHash HashFunction;

	BaseData() : class_hash(0), base_hash(0) {}
	virtual ~BaseData() {}
	virtual BaseData *Clone() const = 0;
	unsigned int class_hash;	//클래스 이름기반의 해시. 고유
	unsigned int base_hash;		//부모 클래스 기반의 해시. 같은 그룹은 같다
};

/// begin macro 
#undef STRUCT_NEW_BEGIN
#undef STRUCT_BEGIN
#undef STRUCT_END
#undef STRUCT_VAR_INT
#undef STRUCT_VAR_FLOAT
#undef STRUCT_VAR_STR_ENUM_BEGIN
#undef STRUCT_VAR_STR_ENUM_KEY
#undef STRUCT_VAR_STR_ENUM_END
#undef STRUCT_VAR_BOOL
#undef STRUCT_VAR_STRING
#undef STRUCT_VAR_STRING_LIST

#define STRUCT_BEGIN(BASE, VERSION)	\
	bool init_##BASE##_v##VERSION();	\
	struct BASE##_v##VERSION : public BaseData {	\
		BASE##_v##VERSION() {	\
			class_hash = ClassHash();	\
			base_hash = BaseHash();	\
		}	\
		~BASE##_v##VERSION() {}	\
		virtual BaseData *Clone() const { return new BASE##_v##VERSION(); }	\
		static unsigned int ClassHash() {	\
			static bool init = false;	\
			static unsigned int hash_value = 0;	\
			if(init == false) {	\
				init = true;	\
				std::string base_name = #BASE;	\
				std::string version_str = #VERSION;	\
				std::string class_name = base_name + "_v" + version_str;	\
				hash_value = BaseData::HashFunction::Hash(class_name.c_str(), class_name.length());	\
			}	\
			return hash_value;	\
		}	\
		static unsigned int BaseHash() {	\
			static bool init = false;	\
			static unsigned int hash_value = 0;	\
			if(init == false) {	\
				init = true;\
        std::string base_name = #BASE;  \
				hash_value = BaseData::HashFunction::Hash(base_name.c_str(), base_name.length());	\
			}	\
			return hash_value;	\
		}


//새버전이라고 정의할때 쓰인다
#define STRUCT_NEW_BEGIN(BASE, VERSION)	\
	struct		BASE##_v##VERSION;	\
	typedef		BASE##_v##VERSION	BASE;	\
	STRUCT_BEGIN(BASE, VERSION)

#define STRUCT_END()	};
#define STRUCT_VAR_INT(NAME)	int NAME;
#define STRUCT_VAR_FLOAT(NAME)	float NAME;
#define STRUCT_VAR_STR_ENUM_BEGIN(NAME)	std::string NAME;
#define STRUCT_VAR_STR_ENUM_KEY(KEY)
#define STRUCT_VAR_STR_ENUM_END()

#define STRUCT_VAR_BOOL(NAME)	bool NAME;
#define STRUCT_VAR_STRING(NAME)	std::string NAME;
#define STRUCT_VAR_STRING_LIST(NAME)	std::vector<std::string> NAME;

/// end macro

//디버깅 및 기본 매크로 테스트 위해서 예제 클래스 2개만 풀어서 쓰자
bool init_SampleData_v1();
struct SampleData_v1 : public BaseData {
	SampleData_v1() {}
	~SampleData_v1() {}
	virtual BaseData *Clone() const { return new SampleData_v1(); }

	int int_value;
};

bool init_SampleData_v2();
struct SampleData_v2 : public BaseData {
	SampleData_v2() {}
	~SampleData_v2() {}
	virtual BaseData *Clone() const { return new SampleData_v2(); }

	float float_value;
	std::string str_enum;
};

#include "data_file.h"

/*
bool init_DemoStruct_v1();
struct DemoStruct_v1 : public BaseData {
	DemoStruct_v1() {}
	~DemoStruct_v1() {}
	virtual BaseData *Clone() const { return new DemoStruct_v1(); }
	
	int a;
	float b;
};
*/