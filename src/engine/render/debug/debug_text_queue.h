#pragma once

#include <engine/time.h>
#include <engine/types.h>

#include <EASTL/bonus/fixed_ring_buffer.h>

namespace Engine::Render::dbg
{
  struct QueuedText
  {
    string text;
    float4 color;
    uint64_t expireAt;
  };

  class TextDbgQueue
  {
    static constexpr size_t MAX_TEXT_LEN = 128;
    static constexpr size_t N = 10;

    public:
      using Container = eastl::ring_buffer<QueuedText>;

    public:
      TextDbgQueue()
      {}

      void init();
      void tick();
      void push(string&& text, const float4 color);
      auto begin() const -> typename Container::const_iterator;
      auto end() const -> typename Container::const_iterator;

    private:
     Container m_Queue;
  };
}
