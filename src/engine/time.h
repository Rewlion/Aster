#pragma once

#include <stdint.h>

namespace Engine::Time
{
  bool init_clock();

  void tick();

  uint64_t get_time_since_start();

  float get_sec_since_start();

  float get_dt();

  float convert_to_sec(uint64_t rawTime);

  uint64_t convert_sec_to_clocks(uint64_t sec);
}
