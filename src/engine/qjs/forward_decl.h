#pragma once

#include <engine/types.h>

struct JSRuntime;
struct JSContext;
struct JSModuleDef;
typedef uint64_t JSValue;

namespace qjs
{
  class VM;
  class Value;
  class ValueView;
  class ArrayView;
  class FunctionView;
  class ObjectView;
}