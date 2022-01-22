#pragma once

#include <spdlog/spdlog.h>

namespace Engine
{
  extern spdlog::logger LOGGER;
  void InitLog();
}

template<class ...Args>
void debug(Args... args)
{
  Engine::LOGGER.debug(args...);
}

template<class ...Args>
void log(Args... args)
{
  Engine::LOGGER.info(args...);
}

template<class ...Args>
void logwarn(Args... args)
{
  Engine::LOGGER.warn(args...);
}

template<class ...Args>
void logerror(Args... args)
{
  Engine::LOGGER.error(args...);
}

template<class ...Args>
void logcrit(Args... args)
{
  Engine::LOGGER.log(args...);
}
