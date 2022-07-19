#pragma once

#include <stdint.h>

#include <engine/assert.h>
#include <engine/gapi/resources.h>

namespace ShadersSystem
{
  enum class ResourceType: uint8_t
  {
    None = 0,
    Cbuffer = 1,
    Texture2D = 2,
    Float = 3,
    Float2 = 4,
    Float3 = 5,
    Float4 = 6,
    Int = 7,
    Int2 = 8,
    Int3 = 9,
    Int4 = 10,
  };

  enum class AttributeType: uint8_t
  {
    None = 0,
    Float = 3,
    Float2 = 4,
    Float3 = 5,
    Float4 = 6,
    Int = 7,
    Int2 = 8,
    Int3 = 9,
    Int4 = 10,
  };

  inline size_t attributeType_to_size(const AttributeType type)
  {
    switch (type)
    {
      case AttributeType::Float:
      case AttributeType::Int:
        return 4;
      case AttributeType::Float2:
      case AttributeType::Int2:
        return 8;
      case AttributeType::Float3:
      case AttributeType::Int3:
        return 12;
      case AttributeType::Float4:
      case AttributeType::Int4:
        return 16;

      default:
      {
        ASSERT(!"unsupported type");
        return 0;
      }
    }
  }

  inline string attributeType_to_string(const AttributeType type)
  {
    switch (type)
    {
      case AttributeType::Float:  return "float";
      case AttributeType::Int:    return "int";
      case AttributeType::Float2: return "float2";
      case AttributeType::Int2:   return "int2";
      case AttributeType::Float3: return "float3";
      case AttributeType::Int3:   return "int3";
      case AttributeType::Float4: return "float4";
      case AttributeType::Int4:   return "int4";

      default:
      {
        ASSERT(!"unsupported type");
        return 0;
      }
    }
  }

  enum class ResourceAccessType: uint8_t
  {
    None = 0,
    Extern = 1,
    Channel = 2
  };

  struct TexturesRangeReserve
  {
    uint8_t begin = 0, end = 0;
  };

  struct Node
  {
    enum Type
    {
      None = 0,
      ResourcesReservation = 1,
      ResourceAcquire = 2
    };
    Type type = None;
  };

  struct TechniqueExp: public Node
  {
    enum class Type
    {
      None = 0,
      ScopeSupport = 1,
      ScopeActivate = 2,
      Hlsl = 3,
      Compile = 4,
      Input = 5,
    };

    TechniqueExp* next;
    Type type;

    TechniqueExp(Type type)
      : type(type)
      , next(nullptr)
    {
    }

    virtual ~TechniqueExp()
    {
      if (next)
      {
        delete next;
        next = nullptr;
      }
    }
  };

  struct ScopeSupportExp: public TechniqueExp
  {
    const char* name;

    ScopeSupportExp(const char* name)
      : TechniqueExp(TechniqueExp::Type::ScopeSupport)
      , name(name)
    {
    }

    virtual ~ScopeSupportExp()
    {
      if (name)
      {
        delete name;
        name = nullptr;
      }
    }
  };

  struct ScopeActivateExp: public TechniqueExp
  {
    const char* name;

    ScopeActivateExp(const char* name)
      : TechniqueExp(TechniqueExp::Type::ScopeActivate)
      , name(name)
    {
    }

    virtual ~ScopeActivateExp()
    {
      if (name)
      {
        delete name;
        name = nullptr;
      }
    }
  };

  struct InputAttributeExp: public Node
  {
    AttributeType attributeType;
    const char* name;
    const InputAttributeExp* next;

    InputAttributeExp(const AttributeType type, const char* name)
      : attributeType(type)
      , name(name)
      , next(nullptr)
    {
    }

    ~InputAttributeExp()
    {
      if (name)
      {
        delete name;
        name = nullptr;
      }
      if (next)
      {
        delete next;
        next = nullptr;
      }
    }
  };

  struct InputBufferExp: public Node
  {
    size_t reg;
    const InputAttributeExp* attributes;
    const InputBufferExp* next;

    InputBufferExp(const size_t reg, const InputAttributeExp* attributes)
      : reg(reg)
      , attributes(attributes)
      , next(nullptr)
    {
    }

    ~InputBufferExp()
    {
      if (attributes)
      {
        delete attributes;
        attributes = nullptr;
      }
      if (next)
      {
        delete next;
        next = nullptr;
      }
    }
  };

  struct InputExp: public TechniqueExp
  {
    InputBufferExp* buffers;
    InputExp(InputBufferExp* buffers)
      : TechniqueExp(TechniqueExp::Type::Input)
      , buffers(buffers)
    {
    }

    virtual ~InputExp()
    {
      if (buffers)
      {
        delete buffers;
        buffers = nullptr;
      }
    }
  };

  enum class TargetProfile
  {
    None,
    VS_6_0,
    VS_6_1,
    VS_6_2,
    VS_6_3,
    VS_6_4,
    VS_6_5,
    VS_6_6,
    VS_6_7,
    PS_6_0,
    PS_6_1,
    PS_6_2,
    PS_6_3,
    PS_6_4,
    PS_6_5,
    PS_6_6,
    PS_6_7,
  };

  struct CompileExp: public TechniqueExp
  {
    TargetProfile targetProfile;
    const char* entry;

    CompileExp(const TargetProfile targetProfile, const char* entry)
      : TechniqueExp(TechniqueExp::Type::Compile)
      , targetProfile(targetProfile)
      , entry(entry)
    {
    }

    virtual ~CompileExp()
    {
      if (entry)
      {
        delete entry;
        entry = nullptr;
      }
    }
  };

