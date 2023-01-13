#include "user_state.h"
#include "inc.h"
#include "runtime_state.h"

namespace qjs
{
  void* get_user_state_ptr(JSContext* ctx)
  {
    auto* rtState = reinterpret_cast<RuntimeState*>(JS_GetRuntimeOpaque(JS_GetRuntime(ctx)));
    return rtState->userState;
  }
}
