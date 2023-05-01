#pragma once

#include <engine/log.h>
#include <engine/types.h>

#define ASSERT_COMMON(cond, msg, isIgnorable)\
        do{\
          if ((cond) == false)\
            show_assert_window(msg, isIgnorable);\
        } while(false)

#define ASSERT(cond) ASSERT_COMMON(cond, #cond, false)

#define ASSERT_FMT(cond, f, ...) ASSERT_COMMON(cond, fmt::format(f, __VA_ARGS__), false)
#define ASSERT_FMT_RETURN(cond, ret_val, f, ...)  ASSERT_FMT(cond, f, __VA_ARGS__); if (!(cond)) return ret_val

#define IASSERT(cond) ASSERT_COMMON(cond, #cond, true)

void show_assert_window(string error, const bool isIgnorable = false);
