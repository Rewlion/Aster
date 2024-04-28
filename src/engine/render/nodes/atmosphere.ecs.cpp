#include <engine/components/atmosphere.h>
#include <engine/components/sun.h>
#include <engine/console/cmd.h>
#include <engine/ecs/ecs.h>
#include <engine/gapi/gapi.h>
#include <engine/gapi/resource_wrappers.h>
#include <engine/log.h>
#include <engine/math.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/render/imgui/imgui.h>
#include <engine/tfx/tfx.h>
#include <engine/work_cycle/camera.h>

#include <engine/shaders/shaders/atmosphere/consts.hlsl>
#include <engine/shaders/shaders/atmosphere/sph.hlsl>

#include <bit>

enum class AtmosphereLutState : int
{
  Preparing = 0,
  Ready = 1
};

auto get_envi_specular_mips() -> uint
{
  return std::min(std::bit_width((uint)ENVI_SPECULAR_LUT_SIZE.x), 5u);
}

class AtmosphereRenderState
{
  public:
    AtmosphereRenderState() = default;
    AtmosphereRenderState(const ed::Scope* init)
    {
    }

    void init()
    {
      m_LutState = AtmosphereLutState::Preparing;

      m_TrLut = {
        gapi::allocate_texture(
          gapi::TextureAllocationDescription{
            .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
            .extent =          uint3(TR_LUT_SIZE, 1),
            .mipLevels =       1,
            .arrayLayers =     1,
            .samplesPerPixel = gapi::TextureSamples::s1,
            .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV),
            .name =            "atm_tr_lut"
          }
        ),
        gapi::TextureState::Undefined
      };

      m_MsLut = {
        gapi::allocate_texture(
          gapi::TextureAllocationDescription{
            .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
            .extent =          uint3(MS_LUT_SIZE, 1),
            .mipLevels =       1,
            .arrayLayers =     1,
            .samplesPerPixel = gapi::TextureSamples::s1,
            .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV),
            .name =             "atm_ms_lut"
          }
        ),
        gapi::TextureState::Undefined
      };

      m_SkyLut = {
        gapi::allocate_texture(
          gapi::TextureAllocationDescription{
            .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
            .extent =          uint3(SKY_LUT_SIZE, 1),
            .mipLevels =       1,
            .arrayLayers =     1,
            .samplesPerPixel = gapi::TextureSamples::s1,
            .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV),
            .name =            "atm_sky_lut"
          }
        ),
        gapi::TextureState::Undefined
      };

      m_ApLut = {
        gapi::allocate_texture(
          gapi::TextureAllocationDescription{
            .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
            .extent =          uint3(AP_LUT_SIZE),
            .mipLevels =       1,
            .arrayLayers =     1,
            .samplesPerPixel = gapi::TextureSamples::s1,
            .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_UAV)
          }
        ),
        gapi::TextureState::Undefined
      };

      const uint enviMips = get_envi_specular_mips();
      m_EnviBRDFLut = {
        gapi::allocate_texture(
          gapi::TextureAllocationDescription{
            .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
            .extent =          uint3(ENVI_BRDF_SIZE, 1),
            .mipLevels =       enviMips,
            .arrayLayers =     1,
            .samplesPerPixel = gapi::TextureSamples::s1,
            .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
          }
        ),
        gapi::TextureState::Undefined
      };

      m_EnviSpecular = {
        gapi::allocate_texture(
          gapi::TextureAllocationDescription{
            .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
            .extent =          uint3(ENVI_SPECULAR_LUT_SIZE, 1),
            .mipLevels =       enviMips,
            .arrayLayers =     1,
            .samplesPerPixel = gapi::TextureSamples::s1,
            .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
          }
        ),
        gapi::TextureState::Undefined
      };
    }

    auto getTrLut() -> gapi::TextureViewWithState* { return &m_TrLut; }
    auto getMsLut() -> gapi::TextureViewWithState* { return &m_MsLut; }
    auto getSkyLut() -> gapi::TextureViewWithState* { return &m_SkyLut; }
    auto getApLut() -> gapi::TextureViewWithState* { return &m_ApLut; }
    auto getEnviBRDFLut() -> gapi::TextureViewWithState* { return &m_EnviBRDFLut; }
    auto getEnviSpecular() -> gapi::TextureViewWithState* { return &m_EnviSpecular; }

  private:
    AtmosphereLutState m_LutState;
    gapi::UniqueTextureWithState m_TrLut;
    gapi::UniqueTextureWithState m_MsLut;
    gapi::UniqueTextureWithState m_SkyLut;
    gapi::UniqueTextureWithState m_ApLut;
    gapi::UniqueTextureWithState m_EnviBRDFLut;
    gapi::UniqueTextureWithState m_EnviSpecular;
};
DECLARE_INITABLE_ECS_OBJECT_COMPONENT(AtmosphereRenderState);

