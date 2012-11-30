// Ŭnicode please
#pragma once

//옛날의 stage header역할의 일부를 맡는다
//구조체 생성은 자동화가 되엇기떄문에 여기에는 게임과 툴에서 공유할 enum
//과 아직 안옮긴 코드만 둿다

#include <string>
#include <vector>
#include "vector.h"

#include "data_define.h"
#include "typelist.h"
#include "assert_include.h"

enum
{
	kStructNone,
	kStructDemo, 
	kStructTypeCount	//항상 마지막에 배치하기
};

template<typename T>
struct GetStructType { enum { value = kStructNone }; };

template<> struct GetStructType<VCSDemoHeader> { enum { value = kStructDemo }; };

//typelist로 어떻게 위와 같은 코드를 자동화 시킬수 잇을거같은데..
//linear joint, movable block , cheese는 아직 구현되어잇지 않아서 여기에 넣으면 문제가 생긴다

typedef TYPELIST_1(
	VCSDemoHeader
	) StructHeaderTypeList;
