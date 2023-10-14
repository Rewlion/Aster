#include "compiler.h"
#include "limits.h"

#include <engine/log.h>

namespace ShadersSystem
{
  class ScopeProcessor
  {
    public:
      ScopeProcessor(Compiler& c)
        : m_Compiler(c)
      {
      }

      void process(const ScopeDeclarationExp& s, const string& current_compilation_file)
      {
        m_Scope = ScopeDeclaration{};
        m_Scope.fileDeclaration = current_compilation_file;
        m_Scope.name = string(s.name);

        processExps(s.exps);
        placeScopeResources();
        placeScopeCbufferVars();
        generateHlslCodeForScope();
        generateByteCodeForScope();
      }

      ScopeDeclaration&& takeScope()
      {
        return std::move(m_Scope);
      }

    private:
      void processExps(const ScopeExp* exp)
      {
        while (exp != nullptr)
        {
          switch(exp->type)
          {
            case ScopeExp::Type::ShadersResourcesReserve:
            {
              auto& e = *static_cast<const ShadersResourcesReserveExp*>(exp);
              addShadersResourcesReserve(e);
              break;
            }

            case ScopeExp::Type::HlslResourceDeclaration:
            {
              auto& e = *static_cast<const HlslResourceDeclarationExp*>(exp);
              addHlslResourceDeclaration(e);
              break;
            }

            case ScopeExp::Type::ResourceDeclaration:
            {
              auto& e = *static_cast<const ResourceDeclarationExp*>(exp);
              addResourceDeclaration(e);
              break;
            }

            case ScopeExp::Type::CbufferVarDeclaration:
            {
              m_Scope.cbuffer = CBUFFER_NOT_ASSIGNED;
              auto& e = *static_cast<const CbufferVarDeclarationExp*>(exp);
              addCbufferVarDeclaration(e);
              break;
            }

            default:
            {
              ASSERT(!"unsupported");
              break;
            }
          }

          exp = exp->next;
        }
      }

      void addShadersResourcesReserve(const ShadersResourcesReserveExp& resourcesReserve)
      {
        const ResourceReserveExp* exp = resourcesReserve.exps;
        while (exp != nullptr)
        {
          switch (exp->reserveExpType)
          {
            case ResourceReserveExp::Type::DescriptorSet:
            {
              const DescriptorSetReserveExp* e = reinterpret_cast<const DescriptorSetReserveExp*>(exp);
              m_Scope.descriptorSet = e->dset;
              break;
            }

            case ResourceReserveExp::Type::Register:
            {
              const RegistersReserveExp* e = reinterpret_cast<const RegistersReserveExp*>(exp);
              RegistersReserve r;
              r.begin = e->beginRange;
              r.end = e->endRange;
              switch (e->registerType)
              {
                case RegistersReserveExp::Register::Texture:
                {
                  m_Scope.textureRegisters = r;
                  break;
                }

                case RegistersReserveExp::Register::Sampler:
                {
                  m_Scope.samplerRegisters = r;
                  break;
                }

                case RegistersReserveExp::Register::Buffer:
                {
                  m_Scope.bufferRegisters= r;
                  break;
                }

                default:
                {
                  ASSERT(!"unsupported");
                  break;
                }
              }
              break;
            }

            default:
            {
              ASSERT(!"unsupported");
              break;
            }
          }
          exp = exp->next;
        }
      }

      void addHlslResourceDeclaration(const HlslResourceDeclarationExp& decl)
      {
        m_Scope.declaredHlslResources.push_back(decl.code);
      }

      void addResourceDeclaration(const ResourceDeclarationExp& resourceDeclaration)
      {
        const string_hash resNameHash = str_hash(resourceDeclaration.name);
        if (m_Scope.declaredResources.find(resNameHash) != m_Scope.declaredResources.end())
          throw std::runtime_error(
            fmt::format("error: failed to declare a resource `{}`: it's already declared",
                        resourceDeclaration.name)
          );

        switch (resourceDeclaration.resourceType)
        {
          case ResourceType::Sampler:
          case ResourceType::Buffer:
          case ResourceType::Texture2D:
          case ResourceType::Texture3D:
          case ResourceType::TextureCube:
          case ResourceType::RWTexture2D:
          case ResourceType::RWTexture3D:
          case ResourceType::RWStructuredBuffer:
          case ResourceType::RWBuffer:
          {
            m_Scope.declaredResources.insert({
              resNameHash,
              ResourceDeclaration{
                .type = resourceDeclaration.resourceType,
                .elemStorageType = resourceDeclaration.elemStorageType,
                .name = resourceDeclaration.name,
                .dset = 0,
                .binding = 0,
                .assignExp = resourceDeclaration.assignExps
              }
            });
            break;
          }

          default:
          {
            ASSERT(!"unsupported");
            break;
          }
        }
      }

