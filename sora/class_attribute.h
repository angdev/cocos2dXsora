// Ŭnicode please
#pragma once

#include <string>
#include <vector>
#include <memory>

struct BaseData;

namespace sora {;
typedef enum {
	kAttributeNone,
	kAttributeBool,
	kAttributeString,
	kAttributeInt,
	kAttributeFloat,
	kAttributeVec2,
	kAttributeVec2List,
	kAttributeStringList,
	///@def 몇가지 문자열중 1개만으로 설정해야되는 경우
	///제약을 걸때 사용됨
	kAttributeStringEnum,
} AttributeType;

class Attribute {
public:
	static Attribute Int(const std::string &name, int offset);
	static Attribute Bool(const std::string &name, int offset);
	static Attribute Float(const std::string &name, int offset);
	static Attribute String(const std::string &name, int offset);
	static Attribute Vec2(const std::string &name, int offset);
	static Attribute Vec2List(const std::string &name, int offset);
	static Attribute StringList(const std::string &name, int offset);

	static Attribute StringEnum(const std::string &name, int offset);

public:
	Attribute(const std::string &name, AttributeType type, int offset);
	Attribute();
	~Attribute();

	const std::string &name() const { return name_ ; }
	int offset() const { return offset_; }
	AttributeType type() const { return type_; }
	//enum을 사용하는 타입인가 아닌가
	bool IsUseEnum() const;

private:
	int offset_;
	AttributeType type_;
	std::string name_;

	//어차피 맵에디터쪽에서만쓴다면 상속같은 귀찮은거 생각하지 않고 때려박고 안쓰는것도 좋을듯
	//상속을 넣으면 메모리 관리같은거때문에 구조가 우주로 가기 쉽다(그리고 이전버전은 그랫고)
public:
	typedef std::vector<std::string> StrEnumListType;
	void AddStringEnum(const std::string &key);
	int GetEnumCount() const { return str_enum_list_.size(); }
	bool IsValidStrEnum(const std::string &name) const;
	const std::string &GetDefaultStrEnum() const;
	const StrEnumListType &str_enum_list() const { return str_enum_list_; }

private:
	StrEnumListType str_enum_list_;
};

class ClassAttribute {
public:
	typedef std::vector<Attribute> AttributeListType;
	typedef AttributeListType::iterator Iterator;
	typedef AttributeListType::const_iterator ConstIterator;
public:
	ClassAttribute(const std::string &base_name, const std::string &class_name, int version);
	~ClassAttribute();

	unsigned int Hash() const;
	void set_prototype(BaseData *data);
	const std::string &base_name() const { return base_name_; }
	const std::string &class_name() const { return class_name_; }
	int version() const { return version_; }
	BaseData *Clone() const;

	//attribute등록 + 검색
	bool Add(const Attribute &attr);
	bool IsAttributeExist(const std::string &attr_name) const;
	Attribute *Get(const std::string &attr_name);
	int Count() const { return attr_list_.size(); }

	ConstIterator Begin() const { return attr_list_.begin(); }
	ConstIterator End() const { return attr_list_.end(); }
	Iterator Begin() { return attr_list_.begin(); }
	Iterator End() { return attr_list_.end(); }

private:
	std::string base_name_;
	std::string class_name_;
	int version_;

	std::shared_ptr<BaseData> prototype_;

	AttributeListType attr_list_;
};

}