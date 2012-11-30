// Ŭnicode please
#pragma once

#include <vector>
#include "tuple_include.h"
#include "data_helper.h"
#include "class_dict.h"
#include "class_attribute.h"

#define CONVERTER_DEF() \
	public:	\
	BaseDataPtr Convert(BaseData *from) const {	\
	sora::ClassDict &dict = sora::ClassDict::GetInstance();	\
	/*from의 최고버전에 해당되는 객체 얻기*/	\
	sora::ClassAttribute *from_class_attr = dict.Get(from->class_hash);	\
	IUASSERT(from_class_attr != NULL);	\
	sora::ClassAttribute *to_class_attr = dict.Get(ToType::ClassHash());	\
	IUASSERT(to_class_attr != NULL);	\
	BaseDataPtr to_base_data(to_class_attr->Clone());	\
	FromType &from_data = static_cast<FromType&>(*from);	\
	ToType &to_data = static_cast<ToType&>(*to_base_data);	\
	Convert(from_data, to_data);	\
	return to_base_data;	\
}	

class StructVersionConverter;

struct StructConverterParam {
	unsigned int from_class_hash;
	unsigned int to_class_hash;
	StructVersionConverter *conveter;
};

class StructVersionConverter {
public:
	//최신버전으로 캐스팅하기
	static BaseDataPtr ConvertToLatest(BaseDataPtr from);

	template<typename T>
	static void RegisterConverter() {
		int from_class_hash = T::FromType::ClassHash();
		int to_class_hash = T::ToType::ClassHash();
		T *converter = new T();
		StructConverterParam param;
		param.from_class_hash = from_class_hash;
		param.to_class_hash = to_class_hash;
		param.conveter = converter;
		converter_list_.push_back(param);
	}
private:
	//from_class_hash
	//to_class_hash
	//converter
	static std::vector<StructConverterParam> &GetConverterList();
	static std::vector<StructConverterParam> converter_list_;

public:
	virtual BaseDataPtr Convert(BaseData *from) const = 0;
};

//자동 컨버터 예제
//매크로를 사용하고 FromType, ToType만 적절히 잡아주고
//convert하뭇만 잘 만들어주면 나머지는 알아서됨
class Converter_VCSDemoHeaderV1_VCSDemoHeaderV2 : public StructVersionConverter {
	//아래의 매크로를 선언해
	CONVERTER_DEF()
public:
  typedef VCSDemoHeader_v1 FromType;
	typedef VCSDemoHeader_v2 ToType;

public:
	static void Convert(const FromType &from, ToType &to);
};
