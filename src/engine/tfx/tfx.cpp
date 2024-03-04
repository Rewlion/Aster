#include "tfx.h"

#include <engine/algorithm/hash.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/gapi/gapi.h>
#include <engine/gapi/utils.h>
#include <engine/log.h>
#include <engine/utils/fs.h>

#include <shaders_compiler/ast_types.h>
#include <shaders_compiler/bin.h>
#include <shaders_compiler/serialization.h>

#include <EASTL/hash_map.h>

#include <fstream>
#include <variant>

namespace tfx
{
  namespace
  {
    inline
    auto get_param_bind_type(const Param& p) -> ShadersSystem::ResourceBindType
    {
      return p >> match {
        [](gapi::TextureHandle)  { return ShadersSystem::ResourceBindType::Texture; },
        [](Texture)              { return ShadersSystem::ResourceBindType::Texture; },
        [](TextureArray)         { return ShadersSystem::ResourceBindType::Texture; },
        [](gapi::BufferHandler)  { return ShadersSystem::ResourceBindType::Buffer; },
        [](gapi::SamplerHandler) { return ShadersSystem::ResourceBindType::Sampler; },
        [](auto)                 { return ShadersSystem::ResourceBindType::None;}
      };
    }


    struct Scope
    {
      string name;
      const ShadersSystem::ByteCodes& byteCode;
      eastl::vector<uint8_t> cpuCbuffer;
      gapi::BufferHandler gpuCbuffer;
    };

    struct Technique
    {
      using PipelineDesc = std::variant<gapi::GraphicsPipelineDescription, gapi::ComputePipelineDescription>;

      gapi::PipelineType type;
      string name;
      const ShadersSystem::ByteCodes& byteCode;
      PipelineDesc pipelineDesc;
    };

    template<class T>
    string dump_storage_table(const T& t, const string& table_name)
    {
      string resources;
      for (const auto& [h, res]: t)
        resources += fmt::format("  name:{} hash:{}\n", res.name, h);
      return fmt::format("storage: {}\n", table_name);
    }

    ShadersSystem::MaterialsBin mat_bin;
    eastl::hash_map<string_hash, Scope> scopes_storage;
    eastl::hash_map<string_hash, Technique> techniques_storage;
    eastl::hash_map<string_hash, Param> externs_storage;
    eastl::hash_map<string_hash, Param> channels_storage;

    class Processor
    {
      public:
        void activateScope(Scope& scope, gapi::CmdEncoder* cmd_encoder)
        {
          m_Scope = &scope;
          m_CmdEncoder = cmd_encoder;
          processByteCodes(m_Scope->byteCode);
        }

        void activateTechnique(const Technique& technique, gapi::CmdEncoder* cmd_encoder)
        {
          m_CmdEncoder = cmd_encoder;
          processByteCodes(technique.byteCode);

          if (technique.type == gapi::PipelineType::Graphics)
            m_CmdEncoder->bindGraphicsPipeline(std::get<gapi::GraphicsPipelineDescription>(technique.pipelineDesc));
          else
            m_CmdEncoder->bindComputePipeline(std::get<gapi::ComputePipelineDescription>(technique.pipelineDesc));
        }

      private:
        void processByteCodes(const ShadersSystem::ByteCodes& byte_codes)
        {
          for (const auto& bc: byte_codes)
            std::visit([this](const auto& b){
              processByteCode(b);
            }, bc);
        }

        struct ParamResult
        {
          const Param& value;
          bool result;
        };
        ParamResult getParam(ShadersSystem::ResourceAccessType access, const char* name)
        {
          const auto& storage = access == ShadersSystem::ResourceAccessType::Extern ?
                                externs_storage :
                                channels_storage;
          const string_hash h = str_hash(name);
          const auto it = storage.find(h);
          return {
            it->second, it != storage.end()
          };
        }

