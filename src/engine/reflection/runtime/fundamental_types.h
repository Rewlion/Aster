#pragma once

#include "meta.h"

namespace Meta
{
#ifndef FUNDAMENTAL_META_TYPES_IMPLEMENTATION
  #define DECLARE_FUNDAMENTAL_TYPE(T,Name)\
      template<> const Type* TypeAccessor::GetTypeImpl<T>(TypeTag<T>);
#else
  #define DECLARE_FUNDAMENTAL_TYPE(T,Name)\
      template<> const Type* TypeAccessor::GetTypeImpl<T>(TypeTag<T>) { \
        static Type t { .name = Name, .size = sizeof(T) };\
        return &t;\
      }
#endif // !FUNDAMENTAL_META_TYPES_IMPLEMENTATION

#define DECLARE_FUNDAMENTAL_TYPES\
  DECLARE_FUNDAMENTAL_TYPE(bool,               "Bool")\
  DECLARE_FUNDAMENTAL_TYPE(char,               "Char")\
  DECLARE_FUNDAMENTAL_TYPE(short,              "Short")\
  DECLARE_FUNDAMENTAL_TYPE(int,                "Int")\
  DECLARE_FUNDAMENTAL_TYPE(long,               "Long")\
  DECLARE_FUNDAMENTAL_TYPE(long long,          "LongLong")\
  DECLARE_FUNDAMENTAL_TYPE(float,              "Float")\
  DECLARE_FUNDAMENTAL_TYPE(double,             "Double")\
  DECLARE_FUNDAMENTAL_TYPE(long double,        "LongDouble")\
  DECLARE_FUNDAMENTAL_TYPE(unsigned char,      "UnsignedChar")\
  DECLARE_FUNDAMENTAL_TYPE(unsigned short,     "UnsignedShort")\
  DECLARE_FUNDAMENTAL_TYPE(unsigned int,       "UnsignedInt")\
  DECLARE_FUNDAMENTAL_TYPE(unsigned long,      "UnsignedLong")\
  DECLARE_FUNDAMENTAL_TYPE(unsigned long long, "UnsignedLongLong")\

DECLARE_FUNDAMENTAL_TYPES

#undef DECLARE_FUNDAMENTAL_TYPES
#undef DECLARE_FUNDAMENTAL_TYPE
}
