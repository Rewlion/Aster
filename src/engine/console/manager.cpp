#include "manager.h"
#include "cmd.h"

namespace console
{
  void Manager::init()
  {
    CmdRegistration::enumerate([this](CmdRegistration* cmd){
      const cmd_hash_t hash = cmd_name_hash(cmd->m_Name);
      m_Cmds.insert({hash, cmd});
    });
  }
}
