#include "log.h"

#include <engine/datablock/datablock.h>
#include <engine/settings.h>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <ctime>
#include <iomanip>
#include <memory>
#include <sstream>

spdlog::logger LOGGER = spdlog::logger("multi_sink", {});

static std::string get_time_string()
{
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");

  return oss.str();
}

void InitLog()
{
  const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  console_sink->set_level(spdlog::level::trace);
  console_sink->set_pattern("[%^%l%$] %v");

  DataBlock* settingsBlk = GetAppSettings();
  const String appName = settingsBlk->GetText("app_name");
  const String timeStr = get_time_string();
  const String logFile = fmt::format("logs_{}/{}.log.txt", appName,timeStr);

  const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile.c_str(), true);
  file_sink->set_level(spdlog::level::trace);

  LOGGER = spdlog::logger("", {console_sink, file_sink});
  LOGGER.set_pattern("%H:%M:%S [%^%L%$] %v");
}