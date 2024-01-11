#pragma once

#include <engine/types.h>
#include <engine/utils/concept.hpp>
#include <engine/utils/strong_typedef.h>

STRONG_SCALAR_TYPEDEF(uint16_t, engine_data_type_id_t);

#define ED_BASE_VALUE_TYPES int, int2, int3, int4, float, float2, float3, float4, string

template<class T>
concept IsEdVarCustomType = !Utils::IsAnyOf<std::remove_cvref_t<T>, ED_BASE_VALUE_TYPES>;
