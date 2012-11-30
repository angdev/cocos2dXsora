// Ŭnicode please
#include "sora_stdafx.h"
#include "data_helper.h"
#include <string>
#include "data_define.h"
#include "class_attribute.h"
#include "class_dict.h"
#include "assert_include.h"
#include <sstream>
#include "string_util.h"
#include "tuple_include.h"

#include "stage_header.h"
#include <tinyxml/tinyxml.h>

#define STRING_LIST_DELIM ":"

using namespace std;

namespace sora {;
void DataHelper::DataToXmlNode(BaseData *data, TiXmlElement *elem)
{
	IUASSERT(elem != NULL);

	ClassAttribute *class_attr = ClassDict::GetInstance().Get(data->class_hash);
	IUASSERT(class_attr != NULL);

	//<class_name base="base_name" version="version">...
	elem->SetValue(class_attr->class_name().c_str());
	elem->SetAttribute("base", class_attr->base_name().c_str());
	char version_buf[16];
	sprintf(version_buf, "%d", class_attr->version());
	elem->SetAttribute("version", version_buf);

	//요소를 1개씩 기록
	auto it = class_attr->Begin();
	auto endit = class_attr->End();
	for( ; it != endit ; ++it) {
		const Attribute &attr = *it;
		int offset = attr.offset();
		char buffer[1024 * 4];

		TiXmlElement *child = new TiXmlElement(attr.name().c_str());
		TiXmlText *text = new TiXmlText("");

		switch(attr.type()) {
		case kAttributeInt:
			{
				int *ptr = (int*)((unsigned char*)data + offset);
				sprintf(buffer, "%d", *ptr);
				text->SetValue(buffer);
			}
			break;
		case kAttributeFloat:
			{
				float *ptr = (float*)((unsigned char*)data + offset);
				sprintf(buffer, "%f", *ptr);
				text->SetValue(buffer);
			}
			break;
		case kAttributeBool:
			{
				bool *ptr = (bool*)((unsigned char*)data + offset);
				if(*ptr == true) {
					text->SetValue("t");
				} else {
					text->SetValue("f");
				}
			}
			break;
		case kAttributeString:
		case kAttributeStringEnum:
			{
				std::string *ptr = (string*)((unsigned char*)(data) + offset);
				text->SetValue(ptr->c_str());
			}
			break;
		case kAttributeStringList:
			{
				vector<string> *ptr = (vector<string> *)((unsigned char*)(data) + offset);
				ostringstream oss;
				for(size_t i = 0 ; i < ptr->size() ; i++) {
					string &str = ptr->at(i);
					oss << str;
					if(i != ptr->size() - 1) {
						oss << STRING_LIST_DELIM;	// 구분자
					}
				}
				string str = oss.str();
				text->SetValue(str.c_str());
			}
			break;
		default:
			IUASSERT(!"not valid type");
			break;
		}

		child->LinkEndChild(text);
		elem->LinkEndChild(child);
	}
}

BaseDataPtr DataHelper::XmlNodeToData(TiXmlElement *elem)
{
	//<class_name base="base_name" version="version">...
	string class_name = elem->Value();
	//string base_name = elem->Attribute("base");
	//int version = -1;
	//elem->Attribute("version", &version);

	ClassDict &dict = ClassDict::GetInstance();
	ClassAttribute *class_attr = dict.Get(class_name);
	if(class_attr == NULL) {
		BaseDataPtr empty;
		return empty;
	}
	IUASSERT(class_attr != NULL);

	//내용 채울 부분
	BaseDataPtr result(class_attr->Clone());
	BaseData *data = result.get();
	//최초에 읽는 항목이 꽉채워지지 않앗을수 잇으니까 한번 정리
	Reset(data);

	IUASSERT(data != NULL);

	TiXmlNode *first = elem->FirstChild();
	TiXmlNode *child = NULL;
	for(child = first ; child ; child = child->NextSibling()) {
		if(child->Type() != TiXmlNode::TINYXML_ELEMENT) {
			continue;
		}
		TiXmlNode *text_node = child->FirstChild();
		if(text_node == NULL) {
			continue;
		}
		if(text_node->Type() != TiXmlNode::TINYXML_TEXT) {
			continue;
		}

		string attr_name = child->Value();
		string text = text_node->Value();

		Attribute *attr = class_attr->Get(attr_name);
		if(attr == NULL) {
			continue;
		}
		int offset = attr->offset();

		typedef unsigned char byte;
		switch(attr->type()) {
		case kAttributeBool:
			{
				bool *ptr = (bool*)((byte*)(data) + offset);
				if(text == "t") {
					*ptr = true;
				} else {
					*ptr = false;
				}
			}
			break;
		case kAttributeInt:
			{
				int *ptr = (int*)((byte*)(data) + offset);
				*ptr = atoi(text.c_str());
			}
			break;
		case kAttributeFloat:
			{
				float *ptr = (float*)((byte*)data + offset);
				*ptr = (float)atof(text.c_str());
			}
			break;
		case kAttributeString:
		case kAttributeStringEnum:
			{
				string *ptr = (string*)((byte*)data + offset);
				*ptr = text;
			}
			break;
		case kAttributeStringList:
			{
				vector<string> *ptr = (vector<string>*)((byte*)(data) + offset);
				StringUtil::Split(text, *ptr, string(STRING_LIST_DELIM));
			}
			break;
		default:
				break;	
		}
	}
	return result;
}

bool DataHelper::Equal(BaseData *a, BaseData *b) 
{
	IUASSERT(a != NULL);
	IUASSERT(b != NULL);
	if(a == b) {
		return true;
	}
	if(a->class_hash != b->class_hash) {
		return false;
	}

	ClassDict &dict = ClassDict::GetInstance();
	ClassAttribute *class_attr = dict.Get(a->class_hash);
	IUASSERT(class_attr != NULL);

	auto it = class_attr->Begin();
	auto endit = class_attr->End();
	for( ; it != endit ; it++) {
		const Attribute &attr = *it;

		AttributeType attr_type = attr.type();
		int offset = attr.offset();

		typedef unsigned char byte;
		switch(attr_type) {
		case kAttributeBool:
			{
				bool *ptr_a = (bool*)((byte*)(a) + offset);
				bool *ptr_b = (bool*)((byte*)(b) + offset);
				if(*ptr_a != *ptr_b) {
					return false;
				}
			}
			break;

		case kAttributeInt:
			{
				int *ptr_a = (int*)((byte*)(a) + offset);
				int *ptr_b = (int*)((byte*)(b) + offset);
				if(*ptr_a != *ptr_b) {
					return false;
				}
			}
			break;

		case kAttributeFloat:
			{
				float *ptr_a = (float*)((byte*)(a) + offset);
				float *ptr_b = (float*)((byte*)(b) + offset);
				if(*ptr_a != *ptr_b) {
					return false;
				}
			}
			break;

		case kAttributeString:
		case kAttributeStringEnum:
			{
				string *ptr_a = (string*)((byte*)(a) + offset);
				string *ptr_b = (string*)((byte*)(b) + offset);
				if(*ptr_a != *ptr_b) {
					return false;
				}
			}
			break;
		case kAttributeStringList:
			{
				vector<string> *ptr_a = (vector<string>*)((byte*)a + offset);
				vector<string> *ptr_b = (vector<string>*)((byte*)b + offset);
				if(*ptr_a != *ptr_b) {
					return false;
				}
			}
			break;
			default:
				break;
		}
	}

	return true;
}

void DataHelper::Reset(BaseData *data)
{
	IUASSERT(data != NULL);

	ClassDict &dict = ClassDict::GetInstance();
	ClassAttribute *class_attr = dict.Get(data->class_hash);
	IUASSERT(class_attr != NULL);

	auto it = class_attr->Begin();
	auto endit = class_attr->End();

	for( ; it != endit ; it++) {
		const Attribute &attr = *it;

		AttributeType attr_type = attr.type();
		int offset = attr.offset();

		typedef unsigned char byte;
		if(attr_type == kAttributeBool) {
			bool *ptr = (bool*)((byte*)(data) + offset);
			*ptr = false;

		} else if(attr_type == kAttributeInt) {
			int *ptr = (int*)((byte*)(data) + offset);
			*ptr = 0;

		} else if(attr_type == kAttributeFloat) {
			float *ptr = (float*)((byte*)(data) + offset);
			*ptr = 0.0f;

		} else if(attr_type == kAttributeStringEnum) {
			string *ptr = (string*)((byte*)(data) + offset);
			*ptr = attr.GetDefaultStrEnum();
		}
	}
}

struct ClassVersionData {
	int class_hash;
	int base_hash;
	int type_code;
};

template<int index>
class TypeTableGenerator {
public:
	static void Run(vector<ClassVersionData> &type_table) {
		TypeTableGenerator<index-1>::Run(type_table);
		typedef typename TypeListTypeAt<StructHeaderTypeList, index>::Result ClassType;
		Register<ClassType>(type_table);
	}

