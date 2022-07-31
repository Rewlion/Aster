#pragma once

#include <spdlog/spdlog.h>

namespace Engine
{
  void InitLog();
  spdlog::logger* get_logger();
}

template<class ...Args>
void debug(Args... args)
{
  Engine::LOGGER.debug(args...);
}

#define loginfo(...) Engine::get_logger()->info(__VA_ARGS__);

template<class ...Args>
void logwarn(Args... args)
{
  //Engine::LOGGER.warn(args...);
}

template<class ...Args>
void logerror(Args... args)
{
  //Engine::LOGGER.error(args...);
}

template<class ...Args>
void logcrit(Args... args)
{
  //Engine::LOGGER.log(args...);
}
