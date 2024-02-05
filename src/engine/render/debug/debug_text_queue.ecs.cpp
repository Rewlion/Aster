#include "debug_text_queue.h"

#include <engine/ecs/type_meta.h>
#include <engine/ecs/macros.h>
#include <engine/events_render.h>

ECS_EVENT_SYSTEM()
void dbg_queue_tick(const Engine::OnBeforeRender&,
                    Engine::Render::dbg::TextDbgQueue& dbg_text_queue)
{
  dbg_text_queue.tick();
}

namespace Engine::Render::dbg
{
  void TextDbgQueue::init()
  {
    m_Queue = Container{N};
  }

  void TextDbgQueue::tick()
  {
    const uint64_t t = Time::get_time_since_start();

    auto front = m_Queue.begin();
    while (front != m_Queue.end() && front->expireAt <= t)
    {
      m_Queue.pop_front();
      front = m_Queue.begin();
    }
  }

  void TextDbgQueue::push(string&& text, const float4 color)
  {
    if (text.size() > MAX_TEXT_LEN)
      text = string(text.c_str(), MAX_TEXT_LEN);

    const uint64_t secToLive = 20;
    m_Queue.push_back();
    m_Queue.back() = QueuedText{
      std::move(text),
      color,
      Time::get_time_since_start() + Time::convert_sec_to_clocks(secToLive)
    };
  }

  auto TextDbgQueue::begin() const -> typename Container::const_iterator
  {
    return m_Queue.begin();
  }

  auto TextDbgQueue::end() const -> typename Container::const_iterator
  {
    return m_Queue.end();
  }

  DECLARE_ECS_COMPONENT(TextDbgQueue, "TextDbgQueue", (ecs::NonTrivialTypeManager<TextDbgQueue, ecs::INITABLE_TYPE>), false, true);
}
