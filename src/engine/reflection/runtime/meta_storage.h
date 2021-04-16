#pragma once

#include "meta.h"
#include <iostream>
#include <map>

namespace Meta
{
  class MetaStorage
  {
  public:
    static inline MetaStorage* Instance()
    {
      static MetaStorage s;
      return &s;
    }

    inline void RegisterClass(const Class* c, const char* name)
    {
      m_NameClassMap[name] = c;
    }

  private:
    MetaStorage() {}
    ~MetaStorage() {}
    MetaStorage(const MetaStorage&){}
    MetaStorage& operator=(const MetaStorage&) = delete;

  private:
    std::map<const char*, const Class*> m_NameClassMap;
  };
}