	template<typename T>
	static void Register(vector<ClassVersionData> &type_table) {
		int class_hash = T::ClassHash();
		int type_code = GetStructType<T>::value;
		int base_hash = T::BaseHash();
		ClassVersionData data;
		data.class_hash = class_hash;
		data.type_code = type_code;
		data.base_hash = base_hash;

		type_table.push_back(data);
	}
};
template<>
class TypeTableGenerator<0> {
public:
	static void Run(vector<ClassVersionData> &type_table) {
		typedef TypeListTypeAt<StructHeaderTypeList, 0>::Result ClassType;
		TypeTableGenerator<1>::Register<ClassType>(type_table);
	}
};

int DataHelper::GetStructTypeCode(BaseData *data)
{
	int base_hash = data->base_hash;

	static int init = false;
	
	//class_hash, basetype_code
	static vector<ClassVersionData> type_table;
	if(init == false) {
		init = true;
		
		const int type_list_count = TypeListLength<StructHeaderTypeList>::value;
		TypeTableGenerator<type_list_count-1>::Run(type_table);
	}

	for(size_t i = 0 ; i < type_table.size() ; i++) {
		auto tpl = type_table[i];
		if(tpl.base_hash == base_hash) {
			return tpl.type_code;
		}
	}
	//IUASSERT(!"not valid type");
	return -1;
}
void DataHelper::WriteType(BaseData *data, int type)
{
	ClassDict &dict = ClassDict::GetInstance();
	ClassAttribute *class_attr = dict.Get(data->class_hash);
	IUASSERT(class_attr != NULL);

	Attribute *attr = class_attr->Get("type");
	IUASSERT(attr != NULL);
	IUASSERT(attr->type() == kAttributeInt);

	int *type_ptr = (int*)((unsigned char*)(data) + attr->offset());
	*type_ptr = type;
}

int DataHelper::GetType(BaseData *data)
{
	ClassDict &dict = ClassDict::GetInstance();
	ClassAttribute *class_attr = dict.Get(data->class_hash);
	IUASSERT(class_attr != NULL);

	Attribute *attr = class_attr->Get("type");
	IUASSERT(attr != NULL);
	IUASSERT(attr->type() == kAttributeInt);

	int *type_ptr = (int*)((unsigned char*)(data) + attr->offset());
	return *type_ptr;
}
}