// Ŭnicode please
#pragma once

#include <set>
#include <string>
#include <vector>
#include "shared_object.h"
#include "class_attribute.h"
#include <unordered_map>

struct BaseData;

namespace sora {;
//클래스이름-attribute로 연결되니까
//중간클래스를 도입해서 복잡도를 늘리기보다
//multimap으로 적절히 때울수 잇을거같다
//이 클래스 하나로 클래스사용가능한 클래스 목록. 내부에 들어잇는 변수목록까지
//전부 때우자. 이걸 여러 클래스로 나누면 그것도 그거 나름대로 복잡해 질거같아서..
class ClassDict : public SharedObject<ClassDict> {
public:
	//컴포넌트마다 버전이 잇고 버전별로 여러개의 조합이 생길수 잇으니까 이렇게 
	//사전을 하나 추가해서 데이터 관리로 쓸수 잇도록햇다
	//key : BeltHeader
	//value : pair<VeltHeader_v1, 1> 같은 식으로 되도록함
	typedef std::unordered_multimap<std::string, std::pair<std::string, int> > VersionDictType;
	typedef std::vector<ClassAttribute> ClassAttributeListType;
public:
	ClassDict();
	~ClassDict();
	void Clear();

	// BeltHeader, BeltHeader_v1, 1 같이 특정 클래스에 묶이 이름을 등록해서 얻을수 잇도록하자
	// 이게 되야 버전업/다운을 할수 잇을거같다
	void SetClassVersionInfo(const std::string &base_name, const std::string &class_name, int version);
	bool IsBaseClassExist(const std::string &base_name) const;
	std::string GetLatestClassName(const std::string &base_name) const;
	bool IsLatestClassName(const std::string &class_name) const;
	
	//특정 base class에 특정 버전의 클래스 이름 얻기
	//이름 생성 규칙을 어딘가에 묶어서 할 경우
	//매크로라든가를 거칠때 귀찮을거같아서 완전히 분리함
	//못찾으면 빈 문자열
    //근데 GetClassName은 윈도우 내장 매크로 어딘가에 같은 이름이 존재한다
    //덕분에 아작난다. 그래서 다른 이름을 사용함
	std::string GetName(const std::string &base_name, int version) const;
	
	//구현체 클래스 이름 얻기
	bool IsClassExist(const std::string &class_name) const;

	ClassAttribute *Get(const std::string &class_name);
	ClassAttribute *Get(unsigned int hash);
	const ClassAttribute *Get(const std::string &class_name) const;
	const ClassAttribute *Get(unsigned int hash) const;

	bool Add(const ClassAttribute &class_attr);

	std::set<std::string> GetBaseClassList() const;

private:
	VersionDictType version_dict_;
	ClassAttributeListType class_attr_list_;
};

}