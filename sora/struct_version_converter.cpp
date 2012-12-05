// Ŭnicode please
#include "sora_stdafx.h"
#include "struct_version_converter.h"
#include "class_dict.h"
#include "class_attribute.h"
#include "assert_include.h"

using namespace std;
using namespace sora;

std::vector<StructConverterParam> StructVersionConverter::converter_list_;
std::vector<StructConverterParam> &StructVersionConverter::GetConverterList()
{
	if(converter_list_.empty()) {
		//converter 적절히 등록하기
		RegisterConverter<Converter_VCSDemoHeaderV1_VCSDemoHeaderV2>();
	}
	return converter_list_;
}

BaseDataPtr StructVersionConverter::ConvertToLatest(BaseDataPtr from) {
  //최초에 적절히 생성하기. 여기에서 먼저 생성해준다
	std::vector<StructConverterParam> &converter_list = GetConverterList();
	ClassDict &dict = ClassDict::GetInstance();

	while(true) {
		//최신버전이면 적절히 종료
		ClassAttribute *from_class_attr = dict.Get(from->class_hash);
		if(dict.IsLatestClassName(from_class_attr->class_name())) {
			return from;
		}

		auto it = converter_list.begin();
		auto endit = converter_list.end();
		bool convert_occur = false;
		for( ; it != endit ; ++it) {
			if(it->from_class_hash == from->class_hash) {
				StructVersionConverter *convert = it->conveter;
				from = convert->Convert(from.get());
				convert_occur = true;
			}
		}
		IUASSERT(convert_occur == true);
	}
}

void Converter_VCSDemoHeaderV1_VCSDemoHeaderV2::Convert(const FromType &from, ToType &to)
{
	to.type = from.type;
	to.name = from.name;
    to.fvalue = (float)from.ivalue;
}
