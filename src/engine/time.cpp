#include "time.h"

#include <engine/assert.h>

#include <windows.h>

namespace Engine::Time
{
  static uint64_t perf_counter_frequency = 0;
  static uint64_t start_hires_clock = 0;
  static uint64_t last_hires_clock = 0;

  static uint64_t hires_dt = 0;
  static uint64_t ms_dt = 0;
  static uint64_t us_dt = 0;

  static uint64_t GetHiresClock()
  {
    LARGE_INTEGER largeInt;
    if (QueryPerformanceCounter(&largeInt))
      return largeInt.QuadPart;
    else
      return 0;
  }

  bool InitClock()
  {
    LARGE_INTEGER largeInt;
    const BOOL queriedFreq = QueryPerformanceFrequency(&largeInt);

    if (!queriedFreq)
      return false;

    perf_counter_frequency = largeInt.QuadPart;
    start_hires_clock = GetHiresClock();
    last_hires_clock = start_hires_clock;

    return true;
  }

  void Tick()
  {
    const uint64_t current_time = GetHiresClock();
    hires_dt = current_time - last_hires_clock;
    last_hires_clock = current_time;

    ms_dt = (hires_dt * 1000) / perf_counter_frequency;
    us_dt = (hires_dt * 1000000) / perf_counter_frequency;
  }

  uint64_t GetTimeSinceStart()
  {
    return last_hires_clock - start_hires_clock;
  }

  float GetDt()
  {
    return static_cast<float>(ms_dt) / 1000.0f;
  }
}