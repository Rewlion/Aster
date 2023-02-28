#include "tfx.h"

#include <engine/algorithm/hash.h>
#include <engine/log.h>
#include <engine/gapi/gapi.h>
#include <engine/gapi/cmd_encoder.h>
#include <shaders_compiler/bin.h>
#include <shaders_compiler/serialization.h>

#include <boost/archive/binary_iarchive.hpp>
#include <EASTL/hash_map.h>

#include <fstream>
#include <variant>

namespace tfx
{
  namespace
  {
    struct Scope
    {
      string name;
      const ShadersSystem::ByteCodes& byteCode;
      eastl::vector<uint8_t> cpuCbuffer;
      gapi::BufferHandler gpuCbuffer;
    };

    struct Technique
    {
      string name;
      const ShadersSystem::ByteCodes& byteCode;
      gapi::GraphicsPipelineDescription graphicsPipelineDesc;
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
          m_CmdEncoder->bindGraphicsPipeline(technique.graphicsPipelineDesc);
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

        gapi::ResourceHandler getResource(const ShadersSystem::ShBindResource& bc)
        {
          const auto isValidType = [](const auto& res, const ShadersSystem::ResourceType type) {
            return std::holds_alternative<gapi::TextureHandle>(res) && (type == ShadersSystem::ResourceType::Texture2D) ||
                   std::holds_alternative<gapi::TextureHandle>(res) && (type == ShadersSystem::ResourceType::TextureCube) ||
                   std::holds_alternative<gapi::SamplerHandler>(res) && (type == ShadersSystem::ResourceType::Sampler);
          };

          const auto& [resource, resourceExist] = getParam(bc.accessType, bc.resourceName);
          if (resourceExist && isValidType(resource, bc.type))
          {
            switch (bc.type)
            {
              case ShadersSystem::ResourceType::Sampler:
              {
                const auto h = std::get<gapi::SamplerHandler>(resource);
                return (gapi::ResourceHandler)h;
              }
              case ShadersSystem::ResourceType::Texture2D:
              case ShadersSystem::ResourceType::TextureCube:
              {
                const auto h = std::get<gapi::TextureHandle>(resource);
                return (gapi::ResourceHandler)h;
              }
              default:
              {
                ASSERT(!"unsupported");
                break;
              }
            }
          }

          return gapi::INVALID_RESOURCE_HANDLER;
        }

        void processByteCode(const ShadersSystem::ShBindResource& bc)
        {
          gapi::ResourceHandler h = getResource(bc);
          switch (bc.type)
          {
            case ShadersSystem::ResourceType::Sampler:
            {
              m_CmdEncoder->bindSampler((gapi::SamplerHandler)h, bc.dset, bc.binding);
              break;
            }
            case ShadersSystem::ResourceType::Texture2D:
            case ShadersSystem::ResourceType::TextureCube:
            {
              m_CmdEncoder->bindTexture((gapi::TextureHandle)h, bc.dset, bc.binding);
              break;
            }
            default:
            {
              ASSERT_FMT(false, "unsupported byte code `{}`", bc.type);
              break;
            }
          }
        }

        void processByteCode(const ShadersSystem::ShBeginCbuffer& bc)
        {
        }

        void processByteCode(const ShadersSystem::ShEndCbuffer& bc)
        {
          void* dstBuffer = gapi::map_buffer(m_Scope->gpuCbuffer, 0, m_Scope->cpuCbuffer.size(), gapi::WR_DISCARD);
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

      loginfo("loaded scope `{}`:{}", matScope.name, h);
    }
  }

  static void load_techniques(const eastl::vector<ShadersSystem::MaterialsBin::Technique>& mat_techniques)
  {
    for (const auto& t: mat_techniques)
    {
      const string_hash h = str_hash(t.name.c_str());
      ASSERT_FMT(techniques_storage.find(h) == techniques_storage.end(), "technique {} already exist", t.name);

      eastl::vector<gapi::ShaderModuleHandler> modules;
      modules.reserve(t.blobs.size());
      for (size_t i = 0; i < t.blobs.size(); ++i)
      {
        const gapi::ShaderModuleHandler mh = gapi::add_module((void*)&t.blobs[i]);
        modules.push_back(mh);
      }

      gapi::PipelineLayoutHandler layout = gapi::add_pipeline_layout((void*)&t.dsets);

      techniques_storage.insert({
        h,
        Technique{
          .name = t.name,
          .byteCode = t.byteCode,
          .graphicsPipelineDesc = {
            .layout = layout,
            .shaders = std::move(modules),
            .ia = t.renderState.ia,
            .cullMode = t.renderState.cullMode,
            .topology = t.renderState.topology,
            .depthStencilState = t.renderState.depthStencil,
            .blendState = t.renderState.blending
          }
        }
      });

      loginfo("loaded technique `{}`", t.name);
    }
  }

  void load_materials_bin(const string& file)
  {
    loginfo("loading materials bin {}", file);
    std::ifstream from(file, std::ios::binary);
    if (!from.is_open())
    {
      ASSERT(!"failed to load materials binary");
      return;
    }

    mat_bin = ShadersSystem::MaterialsBin{};
    boost::archive::binary_iarchive archive(from);
    archive & mat_bin;

    loginfo("loading tfx scopes");
    load_scopes(mat_bin.scopes);
    loginfo("loading tfx techniques");
    load_techniques(mat_bin.techniques);
    loginfo("successfuly loaded materials bin");
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
}
