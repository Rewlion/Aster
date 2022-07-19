#include "compiler.h"

#include <engine/assert.h>
#include <engine/log.h>

#include <exception>
#include <cstdio>
extern int shparse(ShadersSystem::Compiler& compiler);
extern void shrestart(FILE* f);
extern FILE *shin;

namespace ShadersSystem
{
  string Compiler::getScopeHlsl(const string& scope) const
  {
    return m_DeclaredScopes.find(str_hash(scope.c_str()))->second.hlslResourceDeclaration;
  }

  bool Compiler::hasScope(const string& scope) const
  {
    return m_DeclaredScopes.find(str_hash(scope.c_str())) != m_DeclaredScopes.end();
  }

  bool Compiler::compileModuleFromFile(const string& file)
  {
    FILE* f = fopen(file.c_str(), "r");

    if (!f)
    {
        fprintf(stderr, "can.t open a file");
        return false;
    }

    m_IsCompilationOk = true;
    m_CurrentCompilationFile = file;
    m_Module = ModuleResources{};

    shin = f;
    shrestart(f);
    shparse(*this);
    fclose(f);

    return m_IsCompilationOk;
  }

  Scope Compiler::constructScope(const ScopeDeclaration& s)
  {
    Scope scope;
    scope.fileDeclaration = m_CurrentCompilationFile;
    scope.name = string(s.name);

    const ScopeExp* exp = s.exps;
    while (exp != nullptr)
    {
      switch(exp->type)
      {
        case ScopeExp::Type::ShadersResourcesReserve:
        {
          const ShadersResourcesReserve* e = reinterpret_cast<const ShadersResourcesReserve*>(exp);
          addShadersResourcesReserve(scope, *e);
          break;
        }

        case ScopeExp::Type::ResourceDeclaration:
        {
          const ResourceDeclarationExp* e = reinterpret_cast<const ResourceDeclarationExp*>(exp);
          addResourceDeclaration(scope, *e);
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

    placeScopeResources(scope);
    placeScopeCbufferVars(scope);
    generateHlslCodeForScope(scope);
    generateByteCodeForScope(scope);

    return scope;
  }

  void Compiler::addShadersResourcesReserve(Scope& scope, const ShadersResourcesReserve& resourcesReserve)
  {
    scope.shaders = resourcesReserve.shaders->shaders;
    const ResourceReserveExp* exp = resourcesReserve.exps;
    while (exp != nullptr)
    {
      switch (exp->reserveExpType)
      {
        case ResourceReserveExp::Type::DescriptorSet:
        {
          const DescriptorSetReserveExp* e = reinterpret_cast<const DescriptorSetReserveExp*>(exp);
          scope.descriptorSet = e->dset;
          break;
        }

        case ResourceReserveExp::Type::CBuffer:
        {
          const CBufferReserveExp* e = reinterpret_cast<const CBufferReserveExp*>(exp);
          scope.cbuffer = e->cbuffer;
          string name = scope.name + "_cbuffer";
          scope.declaredResources.insert({
            str_hash(name.c_str()),
            ResourceDeclaration{
              .type = ResourceType::Cbuffer,
              .name = name,
            }
          });
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
              scope.textureRegisters = r;
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

  void Compiler::addResourceDeclaration(Scope& scope, const ResourceDeclarationExp& resourceDeclaration)
  {
    const string_hash resNameHash = str_hash(resourceDeclaration.name);
    if (scope.declaredResources.find(resNameHash) != scope.declaredResources.end())
    {
      logerror("error: failed to declare a resource `{}`: it's already declared", resourceDeclaration.name);
      markCompilationFailed();
      return;
    }

    switch (resourceDeclaration.resourceType)
    {
      case ResourceType::Texture2D:
      {
        scope.declaredResources.insert({
          resNameHash,
          ResourceDeclaration{
            .type = resourceDeclaration.resourceType,
            .name = resourceDeclaration.name,
            .dset = 0,
            .binding = 0,
            .assignExp = resourceDeclaration.assignExps
          }
        });
        break;
      }

      CASE_CBUFFER_VARS:
      {
        scope.cbufferVariables.push_back(CBufferVar{
          .type = resourceDeclaration.resourceType,
          .name = resourceDeclaration.name,
          .offset = 0,
          .assignExp = resourceDeclaration.assignExps
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

  void Compiler::placeScopeResources(Scope& scope)
  {
    const bool hasTextures = scope.textureRegisters.has_value();
    const size_t texturesEnd = hasTextures ? scope.textureRegisters.value().end : 0;
    size_t currentTexReg = hasTextures ? scope.textureRegisters.value().begin : 0;

    for (auto&[_, resource]: scope.declaredResources)
    {
      resource.dset = scope.descriptorSet;
      switch (resource.type)
      {
        case ResourceType::Cbuffer:
        {
          resource.binding = scope.cbuffer;
          break;
        }

        case ResourceType::Texture2D:
        {
          if (!hasTextures)
          {
            markCompilationFailed();
            logerror("failed to declare texture2D {}: there is no texture declared in scope", resource.name);
            return;
          }
          if (currentTexReg > texturesEnd)
          {
            markCompilationFailed();
            logerror("failed to declare texture2D {}: all texture slots are already in use", resource.name);
            return;
          }
          resource.binding = currentTexReg++;
          break;
        }

        default:
        {
          ASSERT(!"unsupported");
          markCompilationFailed();
          break;
        }
      }
    }
  }

  static size_t get_cbuffer_padSize(const ResourceType type)
  {
    switch (type)
    {
      case ResourceType::Int:
      case ResourceType::Float: return 3;

      case ResourceType::Int2:
      case ResourceType::Float2: return 2;

      case ResourceType::Int3:
      case ResourceType::Float3: return 1;

      case ResourceType::Int4:
      case ResourceType::Float4: return 0;

      default:
      {
        ASSERT(!"unsupported");
        return 0;
      }
    }
  }

  static string get_cbuffer_pad(const ResourceType type, const size_t i)
  {
    const auto getPadName = [&](const int size)
                            { return fmt::format("int{} {}{};", size, "__pad", i); };
    switch (type)
    {
      case ResourceType::Int:
      case ResourceType::Float: return getPadName(3);

      case ResourceType::Int2:
      case ResourceType::Float2: return getPadName(2);

      case ResourceType::Int3:
      case ResourceType::Float3: return getPadName(1);

      case ResourceType::Int4:
      case ResourceType::Float4: return "";

      default:
      {
        ASSERT(!"unsupported");
        return 0;
      }
    }
  }

  static const char* get_resource_type_str(const ResourceType type)
  {
    switch (type)
    {
      case ResourceType::Int: return "int";
      case ResourceType::Float: return "float";

      case ResourceType::Int2: return "int2";
      case ResourceType::Float2: return "float2";

      case ResourceType::Int3: return "int3";
      case ResourceType::Float3: return "float3";

      case ResourceType::Int4: return "int4";
      case ResourceType::Float4: return "float4";

      default:
      {
        ASSERT(!"unsupported");
        return 0;
      }
    }
  }

  void Compiler::placeScopeCbufferVars(Scope& scope)
  {
    size_t offset = 0;
    for (auto& var: scope.cbufferVariables)
    {
      var.offset = offset;
      offset += get_cbuffer_padSize(var.type);
    }

    scope.cbufferSize = offset;
  }

  static string get_hlsl_cbuffer_decl(const Scope& scope)
  {
    string sp = "  ";
    string cbufferStruct = fmt::format("{}_uniforms", scope.name);

    string hlsl = fmt::format("struct {}\n", cbufferStruct)
      + "{\n";

    size_t i = 0;
    for (auto& var: scope.cbufferVariables)
    {
      const string v = fmt::format("{} {};", get_resource_type_str(var.type), var.name);
      hlsl += sp + v + "\n";
      const string pad = get_cbuffer_pad(var.type, i++);
      if (pad != "")
        hlsl += sp + pad + "\n";
    }
    hlsl += "};\n";

    hlsl += fmt::format("ConstantBuffer<{}> {}: register(b{}, space{});\n",
            cbufferStruct, scope.name, scope.cbuffer, scope.descriptorSet);

    return hlsl;
  }

  static string get_hlsl_resources_decl(Scope& scope)
  {
    string hlsl;
    for (auto& [_, var]: scope.declaredResources)
    {
      if (var.type == ResourceType::Cbuffer)
        continue;

      switch (var.type)
      {
        case ResourceType::Texture2D:
        {
          hlsl += fmt::format("Texture2D<float4> {}: register(t{}, space{});\n",
            var.name, var.binding, var.dset);
          break;
        }

        default:
        {
          ASSERT(!"unuspported");
          break;
        }
      }
    }

    return hlsl;
  }

  void Compiler::generateHlslCodeForScope(Scope& scope)
  {
    scope.hlslResourceDeclaration += get_hlsl_cbuffer_decl(scope);
    scope.hlslResourceDeclaration += get_hlsl_resources_decl(scope);
  }

  ByteCodes Compiler::generateByteCodeForCbufferVar(const ResourceDeclaration& res)
  {
    ByteCodes bc;
    ASSERT(res.assignExp->type == ResourceAssignExpNode::Type::Access);
    const AccessResource* assignNode = reinterpret_cast<const AccessResource*>(res.assignExp);

    return bc;
  }

  ByteCodes Compiler::generateByteCodeForTexture(const ResourceDeclaration& res)
  {
    ByteCodes bc;
    ASSERT(res.assignExp->type == ResourceAssignExpNode::Type::Access);
    const AccessResource* assignNode = reinterpret_cast<const AccessResource*>(res.assignExp);

    ShByteCode b = ShBindResource(
      assignNode->accessType,
      res.name,
      res.dset,
      res.binding
    );

    bc.push_back(b);

    return bc;
  }

  void Compiler::generateByteCodeForScope(Scope& scope)
  {
    ByteCodes cbufferVars;

    ByteCodes byteCode;

    for (const auto& [_, res]: scope.declaredResources)
    {
      ByteCodes var;
      switch (res.type)
      {
        CASE_CBUFFER_VARS:
        {
          var = generateByteCodeForCbufferVar(res);
          cbufferVars.insert(cbufferVars.end(), var.begin(), var.end());
          break;
        }
        case ResourceType::Cbuffer:
        {
          break;
        }
        case ResourceType::Texture2D:
        {
          const ByteCodes var = generateByteCodeForTexture(res);
          byteCode.insert(byteCode.end(), var.begin(), var.end());
          break;
        }
      }
    }

    if (!cbufferVars.empty())
    {
      byteCode.push_back(ShBeginCbuffer(scope.name));
      byteCode.insert(byteCode.begin(), cbufferVars.begin(), cbufferVars.end());
      byteCode.push_back(ShEndCbuffer());
    }

    scope.byteCode = std::move(byteCode);
  }

  void Compiler::onScopeDeclaration(const ScopeDeclaration& s)
  {
    const string_hash scopeNameHash = str_hash(s.name);
    if (m_Module.declaredScopes.find(scopeNameHash) != m_Module.declaredScopes.end())
    {
      logerror("failed to declare a new scope `{}`: it's already declared", s.name);
      markCompilationFailed();
      return;
    }
    m_Module.declaredScopes.insert(scopeNameHash);

    Scope scope = constructScope(s);
    const auto declaredSc = m_DeclaredScopes.find(scopeNameHash);
    if (declaredSc != m_DeclaredScopes.end())
      if (declaredSc->second == scope)
        throw std::runtime_error(fmt::format("failed to redeclare a new scope `{}`: scope with such name already declared in module `{}` ",
          s.name, declaredSc->second.fileDeclaration));
    else
      m_DeclaredScopes.insert({
        scopeNameHash,
        scope
      });

    m_DeclaredScopes.insert({
      scopeNameHash,
      scope
    });

    m_Bin.scopes.push_back(MaterialsBin::Scope{scope});
  }
}
