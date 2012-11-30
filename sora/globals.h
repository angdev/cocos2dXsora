// Ŭnicode please
#pragma once

#include "arch.h"

#if SR_USE_PCH == 0
#include <vector>
#endif

namespace sora {;

typedef enum {
    kVertexNone,
    kVertex2D,
    kVertex,
    kVertexTangent,
    kVertexPos2D,
    kVertexPos3D,
    kVertexColor3D,   //color + 3d pos

    kVertexCodeCount,
} VertexCode;

typedef enum {
    kDrawPoints,
    kDrawLines,
    kDrawLineStrip,
    kDrawLineLoop,
    kDrawTriangles,
    kDrawTriangleStrip,
    kDrawTriangleFan,
    kDrawTypeCount,
} DrawType;

typedef enum {
    kTypeFloat,
    kTypeInt,
    kTypeUint,
    kTypeShort,
    kTypeUshort,
    kTypeByte,
    kTypeUbyte,

    kTypeFloatMat4,
    kTypeFloatMat3,
    kTypeFloatMat2,

    kTypeFloatVec4,
    kTypeFloatVec3,
    kTypeFloatVec2,
    kTypeIntVec4,
    kTypeIntVec3,
    kTypeIntVec2,

    kTypeSample2D, 
    kTypeSampleCube,
    kVarTypeCount,
} VarType;

//기본도형은 normal까지만 지원하낟. 그 이상의 정보가 필요하면
//버텍스 목록에 추가 가공을 다른방식으로 집어넣자
template<typename VertexType>
struct DrawCmdData {
    DrawCmdData() : disable_cull_face(false) { }
    bool disable_cull_face;
    DrawType draw_mode;
    std::vector<VertexType> vertex_list;
    std::vector<unsigned short> index_list;
};

}
