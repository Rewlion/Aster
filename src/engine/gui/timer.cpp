#include "timer.h"

#include <engine/log.h>
#include <engine/qjs/function.h>
#include <engine/time.h>

namespace Engine::gui
{
  Timer::Timer()
    : m_ClocksBeforeCall(0)
    , m_NextCallTime(0)
  {
  }

  Timer::Timer(Timer&& rvl)
  {
    *this = std::move(rvl);
  }

  Timer::Timer(qjs::Value&& cb, uint64_t secBeforeCall)
    : m_Cb(std::move(cb))
  {
    m_ClocksBeforeCall = Time::convert_sec_to_clocks(secBeforeCall);
    m_NextCallTime = Time::get_time_since_start() + m_ClocksBeforeCall;
  }

  Timer::~Timer()
  {
    m_ClocksBeforeCall = 0;
    m_NextCallTime = 0;
  }

  void Timer::tick()
  {
    const uint64_t t = Time::get_time_since_start();
    if (t >= m_NextCallTime)
    {
      m_Cb.as<qjs::FunctionView>()({}, 0, nullptr);
      m_NextCallTime += m_ClocksBeforeCall;
    }
  }

  Timer& Timer::operator=(Timer&& rvl)
  {
    this->~Timer();
    m_Cb = std::move(rvl.m_Cb);

    m_ClocksBeforeCall = rvl.m_ClocksBeforeCall;
    rvl.m_ClocksBeforeCall = 0;

    m_NextCallTime = rvl.m_NextCallTime;
    rvl.m_NextCallTime = 0;

    return *this;
  }

  void TimersPool::addTimer(const string& name, qjs::Value&& cb, uint64_t clocksBeforeCall)
  {
    const auto it = m_Timers.find(name);
    if (it != m_Timers.end())
    {
      logerror("ui: can't add a new timer {}: it already exist", name);
      return;
    }

    if (!cb.isFunction())
    {
      logerror("ui: can't add a new timer {}: cb is not a function", name);
      return;
    }

    m_Timers.emplace(eastl::make_pair(name, Timer{std::move(cb), clocksBeforeCall} ));
  }

  void TimersPool::removeTimer(const string& name)
  {
    m_TimersToRemove.insert(name);
  }

  void TimersPool::clearTimers()
  {
    for (const auto& name: m_TimersToRemove)
    {
      const auto it = m_Timers.find(name);
      if (it == m_Timers.end())
      {
        logerror("ui: can't remove timer {}: it doesn't exist", name);
        return;
      }
      m_Timers.erase(it);
    }

    m_TimersToRemove.clear();
  }

  void TimersPool::tick()
  {
    clearTimers();

    for (auto& [_, timer] : m_Timers)
      timer.tick();
  }
}
