#pragma once

#include <engine/assert.h>
#include <engine/log.h>

#define VK_CHECK_RES(resultValue) ASSERT_FMT(resultValue.result == vk::Result::eSuccess, "vK: failed result: {}", vk::to_string(resultValue.result))
#define VK_CHECK(code) ASSERT_FMT(code == vk::Result::eSuccess, "vK: failed result: {}", vk::to_string(code))

//#define VK_CHECK_RES(resultValue) (void)resultValue
//#define VK_CHECK(code) (void)code