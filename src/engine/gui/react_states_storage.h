#pragma once



#include <EASTL/vector_map.h>

namespace Engine::gui
{
  class ReactStateClass;

  class ReactStatesStorage
  {
    public:
      void add(ReactStateClass*);
      void remove(ReactStateClass*);

    private:
      eastl::vector_map<uint64_t, ReactStateClass*> m_RegisteredStates;
  };
}
