#pragma once

#include <engine/types.h>

#define ASSERT_COMMON(cond, isIgnorable)\
        do{\
          if ((cond) == false)\
            show_assert_window(#cond, isIgnorable);\
        } while(false)

#define ASSERT(cond) ASSERT_COMMON(cond, false)

#define IASSERT(cond) ASSERT_COMMON(cond, true)

void show_assert_window(string error, const bool isIgnorable = false);
