#pragma once

#include "ast_types.h"

#include <engine/assert.h>
#include <engine/gapi/resources.h>

#include <stdint.h>
#include <typeinfo>

namespace ShadersSystem
{
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
      None,
      ScopeSupport,
      ScopeActivate,
      Hlsl,
      Compile,
      RenderState,
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

  struct RenderStateExp: public TechniqueExp
  {
    enum class StateType
    {
      PrimitiveTopology,
      DepthTest,
      DepthWrite,
      DepthOp,
      StencilTest,
      StencilFailOp,
      StencilPassOp,
      StencilDepthFailOp,
      StencilCompareOp,
      StencilReferenceValue,
      Blending,
      Input
    };

    RenderStateExp(const StateType stateType)
      : TechniqueExp(TechniqueExp::Type::RenderState)
      , stateType(stateType)
      , next(nullptr)
    {
    }

    virtual ~RenderStateExp()
    {
      if (next)
      {
        delete next;
        next = nullptr;
      }
    }

    StateType stateType;
    RenderStateExp* next;
  };

  struct InputAttributeExp: public Node
  {
    gapi::AttributeType attributeType;
    const char* name;
    const char* semantic;
    const InputAttributeExp* next;

    InputAttributeExp(const gapi::AttributeType type, const char* name, const char* semantic)
      : attributeType(type)
      , name(name)
      , semantic(semantic)
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

  struct InputExp: public RenderStateExp
  {
    InputBufferExp* buffers;
    InputExp(InputBufferExp* buffers)
      : RenderStateExp(RenderStateExp::StateType::Input)
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

  struct PrimitiveTopologyExp: public RenderStateExp
  {
    gapi::PrimitiveTopology tp;

    PrimitiveTopologyExp(const gapi::PrimitiveTopology tp)
      : RenderStateExp(RenderStateExp::StateType::PrimitiveTopology)
      , tp(tp)
    {
    }
  };

  struct DepthTestExp: public RenderStateExp
  {
    DepthTestExp(const bool enabled)
      : RenderStateExp(RenderStateExp::StateType::DepthTest)
      , enabled(enabled)
    {
    }

    bool enabled;
  };

  struct DepthWriteExp: public RenderStateExp
  {
    DepthWriteExp(const bool enabled)
      : RenderStateExp(RenderStateExp::StateType::DepthWrite)
      , enabled(enabled)
    {
    }

    bool enabled;
  };

  struct DepthOpExp: public RenderStateExp
  {
    DepthOpExp(const gapi::CompareOp op)
      : RenderStateExp(RenderStateExp::StateType::DepthOp)
      , op(op)
    {
    }

    gapi::CompareOp op;
  };

  struct StencilTestExp: public RenderStateExp
  {
    StencilTestExp(const bool enabled)
      : RenderStateExp(RenderStateExp::StateType::StencilTest)
      , enabled(enabled)
    {
    }

    bool enabled;
  };

  struct StencilFailOpExp: public RenderStateExp
  {
    StencilFailOpExp(const gapi::StencilOp op)
      : RenderStateExp(RenderStateExp::StateType::StencilFailOp)
      , op(op)
    {
    }

    const gapi::StencilOp op;
  };

  struct StencilPassOpExp: public RenderStateExp
  {
    StencilPassOpExp(const gapi::StencilOp op)
      : RenderStateExp(RenderStateExp::StateType::StencilPassOp)
      , op(op)
    {
    }

    const gapi::StencilOp op;
  };

  struct StencilDepthFailOpExp: public RenderStateExp
  {
    StencilDepthFailOpExp(const gapi::StencilOp op)
      : RenderStateExp(RenderStateExp::StateType::StencilDepthFailOp)
      , op(op)
    {
    }

    const gapi::StencilOp op;
  };

  struct StencilCompareOpExp: public RenderStateExp
  {
    StencilCompareOpExp(const gapi::CompareOp op)
      : RenderStateExp(RenderStateExp::StateType::StencilCompareOp)
      , op(op)
    {
    }

    const gapi::CompareOp op;
  };

