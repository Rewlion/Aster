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
      const ShadersSystem::ByteCodes& byteCode;
      eastl::vector<uint8_t> cpuCbuffer;
      gapi::BufferHandler gpuCbuffer;
    };

    struct Technique
    {
      const ShadersSystem::ByteCodes& byteCode;
      gapi::GraphicsPipelineDescription graphicsPipelineDesc;
    };

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
            return std::holds_alternative<gapi::TextureHandler>(res) && (type == ShadersSystem::ResourceType::Texture2D);
          };

          const auto& [resource, resourceExist] = getParam(bc.accessType, bc.resourceName);
          if (resourceExist && isValidType(resource, bc.type))
          {
            switch (bc.type)
            {
              case ShadersSystem::ResourceType::Texture2D:
              {
                const auto h = std::get<gapi::TextureHandler>(resource);
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
            case ShadersSystem::ResourceType::Texture2D:
            {
              m_CmdEncoder->bindTexture((gapi::TextureHandler)h, bc.dset, bc.binding);
              break;
            }
          }
        }

        void processByteCode(const ShadersSystem::ShBeginCbuffer& bc)
        {
        }

        void processByteCode(const ShadersSystem::ShEndCbuffer& bc)
        {
          gapi::write_buffer(m_Scope->gpuCbuffer, m_Scope->cpuCbuffer.data(), 0,
                             m_Scope->cpuCbuffer.size(), gapi::WR_DISCARD);
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

      log("loaded scope `{}`", matScope.name);
    }
  }

  static void load_techniques(const eastl::vector<ShadersSystem::MaterialsBin::Technique>& mat_techniques)
  {
    for (const auto& t: mat_techniques)
    {
      const string_hash h = str_hash(t.name.c_str());
      ASSERT(techniques_storage.find(h) == techniques_storage.end());
      eastl::vector<gapi::ShaderModuleHandler> modules;
      modules.reserve(t.blobs.size());
      for (size_t i = 0; i < t.blobs.size(); ++i)
      {
        const gapi::ShaderModuleHandler mh = gapi::add_module((void*)&t.blobs[i], (void*)&t.reflections[i]);
        modules.push_back(mh);
      }

      techniques_storage.insert({
        h,
        Technique{
          .byteCode = t.byteCode,
          .graphicsPipelineDesc = {
            .shaders = std::move(modules),
            .ia = t.renderState.ia,
            .topology = t.renderState.topology,
            .depthStencilState = t.renderState.depthStencil,
            .blendState = t.renderState.blending
          }
        }
      });

      log("loaded technique `{}`", t.name);
    }
  }

  void load_materials_bin(const string& file)
  {
    log("loading materials bin {}", file);
    std::ifstream from(file, std::ios::binary);
    if (!from.is_open())
    {
      ASSERT(!"failed to load materials binary");
      return;
    }

    mat_bin = ShadersSystem::MaterialsBin{};
    boost::archive::binary_iarchive archive(from);
    archive & mat_bin;

    log("loading tfx scopes");
    load_scopes(mat_bin.scopes);
    log("loading tfx techniques");
    load_techniques(mat_bin.techniques);
    log("successfuly loaded materials bin");
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

  void activate_scope(const string& name, gapi::CmdEncoder* cmd_encoder)
  {
    const string_hash h = str_hash(name.c_str());
    const auto it = scopes_storage.find(h);
    if (it != scopes_storage.end())
    {
      Processor pr;
      pr.activateScope(it->second, cmd_encoder);
    }
  }

  void activate_technique(const string& name, gapi::CmdEncoder* cmd_encoder)
  {
    const string_hash h = str_hash(name.c_str());
    const auto it = techniques_storage.find(h);
    if (it != techniques_storage.end())
    {
      Processor pr;
      pr.activateTechnique(it->second, cmd_encoder);
    }
  }
}
