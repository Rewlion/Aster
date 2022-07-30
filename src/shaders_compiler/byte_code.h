#pragma once

#include "ast_types.h"

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

  constexpr size_t RESOURCE_MAX_NAME_LEN = 24;
  struct ShOp
  {
    ShOpCode op;
    ShOp() = default;
    ShOp(const ShOpCode op)
      : op(op)
    {
    }
  };

  struct ShBindResource: public ShOp
  {
    ResourceAccessType accessType;
    ResourceType type;
    uint8_t dset, binding;
    char resourceName[RESOURCE_MAX_NAME_LEN];

    ShBindResource() = default;
    ShBindResource(const ResourceAccessType accessType, const ResourceType type, const string& resName,
                   const uint8_t dset, const uint8_t binding)
      : ShOp(ShOpCode::BindResource)
      , accessType(accessType)
      , type(type)
      , dset(dset)
      , binding(binding)
    {
      std::snprintf(resourceName, RESOURCE_MAX_NAME_LEN, "%s", resName.c_str());
    }
  };

  struct ShBeginCbuffer: public ShOp
  {
    char scopeName[RESOURCE_MAX_NAME_LEN];
    ShBeginCbuffer() = default;
    ShBeginCbuffer(const string& scope)
      : ShOp(ShOpCode::BeginCbuffer)
    {
      std::snprintf(scopeName, RESOURCE_MAX_NAME_LEN, "%s", scope.c_str());
    }
  };

  struct ShEndCbuffer: public ShOp
  {
    uint8_t dset, binding;
    ShEndCbuffer() = default;
    ShEndCbuffer(const uint8_t dset, const uint8_t binding)
      : ShOp(ShOpCode::EndCbuffer)
      , dset(dset)
      , binding(binding)
    {
    }
  };

  struct ShBindCbufferVar: public ShOp
  {
    uint16_t offset;
    ResourceAccessType accessType;
    gapi::AttributeType type;
    char resourceName[RESOURCE_MAX_NAME_LEN];

    ShBindCbufferVar() = default;
    ShBindCbufferVar(const uint16_t offset, const ResourceAccessType accessType, const gapi::AttributeType type, const string& resName)
      : ShOp(ShOpCode::BindCbufferVar)
      , accessType(accessType)
      , type(type)
      , offset(offset)
    {
      std::snprintf(resourceName, RESOURCE_MAX_NAME_LEN, "%s", resName.c_str());
    }
  };

  struct ShActivateScope: public ShOp
  {
    char scopeName[RESOURCE_MAX_NAME_LEN];
    ShActivateScope() = default;
    ShActivateScope(const string& name)
      : ShOp(ShOpCode::ActivateScope)
    {
      std::snprintf(scopeName, RESOURCE_MAX_NAME_LEN, "%s", name.c_str());
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
