#pragma once

namespace ecs
{
  template<BasedFromEcsEvent T>
  void Registry::broadcastEvent(T&& event)
  {
    if (m_EventHandleQueries.find(event.eventNameHash) != m_EventHandleQueries.end())
      m_EventsQueue.pushEvent(eastl::forward<T>(event));
    else
      logerror("can't broadcast event `{}`, it is not registered.", event.eventNameHash);
  }

  template<BasedFromEcsEvent T>
  void Registry::registerEvent()
  {
    loginfo("ecs: registering event `{}`", T::eventName);
    const name_hash_t hash = compile_ecs_name_hash(T::eventName);

    ASSERT_FMT(m_EventHandleQueries.find(hash) == m_EventHandleQueries.end(),
      "ecs: can't register a new event `{}` it's already registered", T::eventName);

    m_EventHandleQueries.insert({
      hash,
      eastl::vector<RegisteredEventQueryInfo>{}
    });
  }
}
