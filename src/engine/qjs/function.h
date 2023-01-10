#pragma once

#include "forward_decl.h"

namespace qjs
{
  class FunctionView
  {
    public:
      FunctionView(const Value& v);
      Value operator()(const Value& this_obj, const uint argc, const Value* argv) const;

      inline int64_t getArgsCount() const
      {
        return m_ArgsCount;
      }

    private:
      const Value& m_Value;
      int64_t m_ArgsCount;
  };
}