      void addCbufferVarDeclaration(const CbufferVarDeclarationExp& decl)
      {
        const string_hash resNameHash = str_hash(decl.name);
        if (m_Scope.cbufferVariables.find(resNameHash) != m_Scope.cbufferVariables.end())
          throw std::runtime_error(
            fmt::format("error: failed to declare a cbuffer variable `{}`: it's already declared",
                        decl.name)
          );

        m_Scope.cbufferVariables.insert({
          resNameHash,
          CBufferVar{
            .type = decl.varType,
            .name = decl.name,
            .offset = 0,
            .assignExp = decl.assignExps
          }
        });
      }

      void placeScopeResources()
      {
        struct RegistersPlacing
        {
          bool hasRegisters;
          size_t begin, end;
        };

        const auto getRegistersInfo = [](const auto& registers, const char* registers_name, const int min_reg)
        {
          RegistersPlacing res;
          if (registers.has_value())
          {
            res.hasRegisters = true;
            if (registers->begin > registers->end)
              throw std::runtime_error(fmt::format("invalid interval for registers `{}`", registers_name));
            if ( (registers->begin > MAX_REGISTER) || (registers->end > MAX_REGISTER))
              throw std::runtime_error(
                fmt::format("invalid interval values for registers '{}'. valid values: [{}:{}]", registers_name, 0, MAX_REGISTER));
            
            res.begin = min_reg + registers->begin;
            res.end = min_reg + registers->end;
          }
          else
          {
            res.hasRegisters = false;
            res.begin = 0;
            res.end = 0;
          }

          return res;
        };

        auto [hasTextures, currentTexReg, texturesEnd] = getRegistersInfo(m_Scope.textureRegisters, "texture", TEXTURE_REGISTERS_BEGIN);
        auto [hasSamplers, currentSamplerReg, samplersEnd] = getRegistersInfo(m_Scope.samplerRegisters, "sampler", SAMPLER_REGISTERS_BEGIN);
        auto [hasBuffers, currentBufferReg, buffersEnd] = getRegistersInfo(m_Scope.bufferRegisters, "buffer", BUFFER_REGISTERS_BEGIN);

        if (m_Scope.cbuffer == CBUFFER_NOT_ASSIGNED)
        {
          if (!hasBuffers)
            throw std::runtime_error("error: scope declared cbuffer variables but there are no buffer registers reserved");
          
          m_Scope.cbuffer = currentBufferReg++;
          string name = m_Scope.name + "_cbuffer";
          m_Scope.declaredResources.insert({
            str_hash(name.c_str()),
            ResourceDeclaration{
              .type = ResourceType::Cbuffer,
              .name = name,
              .dset =  m_Scope.descriptorSet,
              .binding = m_Scope.cbuffer
            }
          });
        }

        for (auto&[_, resource]: m_Scope.declaredResources)
        {
          resource.dset = m_Scope.descriptorSet;
          switch (resource.type)
          {
            case ResourceType::Cbuffer: break;

            case ResourceType::Sampler:
            {
              if (!hasSamplers)
                 throw std::runtime_error(fmt::format(
                  "failed to declare sampler {}: there is no samplers declared in scope", resource.name));

              if (currentSamplerReg > samplersEnd)
                throw std::runtime_error(fmt::format(
                  "failed to declare sampler {}: all sampler slots are already in use", resource.name));

              resource.binding = currentSamplerReg++;
              break;
            }

            case ResourceType::Buffer:
            case ResourceType::RWStructuredBuffer:
            case ResourceType::RWBuffer:
            {
              if (!hasBuffers)
                 throw std::runtime_error(fmt::format(
                  "failed to declare a buffer {}: there is no buffers declared in scope", resource.name));

              if (currentBufferReg > buffersEnd)
                throw std::runtime_error(fmt::format(
                  "failed to declare a buffer {}: all buffer slots are already in use", resource.name));

              resource.binding = currentBufferReg++;
              break;
            }

            case ResourceType::Texture2D:
            case ResourceType::Texture3D:
            case ResourceType::TextureCube:
            case ResourceType::RWTexture2D:
            case ResourceType::RWTexture3D:
            {
              if (!hasTextures)
                 throw std::runtime_error(fmt::format(
                  "failed to declare a texture {}: there is no texture declared in scope", resource.name));

              if (currentTexReg > texturesEnd)
                throw std::runtime_error(fmt::format(
                  "failed to declare a texture {}: all texture slots are already in use", resource.name));

              resource.binding = currentTexReg++;
              break;
            }

            default:
            {
              ASSERT(!"unsupported");
              throw std::runtime_error("unsupported");
            }
          }
        }
      }

