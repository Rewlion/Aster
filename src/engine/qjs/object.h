#pragma once

#include "forward_decl.h"

#include <string_view>

namespace qjs
{
  class ObjectView
  {
    public:
      ObjectView(const Value&);
      JSValue getJsProperty(const std::string_view);
      Value getProperty(const std::string_view);
      float getFloatOr(const std::string_view, const float);
      int getIntOr(const std::string_view, const int);
      bool getBoolOr(const std::string_view, const bool);
      string getTextOr(const std::string_view, string&&);

      template<class T>
      T getIntEnumOr(const std::string_view name, const T def)
      {
        const int max = (int)T::LAST;
        const int val = getRangedIntOr(name, (int)def, max);
        return (T)val;
      }

    private:
      int getRangedIntOr(const std::string_view, const int def, const int max);

    private:
      const Value& m_Value;
  };
}
