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
    string name;
    uint8_t dset = 0;
    uint8_t binding = 0;
    const ResourceAssignExp* assignExp = nullptr;

    bool operator==(const ResourceDeclaration& rvl) const;
  };

  struct CBufferVar
  {
    AttributeType type;
    string name;
    size_t offset = 0;
    const ResourceAssignExp* assignExp = nullptr;

    bool operator==(const CBufferVar& rvl) const;
  };
  typedef eastl::vector_map<string_hash, CBufferVar> CbufferVars;

  struct InputDescription
  {
    struct Attribute
    {
      uint32_t offset = 0;
      uint8_t location = 0;
      uint8_t binding = 0;
      AttributeType type;

      bool operator==(const Attribute& rvl) const;
    };

    struct Buffer
    {
      uint32_t stride = 0;
      uint8_t binding = 0;

      bool operator==(const Buffer& rvl) const;
    };

    bool operator==(const InputDescription& ia) const;

    eastl::vector<Attribute> attributes;
    eastl::vector<Buffer> buffers;
  };

  constexpr uint8_t NO_CBUFFER = ~(0);

  struct ScopeDeclaration
  {
    string fileDeclaration;
    string name;
    gapi::ShaderStage shaders;

    uint8_t cbuffer = NO_CBUFFER;
    uint16_t cbufferSize = 0;
    uint8_t descriptorSet;
    std::optional<RegistersReserve> textureRegisters;

    CbufferVars cbufferVariables;
    eastl::vector_map<string_hash, ResourceDeclaration> declaredResources;

    string hlslResourceDeclaration;
    ByteCodes byteCode;

    bool operator==(const ScopeDeclaration& rvl) const
    {
      return
        name == rvl.name &&
        shaders == rvl.shaders &&
        cbuffer == rvl.cbuffer &&
        cbufferSize == rvl.cbufferSize &&
        descriptorSet == rvl.descriptorSet &&
        textureRegisters == rvl.textureRegisters &&
        cbufferVariables == rvl.cbufferVariables &&
        declaredResources == rvl.declaredResources &&
        hlslResourceDeclaration == rvl.hlslResourceDeclaration;
        is_same_byte_code(byteCode, rvl.byteCode);
    }
  };

  struct TechniqueDeclaration
  {
    string name;
    ByteCodes byteCode;
    tfx::RenderState renderState;
    spirv::v2::InputAssembly ia;
    eastl::vector<ShaderBlob> blobs;
    eastl::vector<spirv::v2::Reflection> reflections;

    bool operator==(const TechniqueDeclaration& rvl) const
    {
      return name == rvl.name &&
        is_same_byte_code(byteCode, rvl.byteCode) &&
        renderState == rvl.renderState &&
        ia == rvl.ia;
        blobs == rvl.blobs &&
        reflections == rvl.reflections;
    }
  };
}
