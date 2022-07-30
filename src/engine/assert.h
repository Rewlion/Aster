#pragma once

#include <engine/types.h>

#define ASSERT_COMMON(cond, msg, isIgnorable)\
        do{\
          if ((cond) == false)\
            show_assert_window(msg, isIgnorable);\
        } while(false)

#define ASSERT(cond) ASSERT_COMMON(cond, #cond, false)

#define ASSERT_FMT(cond, f, ...) ASSERT_COMMON(cond, fmt::format(f, __VA_ARGS__), false)

#define IASSERT(cond) ASSERT_COMMON(cond, #cond, true)

void show_assert_window(string error, const bool isIgnorable = false);