  struct HlslExp: public TechniqueExp
  {
    const char* code;

    HlslExp(const char* code)
      : TechniqueExp(TechniqueExp::Type::Hlsl)
      , code(code)
    {
    }

    virtual ~HlslExp()
    {
      if (code)
      {
        delete code;
        code = nullptr;
      }
    }
  };

  struct TechniqueDeclaration: public Node
  {
    const char* name;
    const TechniqueExp* exps;

    TechniqueDeclaration(const char* name, const TechniqueExp* exps)
      : name(name)
      , exps(exps)
    {
    }

    virtual ~TechniqueDeclaration()
    {
      if (name)
      {
        delete name;
        name = nullptr;
      }

      if (exps)
      {
        delete exps;
        exps = nullptr;
      }
    }
  };

  struct ScopeNode: public Node
  {
  };

  struct ScopeExp: public ScopeNode
  {
    enum class Type
    {
      ShadersResourcesReserve = 0,
      ResourceDeclaration = 1
    };
    Type type;
    ScopeExp* next;

    ScopeExp(Type type)
      : type(type)
    {
    }
    virtual ~ScopeExp()
    {
      if (next)
      {
        delete next;
        next = nullptr;
      }
    }
  };

  struct ResourceReserveExp : public Node
  {
    enum class Type
    {
      Shader = 0,
      DescriptorSet = 1,
      Register = 2,
      CBuffer = 3
    };
    Type reserveExpType;
    ResourceReserveExp* next = nullptr;

    ResourceReserveExp(Type reserveExpType)
      : reserveExpType(reserveExpType)
      , next(nullptr)
    {
    }
    virtual ~ResourceReserveExp()
    {
      if (next)
      {
        delete next;
        next = nullptr;
      }
    }
  };

  struct ShaderExp: public ResourceReserveExp
  {
    gapi::ShaderStage shaders;
    ShaderExp(const gapi::ShaderStage shader)
      : ResourceReserveExp(ResourceReserveExp::Type::Shader)
      , shaders(shader)
    {
    }
  };

  struct ShadersResourcesReserve: public ScopeExp
  {
    const ShaderExp* shaders;
    const ResourceReserveExp* exps;
    ShadersResourcesReserve(const ShaderExp* shaders, ResourceReserveExp* exps)
      : ScopeExp(ScopeExp::Type::ShadersResourcesReserve)
      , shaders(shaders)
      , exps(exps)
    {
    }
    virtual ~ShadersResourcesReserve()
    {
      if (shaders)
      {
        delete shaders;
        shaders = nullptr;
      }
      if (exps)
      {
        delete exps;
        exps = nullptr;
      }
    }
  };

  struct DescriptorSetReserveExp: public ResourceReserveExp
  {
    DescriptorSetReserveExp(int dset)
      : ResourceReserveExp(ResourceReserveExp::Type::DescriptorSet)
      , dset(dset)
    {
    }
    int dset = 0;
  };

  struct RegistersReserveExp: public ResourceReserveExp
  {
    enum class Register
    {
      None = 0,
      Texture = 1
    };
    Register registerType;
    int beginRange;
    int endRange;
    RegistersReserveExp(const Register regType, const int beginRange, const int endRange)
      : ResourceReserveExp(ResourceReserveExp::Type::Register)
      , registerType(regType)
      , beginRange(beginRange)
      , endRange(endRange)
    {
    }
  };

  struct CBufferReserveExp: public ResourceReserveExp
  {
    int cbuffer;
    CBufferReserveExp(int cbuffer)
      : ResourceReserveExp(ResourceReserveExp::Type::CBuffer)
      , cbuffer(cbuffer)
    {
    }
  };

  struct ResourceAssignExpNode: public Node
  {
    enum class Type
    {
      None = 0,
      Access = 1
    };
    ResourceAssignExpNode(Type type)
      : type(type)
      , next(nullptr)
      {
      }
    virtual ~ResourceAssignExpNode()
    {
      delete next;
    }
    Type type = Type::None;
    const ResourceAssignExpNode* next = nullptr;
  };

  struct AccessResource: public ResourceAssignExpNode
  {
    ResourceAccessType accessType;
    const char* resourceName;
    AccessResource(const ResourceAccessType accessType, const char* resourceName)
      : ResourceAssignExpNode(ResourceAssignExpNode::Type::Access)
      , accessType(accessType)
      , resourceName(resourceName)
    {
    }
    virtual ~AccessResource()
    {
      if (resourceName)
      {
        delete resourceName;
        resourceName = nullptr;
      }
    }
  };

  struct ResourceDeclarationExp: public ScopeExp
  {
    ResourceType resourceType;
    const char* name;
    const ResourceAssignExpNode* assignExps;
    ResourceDeclarationExp(const ResourceType resType, const char* name,
                        const ResourceAssignExpNode* assignExps)
      : ScopeExp(ScopeExp::Type::ResourceDeclaration)
      , resourceType(resType)
      , name(name)
      , assignExps(assignExps)
    {
    }
    virtual ~ResourceDeclarationExp()
    {
      if (name)
      {
        delete name;
        name = nullptr;
      }
      if (assignExps)
      {
        delete assignExps;
        assignExps = nullptr;
      }
    }
  };

  struct ScopeDeclaration: public Node
  {
    const char* name;
    const ScopeExp* exps;
    ScopeDeclaration(const char* name, const ScopeExp* exps)
      : name(name)
      , exps(exps)
    {
    }
    virtual ~ScopeDeclaration()
    {
      if (name)
      {
        delete name;
        name = nullptr;
      }
      if (exps)
      {
        delete exps;
        exps = nullptr;
      }
    }
  };
}