      void placeScopeCbufferVars()
      {
        size_t offset = 0;
        for (auto& [nameHash, var]: m_Scope.cbufferVariables)
        {
          var.offset = offset;
          offset += getCbufferPlacingSize(var.type);
        }

        m_Scope.cbufferSize = offset;
      }

      size_t getCbufferPlacingSize(const gapi::AttributeType type)
      {
        switch (type)
        {
          case gapi::AttributeType::UInt:
          case gapi::AttributeType::UInt2:
          case gapi::AttributeType::UInt3:
          case gapi::AttributeType::UInt4:
          case gapi::AttributeType::Int:
          case gapi::AttributeType::Int2:
          case gapi::AttributeType::Int3:
          case gapi::AttributeType::Int4:
          case gapi::AttributeType::Float:
          case gapi::AttributeType::Float2:
          case gapi::AttributeType::Float3:
          case gapi::AttributeType::Float4: return 4*4;
          case gapi::AttributeType::Float4x4: return (4*4)*4;

          default:
          {
            ASSERT(!"unsupported");
            return 0;
          }
        }
      }

      size_t getCbufferPadSize(const gapi::AttributeType type)
      {
        switch (type)
        {
          case gapi::AttributeType::UInt:
          case gapi::AttributeType::Int:
          case gapi::AttributeType::Float: return 3;

          case gapi::AttributeType::UInt2:
          case gapi::AttributeType::Int2:
          case gapi::AttributeType::Float2: return 2;

          case gapi::AttributeType::UInt3:
          case gapi::AttributeType::Int3:
          case gapi::AttributeType::Float3: return 1;

          case gapi::AttributeType::UInt4:
          case gapi::AttributeType::Int4:
          case gapi::AttributeType::Float4x4:
          case gapi::AttributeType::Float4: return 0;

          default:
          {
            ASSERT(!"unsupported");
            return 0;
          }
        }
      }

      void generateHlslCodeForScope()
      {
        m_Scope.hlslResourceDeclaration += generateHlslCbufferDecl();
        m_Scope.hlslResourceDeclaration += generateHlslResourcesDecl();
      }

      string generateHlslCbufferDecl() const
      {
        string sp = "  ";
        string cbufferStruct = fmt::format("{}_uniforms", m_Scope.name);

        string hlsl = fmt::format("struct {}\n", cbufferStruct)
          + "{\n";

        size_t i = 0;
        for (auto& [_, var]: m_Scope.cbufferVariables)
        {
          const string v = fmt::format("{} {};", gapi::attributeType_to_string(var.type), var.name);
          hlsl += sp + v + "\n";
          const string pad = getCbufferPad(var.type, i++);
          if (pad != "")
            hlsl += sp + pad + "\n";
        }
        hlsl += "};\n";

        hlsl += fmt::format("ConstantBuffer<{}> {}: register(b{}, space{});\n",
                cbufferStruct, m_Scope.name, m_Scope.cbuffer, m_Scope.descriptorSet);

        return hlsl;
      }

      string getCbufferPad(const gapi::AttributeType type, const size_t i) const
      {
        const auto getPadName = [&](const int size)
                                { return fmt::format("int{} {}{};", size, "__pad", i); };
        switch (type)
        {
          case gapi::AttributeType::UInt:
          case gapi::AttributeType::Int:
          case gapi::AttributeType::Float: return getPadName(3);

          case gapi::AttributeType::UInt2:
          case gapi::AttributeType::Int2:
          case gapi::AttributeType::Float2: return getPadName(2);

          case gapi::AttributeType::UInt3:
          case gapi::AttributeType::Int3:
          case gapi::AttributeType::Float3: return getPadName(1);

          case gapi::AttributeType::UInt4:
          case gapi::AttributeType::Int4:
          case gapi::AttributeType::Float4x4:
          case gapi::AttributeType::Float4: return "";

          default:
          {
            ASSERT(!"unsupported");
            return "";
          }
        }
      }

      string generateHlslResourcesDecl() const
      {
        string hlsl;
        const auto getStorageType = [](const ResourceElemStorageType& t) {
          if (auto structName = std::get_if<const char*>(&t))
          {
            return fmt::format("<{}>", *structName);
          }
          else if (auto attrType = std::get_if<gapi::AttributeType>(&t))
          {
            return *attrType != gapi::AttributeType::None ? 
                      fmt::format("<{}>", gapi::attributeType_to_string(*attrType)) :
                      "";
          }
        };

        for (const char* hlslCode : m_Scope.declaredHlslResources)
          hlsl += hlslCode;

        for (auto& [_, var]: m_Scope.declaredResources)
        {
          if (var.type == ResourceType::Cbuffer)
            continue;

          hlsl += fmt::format("{}{} {}: register({}{}, space{});\n",
            to_hlsl_type(var.type), 
            getStorageType(var.elemStorageType),
            var.name,
            to_hlsl_register(var.type),
            var.binding, var.dset);
        }

        return hlsl;
      }

