// Ŭnicode please
#pragma once

#include "shared_ptr_include.h"
#include "stage_header.h"
#include "assert_include.h"

struct BaseData;
typedef std::shared_ptr<BaseData> BaseDataPtr;
class TiXmlElement;

namespace sora {;
class DataHelper {
public:
	static void DataToXmlNode(BaseData *data, TiXmlElement *elem);
	static BaseDataPtr XmlNodeToData(TiXmlElement *elem);
	static bool Equal(BaseData *a, BaseData *b);
	
	//0같은거로 데이터 채우기
	static void Reset(BaseData *data);
	
	//게임에서 객체 생성용으로 쓰이는 어쩌고 header계열은
	//offset0에 type가 잇을것이다
	//여기에는 올바른 타입정보를 넣어주는 함수이다
	template<typename T>
	static void ResetGameHeader(T *data);

	static int GetStructTypeCode(BaseData *data);

	template<typename T>
	static T* ClassCast(BaseData *data);

	//type항목이 잇는 경우, type를 적절히 기록하기
	static void WriteType(BaseData *data, int type);

	static int GetType(BaseData *data);
};

//impl
template<typename T>
void DataHelper::ResetGameHeader(T *data)
{
	BaseData *base_data = static_cast<BaseData*>(data);
	Reset(base_data);

	ClassDict &dict = ClassDict::GetInstance();
	ClassAttribute *class_attr = dict.Get(data->class_hash);
	IUASSERT(class_attr != NULL);

	Attribute *attr = class_attr->Get("type");
	IUASSERT(attr != NULL);
	IUASSERT(attr->type() == kAttributeInt);

	int *type_ptr = (int*)((unsigned char*)base_data + attr->offset());
	int type = GetStructType<T>::value;
	*type_ptr = type;
}

template<typename T>
T* DataHelper::ClassCast(BaseData *data)
{
	if(data->class_hash == T::ClassHash()) {
		return static_cast<T*>(data);
	} else {
		//올바르지 않은 캐스팅
		return NULL;
	}
}

}