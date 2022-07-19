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

  struct ShadersResourcesReserveExp: public ScopeExp
  {
    const ShaderExp* shaders;
    const ResourceReserveExp* exps;
    ShadersResourcesReserveExp(const ShaderExp* shaders, ResourceReserveExp* exps)
      : ScopeExp(ScopeExp::Type::ShadersResourcesReserve)
      , shaders(shaders)
      , exps(exps)
    {
    }
    virtual ~ShadersResourcesReserveExp()
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
    AttributeType varType;
    const char* name;
    const ResourceAssignExp* assignExps;
    CbufferVarDeclarationExp(const AttributeType varType, const char* name,
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
