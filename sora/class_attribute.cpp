// Ŭnicode please
#include "sora_stdafx.h"
#include "class_attribute.h"
#include <algorithm>
#include "data_define.h"

using namespace std;

namespace sora {;
Attribute Attribute::Int(const std::string &name, int offset)
{
	return Attribute(name, kAttributeInt, offset);
}
Attribute Attribute::Bool(const std::string &name, int offset)
{
	return Attribute(name, kAttributeBool, offset);
}
Attribute Attribute::Float(const std::string &name, int offset)
{
	return Attribute(name, kAttributeFloat, offset);
}
Attribute Attribute::String(const std::string &name, int offset)
{
	return Attribute(name, kAttributeString, offset);
}
Attribute Attribute::Vec2(const std::string &name, int offset)
{
	return Attribute(name, kAttributeVec2, offset);
}
Attribute Attribute::Vec2List(const std::string &name, int offset)
{
	return Attribute(name, kAttributeVec2List, offset);
}
Attribute Attribute::StringList(const std::string &name, int offset)
{
	return Attribute(name, kAttributeStringList, offset);
}
Attribute Attribute::StringEnum(const std::string &name, int offset)
{
	return Attribute(name, kAttributeStringEnum, offset);
}

Attribute::Attribute()
	: name_(""), type_(kAttributeNone), offset_(0) { }

Attribute::Attribute(const std::string &name, AttributeType type, int offset)
	: name_(name), type_(type), offset_(offset) { }
Attribute::~Attribute() {}

bool Attribute::IsUseEnum() const
{
	if(type_ == kAttributeStringEnum) {
		return true;
	} else {
		return false;
	}
}
void Attribute::AddStringEnum(const std::string &key)
{
	auto found = find(str_enum_list_.begin(), str_enum_list_.end(), key);
	if(found == str_enum_list_.end()) {
		str_enum_list_.push_back(key);
	}
}
bool Attribute::IsValidStrEnum(const std::string &name) const
{
	auto found = find(str_enum_list_.begin(), str_enum_list_.end(), name);
	return (found != str_enum_list_.end());
}
const std::string &Attribute::GetDefaultStrEnum() const
{
	if(str_enum_list_.empty()) {
		static string empty;
		return empty;
	} else {
		return str_enum_list_.front();
	}
}
/////////////////////////////////////////////
ClassAttribute::ClassAttribute(const std::string &base_name, const std::string &class_name, int version)
: base_name_(base_name),
class_name_(class_name),
version_(version)
{
}
ClassAttribute::~ClassAttribute() 
{
	//class dict에 등록된 다음에는 소멸자/생성자가 추적 불가능하니까 해제를 하지 말자
	//어차피 클래스는 1회 등록된 다음에 안생길테니까 큰 문제는 없을것이다
}
void ClassAttribute::set_prototype(BaseData *data) {
	prototype_ = std::shared_ptr<BaseData>(data);
}
BaseData *ClassAttribute::Clone() const
{
	if(prototype_.get() != NULL) {
		return prototype_->Clone();
	}
	return NULL;
}

bool ClassAttribute::Add(const Attribute &attr) {
	if(IsAttributeExist(attr.name()) == false) {
		attr_list_.push_back(attr);
		return true;
	} else {
		return false;
	}
}

bool ClassAttribute::IsAttributeExist(const std::string &attr_name) const {
	auto it = attr_list_.begin();
	auto endit = attr_list_.end();
	for(; it != endit ; ++it) {
		if(it->name() == attr_name) {
			return true;
		}
	}
	return false;
}
Attribute *ClassAttribute::Get(const std::string &attr_name) {
	auto it = attr_list_.begin();
	auto endit = attr_list_.end();
	for( ; it != endit ; ++it) {
		if(it->name() == attr_name) {
			return &(*it);
		}
	}
	return NULL;
}

unsigned int ClassAttribute::Hash() const
{
  return BaseData::HashFunction::Hash(class_name_.c_str(), class_name_.length());
}
}