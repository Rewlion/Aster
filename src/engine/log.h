#pragma once

#include <spdlog/spdlog.h>

namespace Engine
{
  void InitLog();
  spdlog::logger* get_logger();
}


#define logdbg(...) Engine::get_logger()->debug(__VA_ARGS__)
#define loginfo(...) Engine::get_logger()->info(__VA_ARGS__)
#define logwarn(...) Engine::get_logger()->warn(__VA_ARGS__)
#define logerror(...) Engine::get_logger()->error(__VA_ARGS__)