        void processByteCode(const ShadersSystem::ShBindResource& bc)
        {
          const auto& [resource, resourceExist] = getParam(bc.accessType, bc.resourceName);

          const ShadersSystem::ResourceBindType resBindType = ShadersSystem::to_resource_bind_type(bc.type);
          const auto isResourceValid = resourceExist && (get_param_bind_type(resource) == resBindType);

          switch (resBindType)
          {
            case ShadersSystem::ResourceBindType::Sampler:
            {
              bindSampler(bc, isResourceValid, resource);
              break;
            }
            case ShadersSystem::ResourceBindType::Buffer:
            {
              bindBuffer(bc, isResourceValid, resource);
              break;
            }
            case ShadersSystem::ResourceBindType::Texture:
            {
              bindTexture(bc, isResourceValid, resource);
              break;
            }
            default:
            {
              ASSERT_FMT(false, "unsupported byte code `{}`", (int)bc.type);
              break;
            }
          }
        }

        void bindSampler(const ShadersSystem::ShBindResource& bc, const bool is_res_valid, const Param& param)
        {
          const auto h = is_res_valid ? std::get<gapi::SamplerHandler>(param) : gapi::SamplerHandler::Invalid;
          m_CmdEncoder->bindSampler((gapi::SamplerHandler)h, bc.dset, bc.binding, 0);
        }

        void bindBuffer(const ShadersSystem::ShBindResource& bc, const bool is_res_valid, const Param& param)
        {
          const auto h = is_res_valid ? std::get<gapi::BufferHandler>(param) : gapi::BufferHandler::Invalid;
          m_CmdEncoder->bindBuffer(h, bc.dset, bc.binding, 0);
        }

        void bindTexture(const ShadersSystem::ShBindResource& bc, const bool is_res_valid, const Param& param)
        {
          if (is_res_valid) [[likely]]
          {
            param >> match {
              [&](const Texture& tex) {
                m_CmdEncoder->bindTexture(tex.h, bc.dset, bc.binding, 0, tex.mip);
              },
              [&](const gapi::TextureHandle& h) {
                m_CmdEncoder->bindTexture(h, bc.dset, bc.binding, 0, 0);
              },
              [&](const TextureArray& tex_array) {
                const size_t lastTex = tex_array.size();
                for (size_t i = 0; i < lastTex; ++i)
                {
                  const Texture& tex = tex_array[i];
                  m_CmdEncoder->bindTexture(tex.h, bc.dset, bc.binding, i, tex.mip);
                }
              },
              [](const auto) {
                ASSERT(!"invalid tfx param");
              },
            };
          }
          else
          {
            m_CmdEncoder->bindTexture(gapi::TextureHandle::Invalid, bc.dset, bc.binding, 0, 0);
          }
        }

        void processByteCode(const ShadersSystem::ShBeginCbuffer& bc)
        {
        }

        void processByteCode(const ShadersSystem::ShEndCbuffer& bc)
        {
          void* dstBuffer = gapi::map_buffer(m_Scope->gpuCbuffer, gapi::WR_DISCARD);
          std::memcpy(dstBuffer, m_Scope->cpuCbuffer.data(), m_Scope->cpuCbuffer.size());
          gapi::unmap_buffer(m_Scope->gpuCbuffer);

          m_CmdEncoder->bindConstBuffer(m_Scope->gpuCbuffer, bc.dset, bc.binding);
        }

