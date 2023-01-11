#pragma once

#include <engine/types.h>

struct JSRuntime;
struct JSContext;
struct JSModuleDef;
typedef uint32_t JSClassID;
typedef uint64_t JSValue;
typedef JSValue JSValueConst;

namespace qjs
{
  class VM;
  class Value;
  class ValueView;
  class ArrayView;
  class FunctionView;
  class ObjectView;
}