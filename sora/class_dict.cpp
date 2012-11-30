// Ŭnicode please
#include "sora_stdafx.h"
#include "class_dict.h"
#include <algorithm>
#include "template_lib.h"
#include "class_attribute.h"
#include "data_define.h"
#include "assert_include.h"

using namespace std;

namespace sora {;

/////////////////////////////////////////////////////

ClassDict::ClassDict()
{
}
ClassDict::~ClassDict()
{
}
void ClassDict::Clear()
{
	class_attr_list_.clear();
	version_dict_.clear();
}

void ClassDict::SetClassVersionInfo(const std::string &base_name, const std::string &class_name, int version)
{
	auto version_pair = make_pair(class_name, version);
	version_dict_.insert(make_pair(base_name, version_pair));
}

bool ClassDict::IsBaseClassExist(const std::string &base_name) const 
{
	auto found = version_dict_.find(base_name);
	return (found != version_dict_.end());
}
bool ClassDict::IsClassExist(const std::string &class_name) const
{
	auto it = version_dict_.begin();
	auto endit = version_dict_.end();
	for( ; it != endit ; ++it) {
		if(it->second.first == class_name) {
			return true;
		}
	}
	return false;
}

std::string ClassDict::GetLatestClassName(const std::string &base_name) const
{
	auto ret = version_dict_.equal_range(base_name);
	if(ret.first == version_dict_.end()) {
		static string empty;
		return empty;
	}
	string latest_name;
	int version = -1;
	for(auto it = ret.first ; it != ret.second ; ++it) {
		if(it->second.second > version) {
			latest_name = it->second.first;
			version = it->second.second;
		}
	}
	return latest_name;
}

std::string ClassDict::GetName(const std::string &base_name, int version) const
{
	auto ret = version_dict_.equal_range(base_name);
	if(ret.first == version_dict_.end()) {
		return string("");
	}
	for(auto it = ret.first ; it != ret.second ; ++it) {
		if(it->second.second == version) {
			return it->second.first;
		}
	}
	return string("");
}

bool ClassDict::Add(const ClassAttribute &class_attr)
{
	auto it = class_attr_list_.begin();
	auto endit = class_attr_list_.end();
	for( ; it != endit ; ++it) {
		if(it->class_name() == class_attr.class_name()) {
			//같은 이름 클래스 추가 불가능
			return false;
		}
	}
	class_attr_list_.push_back(class_attr);
	//클래스 추가될때 정보를 기반으로 버전도 자체 설정
	SetClassVersionInfo(class_attr.base_name(), class_attr.class_name(), class_attr.version());
	return true;
}

std::set<std::string> ClassDict::GetBaseClassList() const {
	std::set<std::string> class_set;
	auto it = version_dict_.begin();
	auto endit = version_dict_.end();
	for( ; it != endit ; ++it) {
		class_set.insert(it->first);
	}
	return class_set;
}
ClassAttribute *ClassDict::Get(unsigned int hash)
{
	auto it = class_attr_list_.begin();
	auto endit = class_attr_list_.end();
	for( ; it != endit ; ++it) {
		if(it->Hash() == hash) {
			return &(*it);
		}
	}
	return NULL;
}

ClassAttribute *ClassDict::Get(const std::string &class_name)
{
	auto it = class_attr_list_.begin();
	auto endit = class_attr_list_.end();
	for( ; it != endit ; ++it) {
		if(it->class_name() == class_name) {
			return &(*it);
		}
	}
	return NULL;
}


const ClassAttribute *ClassDict::Get(unsigned int hash) const
{
	auto it = class_attr_list_.begin();
	auto endit = class_attr_list_.end();
	for( ; it != endit ; ++it) {
		if(it->Hash() == hash) {
			return &(*it);
		}
	}
	return NULL;
}

const ClassAttribute *ClassDict::Get(const std::string &class_name) const
{
	auto it = class_attr_list_.begin();
	auto endit = class_attr_list_.end();
	for( ; it != endit ; ++it) {
		if(it->class_name() == class_name) {
			return &(*it);
		}
	}
	return NULL;
}


bool ClassDict::IsLatestClassName(const std::string &class_name) const
{
	//version 정보에서 적절히 긁기
	auto it = version_dict_.begin();
	auto endit = version_dict_.end();
	int curr_version = -1;
	for( ; it != endit ; ++it) {
		if(it->second.first == class_name) {
			curr_version = it->second.second;
			break;
		}
	}
	IUASSERT(curr_version != -1);

	string lastest_name = GetLatestClassName(it->first);
	if(lastest_name == it->second.first) {
		return true;
	} else {
		return false;
	}
}
}
