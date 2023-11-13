#include "compiler.h"
#include "spirv.h"

#include <engine/gapi/vulkan/resources.h>
#include <engine/log.h>
#include <engine/utils/string.h>

#include <Unknwn.h>
#include <dxc/dxcapi.h>

#include <algorithm>
#include <charconv>
#include <exception>
#include <filesystem>
#include <wrl/client.h>

namespace fs = std::filesystem;

namespace ShadersSystem
{
  namespace
  {
    ShaderBlob compile_shader_stage(const string& hlsl,
                                    const string& target, const gapi::ShaderStage stage,
                                    const string& shaderName,
                                    const string& entryName, const string& currentDir,
                                    const bool debug)
    {
      using Microsoft::WRL::ComPtr;
      ComPtr<IDxcUtils> pUtils;
      DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(pUtils.GetAddressOf()));

      ComPtr<IDxcCompiler3> pCompiler;
      DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(pCompiler.GetAddressOf()));

      ComPtr<IDxcBlobEncoding> pSource;
      pUtils->CreateBlob(hlsl.data(), hlsl.size(), CP_UTF8, pSource.GetAddressOf());

      ComPtr<IDxcIncludeHandler> pIncludeHandler;
      pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);

      const std::wstring wTarget(target.begin(), target.end());
      const std::wstring entry(entryName.begin(), entryName.end());
      const std::wstring dir(currentDir.begin(), currentDir.end());

      std::vector<LPCWSTR> args;
      args.push_back(L"-spirv");
      args.push_back(L"-HV");
      args.push_back(L"2018");
      args.push_back(L"-fspv-target-env=vulkan1.1");
      args.push_back(L"-E");
      args.push_back(entry.c_str());
      args.push_back(L"-T");
      args.push_back(wTarget.c_str());
      args.push_back(L"-I");
      args.push_back(dir.c_str());
      args.push_back(L"-Zi");
      args.push_back(debug ? L"-O0" : L"-O3");

      DxcBuffer dxcSrc;
      dxcSrc.Ptr = pSource->GetBufferPointer();
      dxcSrc.Size = pSource->GetBufferSize();
      dxcSrc.Encoding = 0;

      ComPtr<IDxcResult> pCompileResult;
      HRESULT hr = pCompiler->Compile(&dxcSrc, args.data(), args.size(), pIncludeHandler.Get(), IID_PPV_ARGS(pCompileResult.GetAddressOf()));
      ASSERT(SUCCEEDED(hr));

      {
        const char* error;

        ComPtr<IDxcBlobUtf8> dxcErrors;
        hr = pCompileResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(dxcErrors.GetAddressOf()), nullptr);
        ASSERT(SUCCEEDED(hr));

        if (dxcErrors && dxcErrors->GetStringLength() > 0)
        {
          error = (char*)dxcErrors->GetBufferPointer();
          throw std::runtime_error(fmt::format("DXC error: {}", error));
        }
      }

      ComPtr<IDxcBlob> blob;
      hr = pCompileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(blob.GetAddressOf()), nullptr);
      ASSERT(SUCCEEDED(hr));
      ASSERT(blob->GetBufferSize() != 0);

      ShaderBlob shBlob;
      shBlob.data.resize(blob->GetBufferSize());
      std::memcpy(shBlob.data.data(), blob->GetBufferPointer(), blob->GetBufferSize());

      shBlob.stage = stage;
      shBlob.name = shaderName;
      shBlob.entry = entryName;
      return shBlob;
    }

    class TechniqueProcessor
    {
      public:
        TechniqueProcessor(Compiler& compiler)
          : m_Compiler(compiler)
        {
          m_Hlsl += "#define __preshader__\n\n";
        }

        void process(const TechniqueDeclarationExp& tDecl)
        {
          m_TechniqueName = string(tDecl.name);
          processExps(*tDecl.exps);
          validateStages();
          generateReflections();
        }

        TechniqueDeclaration getTechniqueDecription() const
        {
          return TechniqueDeclaration
          { 
            .type = m_PipelineType,
            .name = std::move(m_TechniqueName),
            .byteCode = std::move(m_ByteCode),
            .renderState = std::move(m_RenderState),
            .blobs = std::move(m_Shaders),
            .dsets = std::move(m_DescriptorSets)
          };
        }

      private:
        void validateStages() const
        {
          if (m_Stages == 0)
            throw std::runtime_error("there are no stages declared");

          const auto graphicsBase = gapi::ShaderStage::Vertex | gapi::ShaderStage::Fragment;
          const auto geometry = gapi::ShaderStage::Geometry;
          const auto tesselation = gapi::ShaderStage::TessellationControl | gapi::ShaderStage::TessellationEvaluation;
          const auto compute = gapi::ShaderStage::Compute;
          const auto all = graphicsBase | compute;

          const auto tesselationStages = m_Stages & tesselation;
          const auto allGraphicsStages = m_Stages & (graphicsBase | tesselation | geometry);
          const auto computeStage = m_Stages & compute;

          if (allGraphicsStages && computeStage)
            throw std::runtime_error("technique can't have compute and graphics stages in the same time");
          
          if (tesselationStages && ((tesselationStages & tesselation) != tesselation))
            throw std::runtime_error("tesselation stages are not complete. missing HS or DS");

          if (allGraphicsStages && ((allGraphicsStages & graphicsBase) != graphicsBase))
            throw std::runtime_error("graphics stages are not complete. missing VS or PS");
        }

        void generateReflections()
        {
          eastl::vector<const ShadersSystem::ScopeDeclaration*> scopes;
          for (const auto& n: m_SupportedScopeNames)
            scopes.push_back(&m_Compiler.getScope(n));

          m_DescriptorSets = spirv::v2::reflect(scopes, m_Stages);
        }

        vk::DescriptorSetLayoutBinding& accessDsetBinding(const size_t set, const size_t binding)
        {
          if (m_DescriptorSets.size() <= set)
            m_DescriptorSets.resize(set+1);

          if (m_DescriptorSets[set].size() <= binding)
            m_DescriptorSets[set].resize(binding+1);

          return m_DescriptorSets[set][binding].vk;
        }

        void insureScopeNotSupportedYet(const string& name)
        {
          const string_hash nameHash = str_hash(name.c_str());
          if (m_SupportedScopes.find(nameHash) != m_SupportedScopes.end())
            throw std::runtime_error(fmt::format("can't support scope {}: it's already declared as supported", name));

          m_SupportedScopes.insert(nameHash);
          m_SupportedScopeNames.push_back(name);
        }

        void processTechniqueMacroInvoke(const string& techniqueName)
        {
          const auto& macroDecl = m_Compiler.getTechniqueMacroDeclaration(techniqueName);
          processExps(*macroDecl.techniqueExps);
        }

        void processScopeSupportExp(const string& scope, const bool activate)
        {
          if (m_Compiler.hasScope(scope))
          {
            insureScopeNotSupportedYet(scope);
            m_Hlsl += m_Compiler.getScopeHlsl(scope) + "\n";

            if (activate)
            {
              m_ByteCode.push_back(ShActivateScope::build(scope));
            }
          }
          else
          {
            const string err = fmt::format("can't support scope '{}': it's not declared", scope);
            throw std::runtime_error(err);
          }
        }

        void processInput(const InputExp* exp)
        {
          m_RenderState.ia = gapi::VertexInputDescription{};
          const InputBufferExp* bExp = exp->buffers;

          string sp = "  ";
          string hlsl = fmt::format("struct {}_input{{\n", m_TechniqueName.c_str());

          uint8_t location = 0;
          while (bExp)
          {
            uint32_t offset = 0;
            const InputAttributeExp* attrExp = bExp->attributes;
            while (attrExp)
            {
              m_RenderState.ia.attributes.push_back(gapi::VertexInputDescription::Attribute{
                .offset = offset,
                .location = location++,
                .binding = (uint8_t)bExp->reg,
                .type = attrExp->attributeType
              });

              offset += attributeType_to_size(attrExp->attributeType);
              hlsl += fmt::format("  {} {}: {};\n", attributeType_to_string(attrExp->attributeType), attrExp->name, attrExp->semantic);

              attrExp = attrExp->next;
            }

            m_RenderState.ia.buffers.push_back(gapi::VertexInputDescription::Buffer{
              .stride = offset,
              .binding = (uint8_t)bExp->reg
            });

            bExp = bExp->next;
          }
          hlsl += "};";

          m_Hlsl += hlsl;
        }

        void processHlsl(const HlslExp& hlslExp)
        {
          m_Hlsl += string(hlslExp.code);
        }


        struct ParsedTargetProfile
        {
          string targetProfile;
          gapi::ShaderStage stage;
        };
        auto parseTargetProfile(string_view target) -> ParsedTargetProfile
        {
          auto throwErr = [&](const char* txt = ""){ throw std::runtime_error(fmt::format("invalid compilation target {}: {}", target, txt)); };
          
          struct StageInfo
          {
            string_view stageStr;
            gapi::ShaderStage stage;
          };

          auto getShaderStage = [&]() -> StageInfo
          {
            if (target.length() < 2 || target[1] != 's')
              throwErr("invalid profile format");

            string_view stage = target.substr(0, 2);
            switch(stage[0])
            {
              case 'v': return {stage, gapi::ShaderStage::Vertex};
              case 'p': return {stage, gapi::ShaderStage::Fragment};
              case 'c': return {stage, gapi::ShaderStage::Compute};
              case 'h': return {stage, gapi::ShaderStage::TessellationControl};
              case 'd': return {stage, gapi::ShaderStage::TessellationEvaluation};
              default: throwErr("unknown shader");
            }
            return {};
          };

          auto toNumOrThrow = [&](string_view v)
          {
            int i = 0;
            auto res = std::from_chars(v.data(), v.data() + v.size(), i);
            if (res.ec == std::errc::invalid_argument)
              throwErr("invalid version");
            return (size_t)i;
          };

          struct MajorMinor{size_t major; size_t minor;};
          auto getMajorMinorVersion = [&]() -> MajorMinor
          {
            auto shMajorMinor = Utils::split_view(target, '_');

            if (shMajorMinor.size() == 1)
              return {6, 1};
            else if (shMajorMinor.size() == 3)
            {
              size_t major = toNumOrThrow(shMajorMinor[1]);
              size_t minor = toNumOrThrow(shMajorMinor[2]);
              return {major, minor};
            }
            else
              throwErr("invalid format. Valid example: {vs_6_1 or vs}");
          };

          auto [stageStr, gapiStage] = getShaderStage();
          auto [major, minor] = getMajorMinorVersion();
          return {
            .targetProfile = fmt::format("{}_{}_{}", stageStr, major, minor),
            .stage = gapiStage,
          };
        }

        void processCompile(const CompileExp& compileExp)
        {
          const auto [targetProfile, stage] = parseTargetProfile(compileExp.targetProfile);
          const string shaderName = fmt::format("{}_{}", m_TechniqueName, targetProfile);

          if (m_Stages & stage)
            throw std::runtime_error(fmt::format("failed to compile technique: `{}`, this stage is already compiled", shaderName));

          m_Stages = gapi::ShaderStage(m_Stages | stage);
          m_PipelineType = stage == gapi::ShaderStage::Compute ?
                            gapi::PipelineType::Compute :
                            gapi::PipelineType::Graphics;

          if (stage == gapi::ShaderStage::TessellationControl)
            m_RenderState.tsInputControlPatchCount = 1;

          try
          {
            const string currentDir = fs::path(m_Compiler.getCurrentFileName()).parent_path().string();
            const bool debug = m_Compiler.getFlags() & COMPILE_DEBUG;
            ShaderBlob blob = compile_shader_stage(m_Hlsl, targetProfile, stage, shaderName, compileExp.entry, currentDir, debug);
            m_Shaders.push_back(std::move(blob));
          }
          catch (std::exception&)
          {
            logerror("failed to compile technique `{}`\n==================SHADER DUMP=================\n{}\n==================SHADER DUMP END=================",
              shaderName, m_Hlsl);
            throw;
          }
        }

        gapi::AttachmentBlendState processMrtBlending(const MrtBlendingExp* exp)
        {
          gapi::AttachmentBlendState mrtState;

          #define SET_MRT_STATE(type, state, nodeType, nodeParam) case MrtBlendingExp::Type::type:\
          {\
            mrtState.state = reinterpret_cast<const nodeType*>(exp)->nodeParam;\
            break;\
          }

          while (exp)
          {
            switch (exp->mrtBlendingStateType)
            {
              SET_MRT_STATE(Enabling, blendEnabled, MrtBlendingEnabledExp, enabled)
              SET_MRT_STATE(SrcColorBlendFactor, srcColorBlendFactor, MrtSrcColorBlendFactorExp, factor)
              SET_MRT_STATE(DstColorBlendFactor, dstColorBlendFactor, MrtDstColorBlendFactorExp, factor)
              SET_MRT_STATE(ColorBlendOp, colorBlendOp, MrtColorBlendOpExp, op)
              SET_MRT_STATE(SrcAlphaBlendFactor, srcAlphaBlendFactor, MrtSrcAlphaBlendFactorExp, factor)
              SET_MRT_STATE(DstAlphaBlendFactor, dstAlphaBlendFactor, MrtDstAlphaBlendFactorExp, factor)
              SET_MRT_STATE(AlphaBlendOp, alphaBlendOp, MrtAlphaBlendOpExp, op)
            }
            exp = exp->next;
          }

          #undef SET_MRT_STATE

          return mrtState;
        }

        void processMrtBlendings()
        {
          size_t mrtCount = 0;
          for (const auto& mrt: m_MrtBlendingStates)
            mrtCount = (mrt->n+1 > mrtCount) ? mrt->n+1 : mrtCount;

          if (mrtCount >= gapi::MAX_RENDER_TARGETS)
            throw std::runtime_error(fmt::format("invalid mrt({}),  max number: {}", mrtCount-1, gapi::MAX_RENDER_TARGETS-1));

          for (size_t i = 0; i < mrtCount; ++i)
            m_RenderState.blending.attachments.push({});

          for (const MrtBlendingExp* mrt: m_MrtBlendingStates)
          {
            gapi::AttachmentBlendState& st = m_RenderState.blending.attachments.get(mrt->n);
            st = processMrtBlending(mrt);
          }
        }

        void processDepth(const DepthExp* exp)
        {
          #define SET_RENDER_STATE(type, state, nodeType, nodeParam) case DepthExp::Type::type:\
          {\
            m_RenderState.state = static_cast<const nodeType*>(exp)->nodeParam;\
            break;\
          }

          while (exp)
          {
            switch (exp->type)
            {
              SET_RENDER_STATE(Test,  depthStencil.depthTestEnabled,  DepthTestExp,  enabled);
              SET_RENDER_STATE(Write, depthStencil.depthWriteEnabled, DepthWriteExp, enabled);
              SET_RENDER_STATE(Op,    depthStencil.depthOp,           DepthOpExp,    op);
            }
            exp = exp->next;
          }

          #undef SET_RENDER_STATE
        }

        void processStencil(const StencilExp* exp)
        {
          #define SET_RENDER_STATE(type, state, nodeType, nodeParam) case StencilExp::Type::type:\
          {\
            m_RenderState.state = static_cast<const nodeType*>(exp)->nodeParam;\
            break;\
          }

          while (exp)
          {
            switch (exp->type)
            {
              SET_RENDER_STATE(Test,           depthStencil.stencilTestEnabled,    StencilTestExp,           enabled);
              SET_RENDER_STATE(FailOp,         depthStencil.stencilFailOp,         StencilFailOpExp,         op);
              SET_RENDER_STATE(PassOp,         depthStencil.stencilPassOp,         StencilPassOpExp,         op);
              SET_RENDER_STATE(DepthFailOp,    depthStencil.stencilDepthFailOp,    StencilDepthFailOpExp,    op);
              SET_RENDER_STATE(CompareOp,      depthStencil.stencilCompareOp,      StencilCompareOpExp,      op);
              SET_RENDER_STATE(ReferenceValue, depthStencil.stencilReferenceValue, StencilReferenceValueExp, value);
            }
            exp = exp->next;
          }

          #undef SET_RENDER_STATE
        }

        void processBlending(const BlendingExp* exp)
        {
          #define SET_RENDER_STATE(type, state, nodeType, nodeParam) case BlendingExp::Type::type:\
          {\
            m_RenderState.state = reinterpret_cast<const nodeType*>(exp)->nodeParam;\
            break;\
          }

          while (exp)
          {
            switch (exp->blendingStateType)
            {
              SET_RENDER_STATE(LogicOpEnabling, blending.logicOpEnabled, LogicOpEnablingExp, enabled)
              SET_RENDER_STATE(LogicOp,         blending.logicOp,        LogicOpExp,         op)
              SET_RENDER_STATE(BlendConstants,  blending.blendConstants, BlendConstants,     val)
              case BlendingExp::Type::MrtState:
              {
                const MrtBlendingExp* mrtBlendingExp = reinterpret_cast<const MrtBlendingExp*>(exp);
                m_MrtBlendingStates.push_back(mrtBlendingExp);
                //m_RenderState.blending.attachments.push(processMrtBlending(mrtBlendingExp));
                break;
              }
            }
            exp = exp->next;
          }

          #undef SET_RENDER_STATE
        }

        void processRenderState(const RenderStateExp* exp)
        {
          #define SET_RENDER_STATE(type, state, nodeType, nodeParam) \
            case RenderStateExp::StateType::type:\
            {\
              m_RenderState.state = static_cast<const nodeType*>(exp)->nodeParam;\
              break;\
            }

          #define PROCESS_STATE(type) \
          case RenderStateExp::StateType::type:\
          {\
            const auto* e = static_cast<const type ## Exp*>(exp);\
            process ## type(e);\
            break;\
          }

          while (exp)
          {
            switch (exp->stateType)
            {
              SET_RENDER_STATE(CullMode, cullMode, CullModeExp, cm);
              SET_RENDER_STATE(PolygonMode, polygonMode, PolygonModeExp, pm);
              SET_RENDER_STATE(PrimitiveTopology, topology, PrimitiveTopologyExp, tp);
              PROCESS_STATE(Depth);
              PROCESS_STATE(Stencil);
              PROCESS_STATE(Blending);
              PROCESS_STATE(Input);
            }
            exp = exp->next;
          }

          processMrtBlendings();

          #undef SET_RENDER_STATE
          #undef PROCESS_STATE
        }

        void processExps(const TechniqueExp& exps)
        {
          const TechniqueExp* exp = &exps;
          while (exp)
          {
            switch (exp->type)
            {
              case TechniqueExp::Type::MacroInvoke:
              {
                const TechniqueMacroInvokeExp* mExp = reinterpret_cast<const TechniqueMacroInvokeExp*>(exp);
                processTechniqueMacroInvoke(mExp->techniqueName);
                break;
              }
              case TechniqueExp::Type::ScopeSupport:
              {
                const ScopeSupportExp* scExp = reinterpret_cast<const ScopeSupportExp*>(exp);
                processScopeSupportExp(scExp->name, false);
                break;
              }
              case TechniqueExp::Type::ScopeActivate:
              {
                const ScopeActivateExp* scExp = reinterpret_cast<const ScopeActivateExp*>(exp);
                processScopeSupportExp(scExp->name, true);
                break;
              }
              case TechniqueExp::Type::Hlsl:
              {
                const HlslExp* hlslExp = reinterpret_cast<const HlslExp*>(exp);
                processHlsl(*hlslExp);
                break;
              }
              case TechniqueExp::Type::Compile:
              {
                const CompileExp* compileExp = reinterpret_cast<const CompileExp*>(exp);
                processCompile(*compileExp);
                break;
              }
              case TechniqueExp::Type::RenderState:
              {
                const RenderStateExp* rsExp = reinterpret_cast<const RenderStateExp*>(exp);
                processRenderState(rsExp);
                break;
              }
              default:
              {
                ASSERT(!"unsupported type");
                break;
              }
            }
            exp = exp->next;
          }
        }

      private:
        string m_TechniqueName;
        string m_Hlsl;
        Compiler& m_Compiler;

        eastl::vector_set<string_hash> m_SupportedScopes;
        eastl::vector<string> m_SupportedScopeNames;
        ByteCodes m_ByteCode;

        eastl::vector<const MrtBlendingExp*> m_MrtBlendingStates;

        tfx::RenderState m_RenderState;
        gapi::VertexInputDescription m_Input;
        gapi::PipelineType m_PipelineType;
        gapi::ShaderStage m_Stages = gapi::ShaderStage(0);
        eastl::vector<ShaderBlob> m_Shaders;
        eastl::vector<spirv::v2::DescriptorSet> m_DescriptorSets;
    };
  }

  bool Compiler::onTechniqueDeclaration(TechniqueDeclarationExp* exp)
  {
    try {
      m_GC.push_back(std::unique_ptr<Node>(exp));
      TechniqueDeclarationExp& tDecl = *exp;

      const string_hash nameHash = str_hash(tDecl.name);
      if (m_Module.declaredTechniques.find(nameHash) != m_Module.declaredTechniques.end())
        throw std::runtime_error(fmt::format("failed to declare a new technique `{}`: it's already declared", tDecl.name));
      else
        m_Module.declaredTechniques.insert(nameHash);

      TechniqueProcessor tp(*this);
      tp.process(tDecl);
      TechniqueDeclaration tq = tp.getTechniqueDecription();

      const auto declaredTq = m_DeclaredTechniques.find(nameHash);
      if (declaredTq != m_DeclaredTechniques.end())
      {
        if (declaredTq->second == tq)
          throw std::runtime_error(fmt::format("failed to redeclare a technique `{}`", tq.name));
      }
      else
      {
        m_DeclaredTechniques.insert({
          nameHash,
          tq
        });
        m_Bin.techniques.push_back(tp.getTechniqueDecription());
      }

      return true;
    }
    catch (std::exception& e)
    {
      markCompilationFailed();
      logerror("technique declaration error: {}", e.what());
      return false;
    }
  }

  bool Compiler::onTechniqueMacroDeclaration(TechniqueMacroDeclarationExp* exp)
  {
    try {
      m_GC.push_back(std::unique_ptr<Node>(exp));
      const string_hash nameHash = str_hash(exp->name);

      if (m_Module.macros.find(nameHash) != m_Module.macros.end())
        throw std::runtime_error(fmt::format("macro is already declared"));

      m_Module.macros.insert({
        nameHash,
        exp
      });

      return true;
    } catch (std::exception& e)
    {
      markCompilationFailed();
      logerror("technique macro `{}` declaration error: {}", exp->name, e.what());
      return false;
    }
  }
}
