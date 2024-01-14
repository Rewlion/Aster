#pragma once

#include <engine/types.h>
#include <engine/utils/concept.hpp>
#include <engine/utils/strong_typedef.h>

#include <EASTL/vector.h>
#include <EASTL/span.h>

STRONG_SCALAR_TYPEDEF(uint16_t, engine_data_type_id_t);
constexpr engine_data_type_id_t INVALID_ED_TYPE_ID = engine_data_type_id_t{(uint16_t)~0};

namespace ed
{
  enum class ValueType : uint8_t
  {
    Bool, Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    Text, Mat3, Mat4,
    IntArray, FloatArray, TextArray,
    TypeConstructor,
  };

  using IntArray = eastl::vector<int>;
  using FloatArray = eastl::vector<float>;
  using TextArray = eastl::vector<string>;

  using IntArrayView = eastl::span<const int>;
  using FloatArrayView = eastl::span<const float>;
  using TextArrayView = eastl::span<const string>;
}

#define ED_BASE_VALUE_TYPES bool,\
                            int, int2, int3, int4,\
                            float, float2, float3, float4,\
                            string, float3x3, float4x4, \
                            ed::IntArray, ed::FloatArray, ed::TextArray

#define ED_BASE_VIEW_VALUE_TYPES string_view, ed::IntArrayView,\
                                 ed::FloatArrayView, ed::TextArrayView

template<class T>
concept IsEdVarCustomType = !Utils::IsAnyOf<std::remove_cvref_t<T>, ED_BASE_VALUE_TYPES, ED_BASE_VIEW_VALUE_TYPES>;
