#pragma once

#include <engine/render/debug_text.h>

#include <spdlog/spdlog.h>

namespace Engine
{
  void InitLog();
  spdlog::logger* get_logger();
}


#define logdbg(...) Engine::get_logger()->debug(__VA_ARGS__)
#define loginfo(...) Engine::get_logger()->info(__VA_ARGS__)
#define logwarn(...) Engine::get_logger()->warn(__VA_ARGS__)
#define logerror_no_vis(...) Engine::get_logger()->error(__VA_ARGS__)
#define logerror(...) \
  do {\
    Engine::get_logger()->error(__VA_ARGS__);\
    Engine::dbg::vis_logerr(fmt::format(__VA_ARGS__));\
  } while(false)
