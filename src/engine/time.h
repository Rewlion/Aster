#pragma once

#include <stdint.h>

bool InitEngineClock();

void TickTime();

uint64_t GetTimeSinceStart();

float GetDt();
