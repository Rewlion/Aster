#pragma once

#include "ast.h"
#include "spirv.h"
#include "resources.h"

#include <engine/algorithm/hash.h>
#include <engine/types.h>
#include <engine/utils/fixed_stack.hpp>

#include <stdint.h>
#include <optional>
#include <variant>

#include <boost/serialization/binary_object.hpp>
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>
#include <EASTL/vector_set.h>

namespace ShadersSystem
{
  #define CASE_CBUFFER_VARS \
    case ResourceType::Float:\
    case ResourceType::Float2:\
    case ResourceType::Float3:\
    case ResourceType::Float4:\
    case ResourceType::Int:\
    case ResourceType::Int2:\
    case ResourceType::Int3:\
    case ResourceType::Int4

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
    ShOp(const ShOpCode op)
      : op(op)
    {
    }
  };

  struct ShBindResource: public ShOp
  {
    ResourceAccessType accessType;
    uint8_t dset, binding;
    char resourceName[RESOURCE_MAX_NAME_LEN];

    ShBindResource(const ResourceAccessType accessType, const string& resName,
                   const uint8_t dset, const uint8_t binding)
      : ShOp(ShOpCode::BindResource)
      , accessType(accessType)
      , dset(dset)
      , binding(binding)
    {
      std::snprintf(resourceName, RESOURCE_MAX_NAME_LEN, "%s", resName.c_str());
    }
  };

  struct ShBeginCbuffer: public ShOp
  {
    char scopeName[RESOURCE_MAX_NAME_LEN];
    ShBeginCbuffer(const string& scope)
      : ShOp(ShOpCode::BeginCbuffer)
    {
      std::snprintf(scopeName, RESOURCE_MAX_NAME_LEN, "%s", scope.c_str());
    }
  };

  struct ShEndCbuffer: public ShOp
  {
    ShEndCbuffer()
      : ShOp(ShOpCode::EndCbuffer)
    {
    }
  };

  struct ShBindCbufferVar: public ShOp
  {
    uint16_t offset;
    ResourceAccessType accessType;
    char resourceName[RESOURCE_MAX_NAME_LEN];

    ShBindCbufferVar(const uint16_t offset, const ResourceAccessType accessType, const string& resName)
      : ShOp(ShOpCode::BindCbufferVar)
      , offset(offset)
      , accessType(accessType)
    {
      std::snprintf(resourceName, RESOURCE_MAX_NAME_LEN, "%s", resName.c_str());
    }
  };

  struct ShActivateScope: public ShOp
  {
    char scopeName[RESOURCE_MAX_NAME_LEN];
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

  constexpr uint8_t NO_CBUFFER = ~(0);

  struct RegistersReserve
  {
    bool operator==(const RegistersReserve& rvl) const
    {
      return begin == rvl.begin && end == rvl.end;
    }

    uint8_t begin = 0, end = 0;
  };

  struct ResourceDeclaration
  {
    ResourceType type;
    string name;
    uint8_t dset = 0;
    uint8_t binding = 0;
    const ResourceAssignExpNode* assignExp = nullptr;

    bool operator==(const ResourceDeclaration& rvl) const
    {
      return std::tie(type, name, dset, binding, assignExp) ==
             std::tie(rvl.type, rvl.name, rvl.dset, rvl.binding, rvl.assignExp);
    }
  };

  struct CBufferVar
  {
    ResourceType type;
    string name;
    size_t offset = 0;
    const ResourceAssignExpNode* assignExp = nullptr;

    bool operator==(const CBufferVar& rvl) const
    {
      return type == rvl.type &&
        name == rvl.name &&
        offset == rvl.offset &&
        assignExp == rvl.assignExp;
    }
  };
  typedef eastl::vector<CBufferVar> CbufferVars;

  struct Scope
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

    bool operator==(const Scope& rvl) const
    {
      return fileDeclaration == rvl.fileDeclaration &&
        name == rvl.name &&
        shaders == rvl.shaders &&
        cbuffer == rvl.cbuffer &&
        cbufferSize == rvl.cbufferSize &&
        descriptorSet == rvl.descriptorSet &&
        textureRegisters.has_value() && rvl.textureRegisters.has_value() &&
        textureRegisters.value() == rvl.textureRegisters.value() &&
        cbufferVariables == rvl.cbufferVariables &&
        declaredResources == rvl.declaredResources &&
        hlslResourceDeclaration == rvl.hlslResourceDeclaration &&
        is_same_byte_code(byteCode, rvl.byteCode);
    }
  };

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

  struct Technique
  {
    string name;
    ByteCodes byteCode;
    spirv::v2::InputAssembly ia;
    eastl::vector<ShaderBlob> blobs;
    eastl::vector<spirv::v2::Reflection> reflections;

    bool operator==(const Technique& rvl) const
    {
      return name == rvl.name &&
        is_same_byte_code(byteCode, rvl.byteCode) &&
        ia == rvl.ia;
        blobs == rvl.blobs &&
        reflections == rvl.reflections;
    }
  };

  struct MaterialsBin
  {
    struct Scope
    {
      Scope(const ShadersSystem::Scope& sc)
        : name(sc.name)
        , cbufferReg(sc.cbuffer)
        , cbufferSize(sc.cbufferSize)
        , byteCode(sc.byteCode)
      {
      }

      string name;
      uint8_t cbufferReg;
      uint16_t cbufferSize;
      ByteCodes byteCode;
    };

    eastl::vector<Scope> scopes;
    eastl::vector<Technique> techniques;
  };

  struct ModuleResources
  {
    eastl::vector_set<string_hash> declaredScopes;
    eastl::vector_set<string_hash> declaredTechniques;
  };

  class Compiler
  {
    public:
      bool compileModuleFromFile(const string& file);

      inline void markCompilationFailed() { m_IsCompilationOk = false; }
      void onScopeDeclaration(const ScopeDeclaration&);
      void onTechniqueDeclaration(const TechniqueDeclaration&);

      bool hasScope(const string& scope) const;
      string getScopeHlsl(const string& scope) const;

      inline const MaterialsBin& getMaterialsBins() const
      {
        return m_Bin;
      }

    private:
      Scope constructScope(const ScopeDeclaration&);
      void addShadersResourcesReserve(Scope&, const ShadersResourcesReserve&);
      void addResourceDeclaration(Scope&, const ResourceDeclarationExp&);
      void placeScopeResources(Scope&);
      void placeScopeCbufferVars(Scope&);
      void generateHlslCodeForScope(Scope&);
      void generateByteCodeForScope(Scope&);
      ByteCodes generateByteCodeForResource(const ResourceAssignExpNode& exps);
      ByteCodes generateByteCodeForCbufferVar(const ResourceDeclaration& res);
      ByteCodes generateByteCodeForTexture(const ResourceDeclaration& res);

    private:
      string m_CurrentCompilationFile;
      bool m_IsCompilationOk = true;

      MaterialsBin m_Bin;
      ModuleResources m_Module;

      eastl::vector_map<string_hash, Scope> m_DeclaredScopes;
      eastl::vector_map<string_hash, Technique> m_DeclaredTechniques;
  };
}

