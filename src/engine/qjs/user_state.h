#pragma once

#include "forward_decl.h"

namespace qjs
{
  void* get_user_state_ptr(JSContext*);
  
  template<class T>
  T* get_user_state(JSContext* ctx)
  {
    return reinterpret_cast<T*>(get_user_state_ptr(ctx));
  }
}