ECS_COMP_GETTER(AtmosphereRenderState, atmosphere_render_state);
ECS_COMP_GETTER(AtmosphereComponent, atmosphere);
ECS_COMP_GETTER(SunComponent, sun);

#define GET_ATM(res) [](){ return get_atmosphere_render_state()->get##res(); }

NODE_BEGIN(atm_resources)
  ORDER_ME_AFTER(frame_preparing)
  READ_BLOB(camera_data, Engine::CameraData)
  
  IMPORT_TEX(atm_tr_lut,        GET_ATM(TrLut))
  IMPORT_TEX(atm_ms_lut,        GET_ATM(MsLut))
  IMPORT_TEX(atm_sky_lut,       GET_ATM(SkyLut))
  IMPORT_TEX(atm_ap_lut,        GET_ATM(ApLut))
  IMPORT_TEX(atm_envi_specular, GET_ATM(EnviSpecular))
  IMPORT_TEX(atm_envi_brdf,     GET_ATM(EnviBRDFLut))

  CREATE_BLOB(atm_envi_mips, int)
  CREATE_BLOB(sun_azimuth_altitude, float2)

  EXEC(atm_resources_exec)
NODE_END()

NODE_EXEC()
static
void atm_resources_exec(gapi::CmdEncoder& encoder,
                         int& atm_envi_mips,
                         const Engine::CameraData& camera_data,
                         float2& sun_azimuth_altitude)
{
  atm_envi_mips = get_envi_specular_mips();

  const AtmosphereComponent* atmosphere = get_atmosphere();
  const SunComponent* sun = get_sun();
  if (!atmosphere || !sun)
    return;

  const float azimuth = sun->getAzimuth();
  const float altitude = sun->getAltitude();

  sun_azimuth_altitude = float2(azimuth, altitude);

  float4 sunAzimuth_sunAltitude_rTopMM_rBotMM{
    math::radians(azimuth),
    math::radians(altitude),
    atmosphere->getTopRadiusKm() / 1000.0,
    atmosphere->getBotRadiusKm() / 1000.0
  };

  float cameraH = sunAzimuth_sunAltitude_rTopMM_rBotMM.w + camera_data.pos.y;
  cameraH = (cameraH < 10.0 ? 10.0 : cameraH) / 1e6;

  tfx::set_extern("atmPosMM", float3(0.0f, sunAzimuth_sunAltitude_rTopMM_rBotMM.w + cameraH, 0.0f));
  tfx::set_extern("maxAerialDist_mm", 0.032f);
  tfx::set_channel("sunAzimuth_sunAltitude_rTopMM_rBotMM", sunAzimuth_sunAltitude_rTopMM_rBotMM);
  tfx::activate_scope("AtmosphereScope", encoder);
}

NODE_BEGIN(atm_ap_lut_render)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)
  BIND_TEX_SRV_AS(atm_ms_lut, msLUT)
  BIND_TEX_RW_UAV_AS(atm_ap_lut, apLUT)

  EXEC(atm_ap_lut_render_exec)
NODE_END()

NODE_EXEC()
static
void atm_ap_lut_render_exec(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("AerialPerspective", encoder);
  encoder.updateResources();
  encoder.dispatch(1,1,32);
}

NODE_BEGIN(atm_tr_lut_render)
  ORDER_ME_AFTER(frame_preparing)

  RP_BEGIN()
    TARGET_LOAD_DONTCARE(atm_tr_lut)
  RP_END()

  EXEC(atm_tr_lut_render_exec)
NODE_END()

NODE_EXEC()
static
void atm_tr_lut_render_exec(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("TransmittanceLUT", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}

NODE_BEGIN(atm_ms_lut_render)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)

  RP_BEGIN()
    TARGET_LOAD_DONTCARE(atm_ms_lut)
  RP_END()

  EXEC(atm_ms_lut_render_exec)
NODE_END()

NODE_EXEC()
static
void atm_ms_lut_render_exec(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("MultipleScatteringLUT", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}

NODE_BEGIN(atm_sky_lut_render)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)
  BIND_TEX_SRV_AS(atm_ms_lut, msLUT)

  RP_BEGIN()
    TARGET_LOAD_DONTCARE(atm_sky_lut)
  RP_END()

  EXEC(atm_sky_lut_render_exec)
NODE_END()

