#include "console.h"
#include "cmd.h"
#include "hash.h"

#include <engine/render/imgui/imgui.h>
#include <engine/utils/string.h>

#include <EASTL/bonus/fixed_ring_buffer.h>
#include <EASTL/vector_map.h>
#include <spdlog/fmt/fmt.h>

#include <ranges>

namespace console
{
  constexpr size_t LOGS_LIMIT = 20;

  class Manager
  {
    public:
      Manager()
        : m_Log(LOGS_LIMIT)
      {
      }

      void init();
      void executeCommand(string_view);

      void log(string&&);
      void logerror(string&&);

      struct Log
      {
        ImVec4 color;
        string text;
      };
      using LogsContainer = eastl::fixed_ring_buffer<Log, LOGS_LIMIT>;
      auto getLogs() const -> const LogsContainer& { return m_Log; }

    private:
      eastl::vector_map<cmd_hash_t, CmdRegistration*> m_Cmds;
      LogsContainer m_Log;
  };
  Manager manager;

  void Manager::init()
  {
    CmdRegistration::enumerate([this](CmdRegistration* cmd){
      const cmd_hash_t hash = cmd_name_hash(cmd->m_Name);
      m_Cmds.insert({hash, cmd});
    });
  }

  void Manager::executeCommand(string_view cmd)
  {
    auto strs = Utils::split_view(cmd, ' ');
    if (strs.size() == 0)
      return;

    const cmd_hash_t cmdHash = cmd_name_hash(strs[0]);
    const auto cmdIt = m_Cmds.find(cmdHash);

    if (cmdIt != m_Cmds.end())
    {
      const auto& cmdReg = *cmdIt->second;
      const size_t argc = strs.size() - 1;
      
      const size_t validArgs = (cmdReg.m_MinArgs <= argc) && (cmdReg.m_MaxArgs >= argc);
      if (validArgs)
      {
        eastl::span<string_view> argv{strs.begin()+1, argc};
        cmdReg.m_Cb(argv);
      }
      else
        this->logerror(fmt::format("invalid argc for command {}: argc={} expected: min={} max={}",
          strs[0], argc, cmdReg.m_MinArgs, cmdReg.m_MaxArgs));
    }
    else
      this->logerror(fmt::format("unknown command `{}`", strs[0]));
  }

  void Manager::log(string&& msg)
  {
    m_Log.push_back();
    m_Log.back() = {.color = ImVec4{1,1,1,1}, .text = std::move(msg)};
  }

  void Manager::logerror(string&& msg)
  {
    m_Log.push_back();
    m_Log.back() = m_Log.back() = {.color = ImVec4{1,0,0,1}, .text = std::move(msg)};
  }

  void init()
  {
    manager.init();
  }

  void exec(string_view msg)
  {
    manager.executeCommand(msg);
  }

  void log(string&& msg)
  {
    manager.log(std::move(msg));
  }
  
  void logerror(string&& msg)
  {
    manager.logerror(std::move(msg));
  }

  void imgui_console_wnd()
  {
    const auto& logs = manager.getLogs();
    const size_t logsCount = logs.size();

    size_t i = 0;
    for (const auto& log: logs)
    {
      ImGui::TextColored(log.color, "%s", log.text.c_str());
      ++i;
    }

    for (; i < LOGS_LIMIT; ++i)
      ImGui::NewLine();
    
    constexpr size_t bufSize = 50;
    char buf[bufSize];
    std::memset(buf, 0, bufSize);
    if (ImGui::InputText("buf", buf, bufSize, ImGuiInputTextFlags_EnterReturnsTrue))
      manager.executeCommand(string_view{buf});
    
    if (ImGui::IsItemHovered() || !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
      ImGui::SetKeyboardFocusHere(-1);
  }
}

IMGUI_REG_WND("console", console::imgui_console_wnd, 0);

void hello_cmd(eastl::span<string_view> argv)
{
  string msg = argv.size() == 2 ?
               fmt::format("{} {}", argv[0], argv[1]) :
               string{argv[0]};

  console::log(std::move(msg));
}

CONSOLE_CMD("echo", 1, 2, hello_cmd);