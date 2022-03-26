#pragma once

#include <stdint.h>

namespace Engine::Time
{
  bool init_clock();

  void tick();

  uint64_t get_time_since_start();

  float get_dt();
}
