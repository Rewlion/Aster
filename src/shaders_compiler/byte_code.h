#pragma once

#include "ast_types.h"
#include "limits.h"

#include <engine/gapi/resources.h>

#include <variant>
#include <stdint.h>

namespace ShadersSystem
{
  enum class ShOpCode: uint8_t
  {
    BindResource,
    BeginCbuffer,
    BindCbufferVar,
    EndCbuffer,
    ActivateScope,
  };

  #define SH_OP_BASE_BODY ShOpCode op
  #define SH_OP_BASE(code) r.op = code

  struct ShBindResource
  {
    SH_OP_BASE_BODY;
    ResourceAccessType accessType;
    ResourceType type;
    uint8_t dset, binding;
    char resourceName[RESOURCE_MAX_NAME_LEN];

    static
    auto build(const ResourceAccessType access_type,
               const ResourceType type, const string& res_name,
               const uint8_t dset, const uint8_t binding) -> ShBindResource
    {
      ShBindResource r;
      SH_OP_BASE(ShOpCode::BindResource);
      r.accessType = access_type;
      r.type = type;
      r.dset = dset;
      r.binding = binding;
      std::snprintf(r.resourceName, RESOURCE_MAX_NAME_LEN, "%s", res_name.c_str());

      return r;
    }
  };

  struct ShBeginCbuffer
  {
    SH_OP_BASE_BODY;
    char scopeName[RESOURCE_MAX_NAME_LEN];

    static
    auto build(const string& scope) -> ShBeginCbuffer
    {
      ShBeginCbuffer r;
      SH_OP_BASE(ShOpCode::BeginCbuffer);
      std::snprintf(r.scopeName, RESOURCE_MAX_NAME_LEN, "%s", scope.c_str());

      return r;
    }
  };

  struct ShEndCbuffer
  {
    SH_OP_BASE_BODY;
    uint8_t dset, binding;

    static
    auto build(const uint8_t dset, const uint8_t binding) -> ShEndCbuffer
    {
      ShEndCbuffer r;
      SH_OP_BASE(ShOpCode::EndCbuffer);
      r.dset = dset;
      r.binding = binding;

      return r;
    }
  };

  struct ShBindCbufferVar
  {
    SH_OP_BASE_BODY;
    uint16_t offset;
    ResourceAccessType accessType;
    gapi::AttributeType type;
    char resourceName[RESOURCE_MAX_NAME_LEN];

    static
    auto build(const uint16_t offset, const ResourceAccessType access_type,
               const gapi::AttributeType type, const string& res_name) -> ShBindCbufferVar
    {
      ShBindCbufferVar r;
      SH_OP_BASE(ShOpCode::BindCbufferVar);
      r.offset = offset;
      r.accessType = access_type;
      r.type = type;
      std::snprintf(r.resourceName, RESOURCE_MAX_NAME_LEN, "%s", res_name.c_str());

      return r;
    }
  };

  struct ShActivateScope
  {
    SH_OP_BASE_BODY;
    char scopeName[RESOURCE_MAX_NAME_LEN];

    static
    auto build(const string& scope_name) -> ShActivateScope
    {
      ShActivateScope r;
      SH_OP_BASE(ShOpCode::ActivateScope);
      std::snprintf(r.scopeName, RESOURCE_MAX_NAME_LEN, "%s", scope_name.c_str());

      return r;
    }
  };

  typedef std::variant<ShBindResource, ShBeginCbuffer,
                       ShEndCbuffer, ShBindCbufferVar,
                       ShActivateScope> ShByteCode;
  typedef eastl::vector<ShByteCode> ByteCodes;

  inline bool is_same_byte_code(const ByteCodes& lvl, const ByteCodes& rvl)
  {
    if (lvl.size() != rvl.size())
      return false;

    const size_t len = lvl.size() * sizeof(ShByteCode);
    return std::memcmp(lvl.data(), rvl.data(), len);
  }
}
