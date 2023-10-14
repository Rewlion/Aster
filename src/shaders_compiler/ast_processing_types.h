#pragma once

#include "ast_types.h"
#include "byte_code.h"
#include "spirv.h"

#include <engine/tfx/tfx.h>

namespace ShadersSystem
{
  struct ShaderBlob
  {
    string name;
    string entry;
    gapi::ShaderStage stage;
    eastl::vector<char> data;

    bool operator==(const ShaderBlob& rvl) const
    {
      return name == rvl.name &&
        entry == rvl.entry &&
        stage == rvl.stage &&
        data == rvl.data;
    }
  };

  struct RegistersReserve
  {
    bool operator==(const RegistersReserve& rvl) const;

    uint8_t begin = 0, end = 0;
  };

  struct ResourceAssignExp;

  struct ResourceDeclaration
  {
    ResourceType type;
    ResourceElemStorageType elemStorageType;
    string name;
    uint8_t dset = 0;
    uint8_t binding = 0;
    const ResourceAssignExp* assignExp = nullptr;

    bool operator==(const ResourceDeclaration& rvl) const;
  };

  struct CBufferVar
  {
    gapi::AttributeType type;
    string name;
    size_t offset = 0;
    const ResourceAssignExp* assignExp = nullptr;

    bool operator==(const CBufferVar& rvl) const;
  };
  typedef eastl::vector_map<string_hash, CBufferVar> CbufferVars;

  constexpr uint8_t NO_CBUFFER = ~(0);
  constexpr uint8_t CBUFFER_NOT_ASSIGNED = ~(0)-1;

  struct ScopeDeclaration
  {
    string fileDeclaration;
    string name;

    uint8_t cbuffer = NO_CBUFFER;
    uint16_t cbufferSize = 0;
    uint8_t descriptorSet;
    std::optional<RegistersReserve> textureRegisters;
    std::optional<RegistersReserve> samplerRegisters;
    std::optional<RegistersReserve> bufferRegisters;

    CbufferVars cbufferVariables;
    eastl::vector<const char*> declaredHlslResources;
    eastl::vector_map<string_hash, ResourceDeclaration> declaredResources;

    string hlslResourceDeclaration;
    ByteCodes byteCode;

    bool operator==(const ScopeDeclaration& rvl) const
    {
      return
        name == rvl.name &&
        cbuffer == rvl.cbuffer &&
        cbufferSize == rvl.cbufferSize &&
        descriptorSet == rvl.descriptorSet &&
        textureRegisters == rvl.textureRegisters &&
        samplerRegisters == rvl.samplerRegisters &&
        bufferRegisters == rvl.bufferRegisters &&
        cbufferVariables == rvl.cbufferVariables &&
        declaredHlslResources == rvl.declaredHlslResources &&
        declaredResources == rvl.declaredResources &&
        hlslResourceDeclaration == rvl.hlslResourceDeclaration;
        is_same_byte_code(byteCode, rvl.byteCode);
    }
  };

  struct TechniqueDeclaration
  {
    gapi::PipelineType type;
    string name;
    ByteCodes byteCode;
    tfx::RenderState renderState;
    eastl::vector<ShaderBlob> blobs;
    eastl::vector<spirv::v2::DescriptorSet> dsets;

    bool operator==(const TechniqueDeclaration& rvl) const
    {
      return type == rvl.type &&
        name == rvl.name &&
        is_same_byte_code(byteCode, rvl.byteCode) &&
        renderState == rvl.renderState &&
        blobs == rvl.blobs &&
        dsets == rvl.dsets;
    }
  };
}