        void processByteCode(const ShadersSystem::ShBindCbufferVar& bc)
        {
          const auto& [param, resourceExist] = getParam(bc.accessType, bc.resourceName);

          uint8_t* cbufferDataStart = m_Scope->cpuCbuffer.data() + bc.offset;

          #define SETUP_CBUFFER_VAR(AttrType, T, def) \
            case gapi::AttributeType::AttrType:\
            {\
              const T value = resourceExist && std::holds_alternative<T>(param) ? std::get<T>(param) : def;\
              const size_t start = bc.offset;\
              const size_t ending = start + sizeof(T);\
              const size_t validRange =  m_Scope->cpuCbuffer.size();\
              ASSERT_FMT( ending <= validRange, "Cbuffer placing is out of range. validRange: [0,{}), placing range: [{},{}). Variable name: {}", validRange, start, ending, bc.resourceName);\
              *(T*)(cbufferDataStart) = value;\
              break;\
            }

          switch (bc.type)
          {
            SETUP_CBUFFER_VAR(UInt,     uint,     0)
            SETUP_CBUFFER_VAR(UInt2,    uint2,    (uint2{0,0}))
            SETUP_CBUFFER_VAR(UInt3,    uint3,    (uint3{0,0,0}))
            SETUP_CBUFFER_VAR(UInt4,    uint4,    (uint4{0,0,0,0}))
            SETUP_CBUFFER_VAR(Int,      int,      0)
            SETUP_CBUFFER_VAR(Int2,     int2,     (int2{0,0}))
            SETUP_CBUFFER_VAR(Int3,     int3,     (int3{0,0,0}))
            SETUP_CBUFFER_VAR(Int4,     int4,     (int4{0,0,0,0}))
            SETUP_CBUFFER_VAR(Float,    float,    0.0f)
            SETUP_CBUFFER_VAR(Float2,   float2,   (float2{0.0, 0.0}))
            SETUP_CBUFFER_VAR(Float3,   float3,   (float3{0.0, 0.0, 0.0}))
            SETUP_CBUFFER_VAR(Float4,   float4,   (float4{0.0, 0.0, 0.0, 0.0}))
            SETUP_CBUFFER_VAR(Float4x4, float4x4, (float4x4{0.0}))
            default:
            {
              ASSERT(!"unsupported");
            }
          }
          #undef SETUP_CBUFFER_VAR
        }

        void processByteCode(const ShadersSystem::ShActivateScope& bc)
        {
          const string_hash h = str_hash(bc.scopeName);
          auto it = scopes_storage.find(h);
          if (it != scopes_storage.end())
            activateScope(it->second, m_CmdEncoder);
        }

      private:
        Scope* m_Scope = nullptr;
        gapi::CmdEncoder* m_CmdEncoder = nullptr;
    };
  }

  static void load_scopes(const eastl::vector<ShadersSystem::MaterialsBin::Scope>& mat_scopes)
  {
    for (const auto& matScope: mat_scopes)
    {
      const string_hash h = str_hash(matScope.name.c_str());
      ASSERT(scopes_storage.find(h) == scopes_storage.end());

      Scope s{
        .name = matScope.name,
        .byteCode = matScope.byteCode
      };

      if (matScope.cbufferSize > 0)
      {
        s.cpuCbuffer.resize(matScope.cbufferSize);
        std::memset(s.cpuCbuffer.data(), 0, matScope.cbufferSize);
        s.gpuCbuffer = gapi::allocate_buffer(matScope.cbufferSize, gapi::BF_BindConstant | gapi::BF_CpuVisible);
      }

      scopes_storage.insert({
        h,
        std::move(s)
      });

      loginfo("tfx: loaded scope `{}`:{}", matScope.name, h);
    }
  }

  static void load_techniques(const eastl::vector<ShadersSystem::MaterialsBin::Technique>& mat_techniques)
  {
    for (const auto& t: mat_techniques)
    {
      const string_hash h = str_hash(t.name.c_str());
      ASSERT_FMT(techniques_storage.find(h) == techniques_storage.end(), "technique {} already exist", t.name);

      gapi::PipelineLayoutHandler layout = gapi::add_pipeline_layout((void*)&t.dsets);

      auto technique = Technique{
        .type = t.type,
        .name = t.name,
        .byteCode = t.byteCode
      };

      eastl::vector<gapi::ShaderModuleHandler> modules;
      modules.reserve(t.blobs.size());
      for (size_t i = 0; i < t.blobs.size(); ++i)
      {
        const gapi::ShaderModuleHandler mh = gapi::add_module((void*)&t.blobs[i]);
        modules.push_back(mh);
      }

      if (t.type == gapi::PipelineType::Graphics)
      {
        technique.pipelineDesc = gapi::GraphicsPipelineDescription{
          .layout = layout,
          .shaders = std::move(modules),
          .ia = t.renderState.ia,
          .cullMode = t.renderState.cullMode,
          .polygonMode = t.renderState.polygonMode,
          .topology = t.renderState.topology,
          .depthStencilState = t.renderState.depthStencil,
          .blendState = t.renderState.blending,
          .tsInputControlPatchCount = t.renderState.tsInputControlPatchCount,
          .name = t.name
        };
      }
      else
      {
        technique.pipelineDesc = gapi::ComputePipelineDescription{
          .layout = layout,
          .shader = modules[0],
          .name = t.name,
          .numthreads = t.blobs[0].reflection.numthreads
        };
      }

      techniques_storage.insert({
        h,
        std::move(technique)
      });

      loginfo("tfx: loaded {} technique `{}`", gapi::to_string(t.type), t.name);
    }
  }

