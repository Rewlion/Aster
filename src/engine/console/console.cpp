#include "console.h"
#include "cmd.h"
#include "hash.h"

#include <engine/render/imgui/imgui.h>
#include <engine/utils/string.h>

#include <EASTL/bonus/fixed_ring_buffer.h>
#include <EASTL/vector_map.h>
#include <imgui/imgui_internal.h>
#include <spdlog/fmt/fmt.h>

#include <algorithm>
#include <fstream>
#include <ranges>

namespace console
{
  constexpr size_t LOGS_LIMIT = 20;
  constexpr size_t INPUT_HISTORY_LIMIT = 10;
  constexpr const char* INPUT_HISTORY_FILE = ".console.txt";

  bool enabled = false;
  bool forceWindowFocus = true;

  class InputHistory
  {
    public:
      InputHistory()
        : m_Cursor(0)
        , m_Inputs(INPUT_HISTORY_LIMIT)
      {
        bool cropRequired = false;
        m_Cursor = 0;

        {
          auto savedHistory = std::ifstream(INPUT_HISTORY_FILE);
          if (savedHistory.is_open())
          {
            string cmd;
            size_t n = 0;
            while (std::getline(savedHistory,cmd))
            {
              if (cmd != "")
              {
                ++n;
                m_Inputs.push_back(std::move(cmd));
              }
            }
            resetLook();

            cropRequired = n > INPUT_HISTORY_LIMIT;
          }
        }

        if (cropRequired)
          std::remove(INPUT_HISTORY_FILE);

        m_SaveFile.open(INPUT_HISTORY_FILE, std::ios::app);

        if (cropRequired)
        {
          for (const auto& cmd: m_Inputs)
            saveCmd(cmd);
        }
      }

      void add(string&& cmd)
      {
        m_Inputs.push_back(std::move(cmd));
        saveCmd(cmd);
        resetLook();
      }

      auto lookAt(int offset) -> string_view
      {
        const int len = m_Inputs.size();
        m_Cursor += offset;
        
        if (m_Cursor < 0 || m_Cursor > len)
        {
          m_Cursor = std::clamp(m_Cursor + offset, 0, len);
          return "";
        }

        return m_Inputs[m_Cursor];
      }

    private:
      void saveCmd(string_view cmd)
      {
        if (m_SaveFile.is_open()) [[likely]]
        {
          m_SaveFile << cmd << '\n';
          m_SaveFile.flush();
        }
      }

      void resetLook()
      {
        m_Cursor = m_Inputs.size();
      }

    private:
      int m_Cursor;
      eastl::fixed_ring_buffer<string, INPUT_HISTORY_LIMIT> m_Inputs;
      std::ofstream m_SaveFile;
  };

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

      auto getHistoryInput(bool prev) -> string_view;

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
      InputHistory m_InputHistory;
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

    m_InputHistory.add(string{cmd});
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

  auto Manager::getHistoryInput(bool prev) -> string_view
  {
    return m_InputHistory.lookAt(prev ? -1 : +1);
  }

  void init()
  {
    manager.init();
  }

  void exec(string_view msg)
  {
    manager.executeCommand(msg);
  }

  void clog(string&& msg)
  {
    manager.log(std::move(msg));
  }
  
  void clogerror(string&& msg)
  {
    manager.logerror(std::move(msg));
  }

  void enable(const bool e)
  {
    enabled = e;
    if (e)
      forceWindowFocus = true;
  }

  void imgui_console_wnd()
  {
    if (!enabled)
      return;

    ImGui::Begin("console", nullptr, 0);

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

    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) || ImGui::IsKeyPressed(ImGuiKey_DownArrow))
    {
      bool prev = ImGui::IsKeyPressed(ImGuiKey_UpArrow);
      string_view history = manager.getHistoryInput(prev);
      if (history != "")
      {
        std::strncpy(buf, history.data(), bufSize);

        ImGuiWindow* window = ImGui::GetCurrentWindow();
        const ImGuiID id = window->GetID("buf");

        if (ImGuiInputTextState* state = ImGui::GetInputTextState(id))
          state->ReloadUserBufAndMoveToEnd();
      }
    }

    if (ImGui::InputText("buf", buf, bufSize, ImGuiInputTextFlags_EnterReturnsTrue))
    {
      manager.executeCommand(string_view{buf});
      forceWindowFocus = true;
    }
    
    if (forceWindowFocus && (ImGui::IsItemHovered() || !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
    {
      ImGui::SetKeyboardFocusHere(-1);
      forceWindowFocus = false;
    }

    ImGui::End();
  }
}

IMGUI_REG_WND(console::imgui_console_wnd);

void hello_cmd(eastl::span<string_view> argv)
{
  string msg = argv.size() == 2 ?
               fmt::format("{} {}", argv[0], argv[1]) :
               string{argv[0]};

  console::clog(std::move(msg));
}

CONSOLE_CMD("echo", 1, 2, hello_cmd);