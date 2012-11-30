// Ŭnicode please	
//데이터를 선언만 하지 직접인클루드 하지는 않을것

//반쯤 예제로 쓸 구조체
STRUCT_BEGIN(DemoStruct, 1)
	STRUCT_VAR_INT(a)
	STRUCT_VAR_FLOAT(b)
	STRUCT_VAR_BOOL(c)
	STRUCT_VAR_STRING(d)
	STRUCT_VAR_STRING_LIST(f)
STRUCT_END()

STRUCT_NEW_BEGIN(DemoStruct, 2)
	STRUCT_VAR_STR_ENUM_BEGIN(c)
		STRUCT_VAR_STR_ENUM_KEY(1)
		STRUCT_VAR_STR_ENUM_KEY(2)
		STRUCT_VAR_STR_ENUM_KEY(3)
	STRUCT_VAR_STR_ENUM_END()
STRUCT_END()


//// 실제로 게임에서 사용할 자동생성 구조체를 적절히 정의하기
//string:name, int:type 은 컴포넌트에 따라서 공통속성
//type를 먼저두면 캐스팅한거에서 바로 접근 가능(오프셋이 0이니까)
//는 망한다(아래의 클래스는 가상함수 잇는거 상속이라서
//오프셋0으로 무식한 접근하면 뒤진다
STRUCT_NEW_BEGIN(VCSDemoHeader, 2)
	STRUCT_VAR_INT(type)
	STRUCT_VAR_STRING(name)
  STRUCT_VAR_FLOAT(fvalue) 
STRUCT_END()

//구버전 업그레이드가 지원되나 볼까
STRUCT_BEGIN(VCSDemoHeader, 1)
	STRUCT_VAR_INT(type)
	STRUCT_VAR_STRING(name)
  STRUCT_VAR_INT(ivalue)
STRUCT_END()

// unittest에서 참고하기 위해서 필요한거 돌리기 위해서 최소한으로 필요한거
STRUCT_NEW_BEGIN(TerrainHeader, 2)
 	STRUCT_VAR_INT(type)
 	STRUCT_VAR_STRING(name)
 STRUCT_END()

STRUCT_NEW_BEGIN(PolyBlockHeader, 1)
  STRUCT_VAR_INT(type)
  STRUCT_VAR_STRING(name)
	STRUCT_VAR_STRING(texture_file)
	STRUCT_VAR_BOOL(tap_breakable)
	STRUCT_VAR_FLOAT(angle_editor) //에디터에서만 쓰는 angle 값
  STRUCT_VAR_STRING_LIST(str_list)
STRUCT_END()

STRUCT_NEW_BEGIN(CheeseHeader, 1)
	STRUCT_VAR_INT(type)
	STRUCT_VAR_STRING(name)	
STRUCT_END()

STRUCT_NEW_BEGIN(BalloonHeader, 1)
	STRUCT_VAR_INT(type)
	STRUCT_VAR_STRING(name)
STRUCT_END()

STRUCT_NEW_BEGIN(CoinHeader, 1)
	STRUCT_VAR_INT(type)
	STRUCT_VAR_STRING(name)
STRUCT_END()


/////////////////////////////////////////////
// 니가 쓸거는 여기부터 선언 가능함