NODE_EXEC()
static
void atm_sky_lut_render_exec(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("SkyLUT", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}

NODE_BEGIN(atm_sph_render)
  BIND_BUF_RW_UAV_AS(sph_buf, atmParamsBuffer)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)
  BIND_TEX_SRV_AS(atm_sky_lut, skyLUT)

  EXEC(atm_sph_render_exec)
NODE_END()

NODE_EXEC()
static
void atm_sph_render_exec(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("AtmSphericalHarmonics", encoder);
  encoder.updateResources();
  encoder.dispatch(1,1,1);
}

NODE_BEGIN(atm_sky_apply)
  BIND_TEX_SRV_AS(atm_ap_lut, apLUT)
  BIND_TEX_SRV_AS(atm_sky_lut, skyLUT)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)
  BIND_TEX_RO_DEPTH_AS(gbuffer_depth, gbufferDepth)

  RP_BEGIN()
    TARGET(resolve_target)
  RP_END()

  EXEC(atm_sky_apply_exec)
NODE_END()

NODE_EXEC()
static
void atm_sky_apply_exec(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("SkyApply", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}

NODE_BEGIN(atm_envi_specular)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)
  BIND_TEX_SRV_AS(atm_sky_lut, skyLUT)

  MODIFY_TEX_RT(atm_envi_specular)
  READ_BLOB(atm_envi_mips, int)

  EXEC(atm_envi_specular_exec)
NODE_END()

NODE_EXEC()
static
void atm_envi_specular_exec(gapi::CmdEncoder& encoder,
                            const gapi::TextureHandle atm_envi_specular,
                            const int atm_envi_mips)
{
  tfx::set_extern("enviMips", (float)atm_envi_mips);

  for (int i = 0; i < atm_envi_mips; ++i)
  {
    gapi::RenderTargets mrt {gapi::RenderPassAttachment{
      .texture = atm_envi_specular,
      .mipLevel = (uint32_t)i,
      .loadOp = gapi::LoadOp::DontCare
    }};
  
    encoder.beginRenderpass(mrt, {});

    float roughness = 1.0f / (float)atm_envi_mips * i;
    tfx::set_extern("enviRoughness", roughness);
    tfx::set_extern("renderSize", encoder.getRenderSize());

    tfx::activate_technique("EnviSpecular", encoder);
    encoder.updateResources();
    encoder.draw(4, 1, 0, 0);

    encoder.endRenderpass();
  }
}

NODE_BEGIN(atm_envi_brdf)
  RP_BEGIN()
    TARGET_LOAD_DONTCARE(atm_envi_brdf)
  RP_END()
  EXEC(atm_envi_brdf_exec)
NODE_END()

NODE_EXEC()
static
void atm_envi_brdf_exec(gapi::CmdEncoder& encoder)
{
  tfx::set_extern("renderSize", encoder.getRenderSize());
  tfx::activate_technique("EnviBRDF", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}

NODE_BEGIN(atm_sync_out)
  ORDER_ME_AFTER(frame_preparing)

  RENAME_TEX(atm_envi_specular, atm_envi_specular_srv, TEX_STATE(ShaderRead))
  RENAME_TEX(atm_envi_brdf, atm_envi_brdf_srv, TEX_STATE(ShaderRead))
  RENAME_TEX(atm_tr_lut, atm_tr_lut_srv, TEX_STATE(ShaderRead))
  RENAME_TEX(atm_ms_lut, atm_ms_lut_srv, TEX_STATE(ShaderRead))
  RENAME_TEX(atm_sky_lut, atm_sky_lut_srv, TEX_STATE(ShaderRead))
  RENAME_TEX(atm_ap_lut, atm_ap_lut_srv, TEX_STATE(ShaderRead))

  NO_EXEC()
NODE_END()

static bool show_wnd = false;
void imgui_draw_sun_params()
{
  if (!show_wnd)
    return;

  float altitude = 0.0;
  float azimuth = 0.0;

  SunComponent* sun = get_sun();
  if (sun)
  {
    azimuth = sun->getAzimuth();
    altitude = sun->getAltitude();
  }

  ImGui::Begin("sun", nullptr, 0);
  ImGui::SliderFloat("altitude", &altitude, -90.0, 90.0);
  ImGui::SliderFloat("azimuth", &azimuth, 0.0, 360.0);
  ImGui::End();

  if (sun)
    sun->updateOrientation(azimuth, altitude);
}

static
void fg_show(eastl::span<string_view>)
{
  show_wnd = !show_wnd;
}

CONSOLE_CMD("sun.edit", 0, 0, fg_show);
IMGUI_REG_WND(imgui_draw_sun_params);