#pragma once

#include <engine/assert.h>
#define VMA_HEAVY_ASSERT ASSERT
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
#include <vma/vk_mem_alloc.h>
#pragma clang diagnostic pop