  void load_materials_bin(string_view file)
  {
    loginfo("tfx: loading materials bin {}", file);
    const eastl::vector<char> rawMatBin = Utils::read_file(file);
    ASSERT_FMT_RETURN(!rawMatBin.empty(), , "failed to load materials binary");

    mat_bin = ShadersSystem::MaterialsBin{};
    scopes_storage.clear();
    techniques_storage.clear();

    zpp::bits::in in(rawMatBin);
    const std::error_code err = std::make_error_code(in(mat_bin));
    ASSERT_FMT_RETURN(err.value() == 0, , "tfx: failed to deserialize materials bin: `{}`", err.message());

    loginfo("tfx: loading tfx scopes");
    load_scopes(mat_bin.scopes);
    loginfo("tfx: loading tfx techniques");
    load_techniques(mat_bin.techniques);
    loginfo("tfx: successfuly loaded materials bin");
  }

  void reload_materials_bin(string_view file)
  {
    loginfo("tfx: reloading of materials bin trigerred");
    gapi::wait_gpu_idle();
    gapi::free_pipeline_resources();
    load_materials_bin(file);
  }

  static void set_param(eastl::hash_map<string_hash, Param>& storage, const string& name, const Param& p)
  {
    storage.insert_or_assign(
      str_hash(name.c_str()),
      p
    );
  }

  void set_extern(const string& name, const Param& p)
  {
    set_param(externs_storage, name, p);
  }

  void set_channel(const string& name, const Param& p)
  {
    set_param(channels_storage, name, p);
  }

  void activate_scope(string_view name, gapi::CmdEncoder& cmd_encoder)
  {
    const string_hash h = str_hash(name);
    const auto it = scopes_storage.find(h);

    ASSERT_FMT(it != scopes_storage.end(), "Missing scope: `{}`:{}\n{}", name,h, dump_storage_table(scopes_storage, "scopes_storage"));

    Processor pr;
    pr.activateScope(it->second, &cmd_encoder);
  }

  void activate_technique(string_view name, gapi::CmdEncoder& cmd_encoder)
  {
    const string_hash h = str_hash(name);
    const auto it = techniques_storage.find(h);

    ASSERT_FMT(it != techniques_storage.end(), "Missing material `{}`", name);

    Processor pr;
    pr.activateTechnique(it->second, &cmd_encoder);
  }

  auto calc_group_count(const string_view cs_technique_name, const uint3 threads_count) -> uint3
  {
    const string_hash h = str_hash(cs_technique_name);
    const auto it = techniques_storage.find(h);

    ASSERT_FMT(it != techniques_storage.end(), "Missing material `{}`", cs_technique_name);
    Technique& t = it->second;

    uint3 groupSize{0,0,0};
    if (const auto* desc = std::get_if<gapi::ComputePipelineDescription>(&t.pipelineDesc))
    {
      ASSERT(desc->numthreads != (uint3{0,0,0}));
      groupSize = desc->numthreads;
    }

    const auto calcDim = [](const uint dim, const uint div) -> uint
    {
      const uint add = dim % div > 0 ? 1 : 0;
      return dim / div + add;
    };

    const uint3 groupCount = uint3{
      calcDim(threads_count.x, groupSize.x),
      calcDim(threads_count.y, groupSize.y),
      calcDim(threads_count.z, groupSize.z),
    };
    
    return groupCount;
  }
}
