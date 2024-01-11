#include "custom_type_registry.h"

engine_data_type_id_t ed::RegisteredCompileTypeId::m_LastId{0};

namespace ed
{
  auto CustomTypeRegistry::get(string_view type_name) -> const Entry*
  {
    const string_hash nameHash = str_hash(type_name);
    const auto it = m_NameHashToEntryId.find(nameHash);
    if (it != m_NameHashToEntryId.end())
      return &m_Entries[it->second];
    
    return nullptr;
  }

  auto CustomTypeRegistry::get(const engine_data_type_id_t& type_id) -> const Entry*
  {
    const auto it = m_CompileTimeIdToEntryId.find(type_id);
    if (it != m_CompileTimeIdToEntryId.end())
      return &m_Entries[it->second];
    
    return nullptr;
  }
}
