#pragma once

#include <spdlog/spdlog.h>

extern spdlog::logger LOGGER;

void InitLog();

template<class ...Args>
void debug(Args... args)
{
  LOGGER.debug(args...);
}

template<class ...Args>
void log(Args... args)
{
  LOGGER.info(args...);
}

template<class ...Args>
void logwarn(Args... args)
{
  LOGGER.warn(args...);
}

template<class ...Args>
void logerror(Args... args)
{
  LOGGER.error(args...);
}

template<class ...Args>
void logcrit(Args... args)
{
  LOGGER.log(args...);
}
