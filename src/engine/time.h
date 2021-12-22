#pragma once

#include <stdint.h>

bool init_engine_clock();

void tick_time();

uint64_t get_time_since_start();

float get_dt();
