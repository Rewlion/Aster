#pragma once

#include <engine/qjs/value.hpp>

#include <EASTL/vector_set.h>
#include <EASTL/vector_map.h>

namespace Engine::gui
{
  class Timer
  {
    public:
      Timer();
      Timer(Timer&&);
      Timer(qjs::Value&& cb, uint64_t secBeforeCall);
      Timer& operator=(Timer&&);
      ~Timer();
      void tick();

    private:
      qjs::Value m_Cb;

      uint64_t m_ClocksBeforeCall;
      uint64_t m_NextCallTime;
  };

  class TimersPool
  {
    public:
      void add(const string& name, qjs::Value&& cb, uint64_t secBeforeCall);
      void remove(const string& name);
      void tick();

    private:
      void clearTimers();

    private:
      eastl::vector_set<string> m_TimersToRemove;
      eastl::vector_map<string, Timer> m_Timers;
  };
}
