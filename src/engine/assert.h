#pragma once

#include <engine/types.h>

#define ASSERT_COMMON(cond, isIgnorable)\
        do{\
          if ((cond) == false)\
            ShowAssertWindow(#cond, isIgnorable);\
        } while(false)

#define ASSERT(cond) ASSERT_COMMON(cond, false)

#define IASSERT(cond) ASSERT_COMMON(cond, true)

void ShowAssertWindow(String error, const bool isIgnorable = false);
