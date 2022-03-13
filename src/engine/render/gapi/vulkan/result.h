#pragma once

#include <engine/assert.h>

#define VK_CHECK_RES(resultValue) ASSERT(resultValue.result == vk::Result::eSuccess)
