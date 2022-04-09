#pragma once

#include <engine/assert.h>

#define VK_CHECK_RES(resultValue) ASSERT(resultValue.result == vk::Result::eSuccess)
#define VK_CHECK(code) ASSERT(code == vk::Result::eSuccess)