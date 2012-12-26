// Ŭnicode please	
//테스트라든가하는곳에서 쓰이는거는 기본적으로 내장시킨다
//어차피 이 시스템 자체가 메모리를 그닥 먹지 않으니까 내장시켜도 큰 문제는 없을것이다
#include "default_data_file.h"

/////////////////////////////////////////////
// 니가 쓸거는 여기부터 선언 가능함. 위의 내장용을 참고해서 만들면됨ㅋ

STRUCT_NEW_BEGIN(PlayerObjectHeader, 1)
    STRUCT_VAR_FLOAT(x)
    STRUCT_VAR_FLOAT(y)
    STRUCT_VAR_FLOAT(angle_rad)
    STRUCT_VAR_FLOAT(hit_point)
    STRUCT_VAR_STRING(sprite_name)  //스프라이트 시트 쓸 때는?
    STRUCT_END()    


STRUCT_NEW_BEGIN(BulletObjectHeader, 1)
    STRUCT_VAR_BOOL(from_enemy)
    STRUCT_VAR_FLOAT(x)
    STRUCT_VAR_FLOAT(y)
    STRUCT_VAR_FLOAT(angle_rad)
    STRUCT_VAR_FLOAT(speed)
    STRUCT_VAR_STRING(sprite_name)  //스프라이트 시트 쓸 때는?
    STRUCT_VAR_FLOAT(damage)
STRUCT_END()

STRUCT_NEW_BEGIN(CombatPlaneObjectHeader, 1)
    STRUCT_VAR_BOOL(is_enemy)
    STRUCT_VAR_FLOAT(angle)
    STRUCT_VAR_FLOAT(x)
    STRUCT_VAR_FLOAT(y)
    STRUCT_VAR_FLOAT(hit_point)
    STRUCT_VAR_STRING(sprite_name)  //스프라이트 시트 쓸 때는?
STRUCT_END()    

STRUCT_NEW_BEGIN(ObjectHeader, 1)
    STRUCT_VAR_FLOAT(x)
    STRUCT_VAR_FLOAT(y)
    STRUCT_VAR_STRING(sprite_name)  //스프라이트 시트 쓸 때는?
STRUCT_END()    

STRUCT_NEW_BEGIN(ShieldHeader, 1)
    STRUCT_VAR_FLOAT(x)
    STRUCT_VAR_FLOAT(y)
    STRUCT_VAR_FLOAT(hit_point)
    STRUCT_VAR_FLOAT(duration)
    STRUCT_VAR_INT(target_id)
STRUCT_END()

STRUCT_NEW_BEGIN(FormationHeader, 1)
STRUCT_END()

//로직 객체들
STRUCT_NEW_BEGIN(GameTriggerObjectHeader, 1)
STRUCT_END()