  struct StencilReferenceValueExp: public RenderStateExp
  {
    StencilReferenceValueExp(const uint32_t value)
      : RenderStateExp(RenderStateExp::StateType::StencilReferenceValue)
      , value(value)
    {
    }
    uint32_t value;
  };

  struct BlendingExp: public RenderStateExp
  {
    enum class Type
    {
      LogicOpEnabling,
      LogicOp,
      BlendConstants,
      MrtState
    };

    BlendingExp(const Type blendingStateType)
      : RenderStateExp(RenderStateExp::StateType::Blending)
      , blendingStateType(blendingStateType)
      , next(nullptr)
    {
    }

    virtual ~BlendingExp()
    {
      if (next)
      {
        delete next;
        next = nullptr;
      }
    }

    Type blendingStateType;
    BlendingExp* next;
  };

  struct LogicOpEnablingExp: public BlendingExp
  {
    LogicOpEnablingExp(const bool enabled)
      : BlendingExp(BlendingExp::Type::LogicOpEnabling)
      , enabled(enabled)
    {
    }
    bool enabled;
  };

  struct LogicOpExp: public BlendingExp
  {
    LogicOpExp(const gapi::LogicOp op)
      : BlendingExp(BlendingExp::Type::LogicOp)
      , op(op)
    {
    }

    gapi::LogicOp op;
  };

  struct BlendConstants: public BlendingExp
  {
    BlendConstants(const float4 val)
      : BlendingExp(BlendingExp::Type::BlendConstants)
      , val(val)
    {
    }

    float4 val;
  };

  struct MrtBlendingExp: public BlendingExp
  {
    enum class Type
    {
      Enabling,
      SrcColorBlendFactor,
      DstColorBlendFactor,
      ColorBlendOp,
      SrcAlphaBlendFactor,
      DstAlphaBlendFactor,
      AlphaBlendOp
    };

    MrtBlendingExp(const Type type)
      : BlendingExp(BlendingExp::Type::MrtState)
      , mrtBlendingStateType(type)
      , next(nullptr)
      , n(0)
    {
    }

    virtual ~MrtBlendingExp()
    {
      if (next)
      {
        delete next;
        next = nullptr;
      }
    }

    Type mrtBlendingStateType;
    MrtBlendingExp* next;
    int n;
  };

  struct MrtBlendingEnabledExp: public MrtBlendingExp
  {
    MrtBlendingEnabledExp(const bool enabled)
      : MrtBlendingExp(MrtBlendingExp::Type::Enabling)
      , enabled(enabled)
    {
    }

    bool enabled;
  };

  struct MrtSrcColorBlendFactorExp: public MrtBlendingExp
  {
    MrtSrcColorBlendFactorExp(const gapi::BlendFactor factor)
      : MrtBlendingExp(MrtBlendingExp::Type::SrcColorBlendFactor)
      , factor(factor)
    {
    }

    gapi::BlendFactor factor;
  };

  struct MrtDstColorBlendFactorExp: public MrtBlendingExp
  {
    MrtDstColorBlendFactorExp(const gapi::BlendFactor factor)
      : MrtBlendingExp(MrtBlendingExp::Type::DstColorBlendFactor)
      , factor(factor)
    {
    }

    gapi::BlendFactor factor;
  };

  struct MrtColorBlendOpExp: public MrtBlendingExp
  {
    MrtColorBlendOpExp(const gapi::BlendOp op)
      : MrtBlendingExp(MrtBlendingExp::Type::ColorBlendOp)
      , op(op)
    {
    }

    gapi::BlendOp op;
  };

  struct MrtSrcAlphaBlendFactorExp: public MrtBlendingExp
  {
    MrtSrcAlphaBlendFactorExp(const gapi::BlendFactor factor)
      : MrtBlendingExp(MrtBlendingExp::Type::SrcAlphaBlendFactor)
      , factor(factor)
    {
    }

    gapi::BlendFactor factor;
  };

