#pragma once

#include <engine/time.h>
#include <engine/types.h>

#ifdef DEBUG
#include <EASTL/bonus/fixed_ring_buffer.h>

namespace Engine::Render::dbg
{
  struct QueuedText
   {
     string text;
     float4 color;
     uint64_t expireAt;
   };

  template <size_t N, size_t MAX_TEXT_LEN>
  class TextDbgQueue
  {
    public:
      using ThisType = TextDbgQueue<N, MAX_TEXT_LEN>;
      using Container = eastl::ring_buffer<QueuedText>;

    public:
      TextDbgQueue()
        : m_Queue(N)
      {
      }

      void tick()
      {
        const uint64_t t = Time::get_time_since_start();

        auto front = m_Queue.begin();
        while (front != m_Queue.end() && front->expireAt <= t)
        {
          m_Queue.pop_front();
          front = m_Queue.begin();
        }
      }

      void push(string&& text, const float4 color)
      {
        if (text.size() > MAX_TEXT_LEN)
          text = string(text.c_str(), MAX_TEXT_LEN);

        const uint64_t secToLive = 4;
        m_Queue.push_back();
        m_Queue.back() = QueuedText{
          std::move(text),
          color,
          Time::get_time_since_start() + Time::convert_sec_to_clocks(secToLive)
        };
      }

      typename Container::const_iterator begin() const
      {
        return m_Queue.begin();
      }

      typename Container::const_iterator end() const
      {
        return m_Queue.end();
      }

    private:
     Container m_Queue;
  };
}
#else
namespace Engine::Render::dbg
{
  template <size_t N, size_t MAX_TEXT_LEN>
  class TextDbgQueue
  {
    public:
      using ThisType = TextDbgQueue<N, MAX_TEXT_LEN>;
      using Container = eastl::fixed_ring_buffer<QueuedText, N>;

    public:
      void tick()
      {
      }

      void push(string&& text, const float4 color)
      {
      }

      typename Container::reverse_iterator begin() const
      {
        return Container::reverse_iterator{};
      }

      typename Container::reverse_iterator end() const
      {
        return Container::reverse_iterator{};
      }
  };
}
#endif
