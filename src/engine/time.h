#pragma once

#include <stdint.h>

namespace Engine::Time
{
  bool InitClock();

  void Tick();

  uint64_t GetTimeSinceStart();

  float GetDt();
}