  struct MrtDstAlphaBlendFactorExp: public MrtBlendingExp
  {
    MrtDstAlphaBlendFactorExp(const gapi::BlendFactor factor)
      : MrtBlendingExp(MrtBlendingExp::Type::DstAlphaBlendFactor)
      , factor(factor)
    {
    }

    gapi::BlendFactor factor;
  };

  struct MrtAlphaBlendOpExp: public MrtBlendingExp
  {
    MrtAlphaBlendOpExp(const gapi::BlendOp op)
      : MrtBlendingExp(MrtBlendingExp::Type::ColorBlendOp)
      , op(op)
    {
    }

    gapi::BlendOp op;
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

  struct TechniqueDeclarationExp: public Node
  {
    const char* name;
    const TechniqueExp* exps;

    TechniqueDeclarationExp(const char* name, const TechniqueExp* exps)
      : name(name)
      , exps(exps)
    {
    }

    virtual ~TechniqueDeclarationExp()
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
      ResourceDeclaration = 1,
      CbufferVarDeclaration = 2,
    };
    Type type;
    ScopeExp* next;

    ScopeExp(Type type)
      : type(type)
      , next(nullptr)
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

  struct ShadersResourcesReserveExp: public ScopeExp
  {
    const ResourceReserveExp* exps;
    ShadersResourcesReserveExp(ResourceReserveExp* exps)
      : ScopeExp(ScopeExp::Type::ShadersResourcesReserve)
      , exps(exps)
    {
    }
    virtual ~ShadersResourcesReserveExp()
    {
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
      None,
      Texture,
      Sampler
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

  struct ResourceAssignExp: public Node
  {
    enum class Type
    {
      None = 0,
      Access = 1
    };
    ResourceAssignExp(Type type)
      : type(type)
      , next(nullptr)
      {
      }
    virtual ~ResourceAssignExp()
    {
      delete next;
    }

    bool operator==(const ResourceAssignExp& e) const
    {
      const ResourceAssignExp* a = next;
      const ResourceAssignExp* b = e.next;

      while (a && b)
      {
        if (a->isEqual(b) == false)
          return false;

        a = a->next;
        b = b->next;
      }

      return (a == nullptr) && (b == nullptr);
    }

    Type type = Type::None;
    const ResourceAssignExp* next = nullptr;

  protected:
    virtual bool isEqual(const ResourceAssignExp* e) const
    {
      return type == e->type;
    }
  };

  struct AccessResource: public ResourceAssignExp
  {
    ResourceAccessType accessType;
    const char* resourceName;
    AccessResource(const ResourceAccessType accessType, const char* resourceName)
      : ResourceAssignExp(ResourceAssignExp::Type::Access)
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

  protected:
    virtual bool isEqual(const ResourceAssignExp* e) const
    {
      if (typeid(this) == typeid(e))
      {
        const auto* rvl = reinterpret_cast<const AccessResource*>(e);
        return accessType == rvl->accessType &&
               !std::strcmp(resourceName, rvl->resourceName);
      }

      return false;
    }
  };

  struct ResourceDeclarationExp: public ScopeExp
  {
    ResourceType resourceType;
    const char* name;
    const ResourceAssignExp* assignExps;
    ResourceDeclarationExp(const ResourceType resType, const char* name,
                        const ResourceAssignExp* assignExps)
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

  struct CbufferVarDeclarationExp: public ScopeExp
  {
    gapi::AttributeType varType;
    const char* name;
    const ResourceAssignExp* assignExps;
    CbufferVarDeclarationExp(const gapi::AttributeType varType, const char* name,
                             const ResourceAssignExp* assignExps)
      : ScopeExp(ScopeExp::Type::CbufferVarDeclaration)
      , varType(varType)
      , name(name)
      , assignExps(assignExps)
    {
    }
    virtual ~CbufferVarDeclarationExp()
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

  struct ScopeDeclarationExp: public Node
  {
    const char* name;
    const ScopeExp* exps;
    ScopeDeclarationExp(const char* name, const ScopeExp* exps)
      : name(name)
      , exps(exps)
    {
    }
    virtual ~ScopeDeclarationExp()
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
