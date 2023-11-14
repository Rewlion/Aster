#pragma once

#include "config.h"

#if CFG_VMA_ASSERTS
#include <engine/assert.h>
#define VMA_ASSERT ASSERT
#define VMA_HEAVY_ASSERT ASSERT
#else
#define VMA_ASSERT(exp) 
#define VMA_HEAVY_ASSERT(exp) 
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
#include <vma/vk_mem_alloc.h>
#pragma clang diagnostic pop