      void generateByteCodeForScope()
      {
        ByteCodes cbufferVars;
        ByteCodes byteCode;

        for (const auto& [_, var]: m_Scope.cbufferVariables)
        {
          const ByteCodes bc = generateByteCodeForCbufferVar(var);
          cbufferVars.insert(cbufferVars.end(), bc.begin(), bc.end());
        }

        for (const auto& [_, res]: m_Scope.declaredResources)
        {
          ByteCodes var;
          switch (res.type)
          {
            case ResourceType::Cbuffer:
            {
              break;
            }
            case ResourceType::Sampler:
            case ResourceType::Buffer:
            case ResourceType::Texture2D:
            case ResourceType::Texture3D:
            case ResourceType::TextureCube:
            case ResourceType::RWStructuredBuffer:
            case ResourceType::RWBuffer:
            case ResourceType::RWTexture2D:
            case ResourceType::RWTexture3D:
            {
              const ByteCodes var = generateByteCodeForResourceDeclaration(res);
              byteCode.insert(byteCode.end(), var.begin(), var.end());
              break;
            }
            default:
            {
              ASSERT(!"unsupported type");
              break;
            }
          }
        }

        if (!cbufferVars.empty())
        {
          byteCode.push_back(ShBeginCbuffer(m_Scope.name));
          byteCode.insert(byteCode.begin(), cbufferVars.begin(), cbufferVars.end());
          byteCode.push_back(ShEndCbuffer(m_Scope.descriptorSet, m_Scope.cbuffer));
        }

        m_Scope.byteCode = std::move(byteCode);
      }

      ByteCodes generateByteCodeForCbufferVar(const CBufferVar& res) const
      {
        ByteCodes bc;
        ASSERT(res.assignExp->type == ResourceAssignExp::Type::Access);
        const AccessResource* assignNode = reinterpret_cast<const AccessResource*>(res.assignExp);

        const string_hash resNameHash = str_hash(res.name.c_str());
        const auto it = m_Scope.cbufferVariables.find(resNameHash);
        if (it == m_Scope.cbufferVariables.end())
          throw std::runtime_error(fmt::format("failed to generate byte code for cbuffer var `{}`", res.name));

        ShByteCode b = ShBindCbufferVar(it->second.offset, assignNode->accessType, res.type, assignNode->resourceName);
        bc.push_back(b);

        return bc;
      }

      ByteCodes generateByteCodeForResourceDeclaration(const ResourceDeclaration& res) const
      {
        ByteCodes bc;
        ASSERT(res.assignExp->type == ResourceAssignExp::Type::Access);
        const AccessResource* assignNode = reinterpret_cast<const AccessResource*>(res.assignExp);

        ShByteCode b = ShBindResource(
          assignNode->accessType,
          res.type,
          assignNode->resourceName,
          res.dset,
          res.binding
        );

        bc.push_back(b);

        return bc;
      }

    private:
      Compiler& m_Compiler;

      ScopeDeclaration m_Scope;
  };

  bool Compiler::onScopeDeclaration(ScopeDeclarationExp* exp)
  {
    const char* scopeName = exp->name;
    try
    {
      m_GC.push_back(std::unique_ptr<Node>(exp));
      ScopeDeclarationExp& s = *exp;

      const string_hash scopeNameHash = str_hash(s.name);
      if (m_Module.declaredScopes.find(scopeNameHash) != m_Module.declaredScopes.end())
        throw std::runtime_error(
          fmt::format("failed to declare a new scope `{}`: it's already declared",
          s.name)
        );

      m_Module.declaredScopes.insert(scopeNameHash);

      ScopeProcessor sp(*this);
      sp.process(s, getCurrentFileName());
      ScopeDeclaration scope = sp.takeScope();

      const auto declaredSc = m_DeclaredScopes.find(scopeNameHash);
      if (declaredSc != m_DeclaredScopes.end())
      {
        if (declaredSc->second != scope)
          throw std::runtime_error(
            fmt::format("failed to redeclare a new scope `{}`: scope with such name already declared in module `{}` ",
            s.name, declaredSc->second.fileDeclaration));
      }
      else
      {
        m_DeclaredScopes.insert({
          scopeNameHash,
          scope
        });
        m_Bin.scopes.push_back(MaterialsBin::Scope{scope});
      }


      return true;
    }
    catch (std::exception& e)
    {
      markCompilationFailed();
      logerror("scope '{}' declaration error: {}", scopeName, e.what());
      return false;
    }
  }
}
