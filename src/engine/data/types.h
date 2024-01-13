#pragma once

#include <engine/types.h>
#include <engine/utils/concept.hpp>
#include <engine/utils/strong_typedef.h>

STRONG_SCALAR_TYPEDEF(uint16_t, engine_data_type_id_t);
constexpr engine_data_type_id_t INVALID_ED_TYPE_ID = engine_data_type_id_t{(uint16_t)~0};

#define ED_BASE_VALUE_TYPES bool, int, int2, int3, int4, float, float2, float3, float4, string, float3x3, float4x4

template<class T>
concept IsEdVarCustomType = !Utils::IsAnyOf<std::remove_cvref_t<T>, ED_BASE_VALUE_TYPES>;

namespace ed
{
  enum class ValueType : uint8_t
  {
    Bool, Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    Text, Mat3, Mat4,
    TypeConstructor,
  };
}