namespace boost::serialization
{
  template <class Archive>
  void save(Archive& a, const ShadersSystem::ByteCodes& bc, const unsigned int version)
  {
    const size_t count = bc.size();
  	a & count
      & make_binary_object(bc.data(), bc.size() * sizeof(bc[0]));
  }

  template <class Archive>
  void load(Archive& a, ShadersSystem::ByteCodes& bc, const unsigned int version)
  {
  	size_t count = 0;
    a & count;
    bc.reserve(count);
    a & make_binary_object(bc.data(), count * sizeof(bc[0]));
  }

  template <class Archive>
  void serialize(Archive& a, ShadersSystem::ByteCodes& bc, const unsigned int version)
  {
  	split_free(a, bc, version);
  }

  template<class Archive>
  void serialize(Archive& a, vk::VertexInputBindingDescription& vib, const unsigned version)
  {
    a & make_binary_object(&vib, sizeof(vib));
  }

  template<class Archive>
  void serialize(Archive& a, vk::VertexInputAttributeDescription& via, const unsigned version)
  {
    a & make_binary_object(&via, sizeof(via));
  }

  template<class Archive>
  void serialize(Archive& a, spirv::v2::InputAssembly& ia, const unsigned version)
  {
    a & ia.buffers
      & ia.attributes;
  }

  template<class Archive>
  void serialize(Archive& a, ShadersSystem::ShaderBlob& b, const unsigned version)
  {
    a & b.name
      & b.entry
      & b.stage
      & b.data;
  }

  template<class Archive>
  void serialize(Archive& a, spirv::v2::Binding& b, const unsigned version)
  {
    a & b.type
      & b.stages
      & b.name;
  }

  template<class Archive>
  void serialize(Archive& a, spirv::v2::DescriptorSet& dset, const unsigned version)
  {
    a & dset.bindings;
  }

  template<class Archive>
  void serialize(Archive& a, spirv::v2::Reflection& r, const unsigned version)
  {
    a & r.entry
      & r.stage;
      & r.dsets;
  }

  template<class Archive>
  void serialize(Archive& a, ShadersSystem::Technique& t, const unsigned version)
  {
    a & t.name
      & t.byteCode
      & t.ia
      & t.blobs
      & t.reflections;
  }

  template<class Archive>
  void serialize(Archive& a, ShadersSystem::MaterialsBin::Scope& s, const unsigned version)
  {
    a & s.name
      & s.cbufferReg
      & s.cbufferSize
      & s.byteCode;
  }

  template<class Archive>
  void serialize(Archive &a, ShadersSystem::MaterialsBin& bin, const unsigned version)
  {
    a & bin.techniques
      & bin.scopes;
  }

  template <class Archive, class T>
  void save(Archive& a, const eastl::vector<T>& vec, const unsigned int version)
  {
    const size_t count = vec.size();
    a & count
      & make_binary_object(vec.data(), count * sizeof(vec[0]));
  }

  template <class Archive, class T>
  void load(Archive& a, eastl::vector<T>& vec, const unsigned int version)
  {
    size_t count = 0;
    a & count;
    vec.reserve(count);
    a & make_binary_object(vec.data(), count * sizeof(vec[0]));
  }

  template<class Archive, class T>
  void serialize(Archive& a, eastl::vector<T>& vec, const unsigned version)
  {
    split_free(a, vec, version);
  }
}
