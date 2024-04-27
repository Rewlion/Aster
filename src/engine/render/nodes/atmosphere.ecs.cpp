#include <engine/components/atmosphere.h>
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

ECS_DESCRIBE_QUERY(query_atm_tr_lut, (const gapi::TextureWrapper& atm_tr_lut));
ECS_COMP_GETTER(AtmosphereComponent, atmosphere);
ECS_DESCRIBE_QUERY(query_sun_params, (float sun_azimuth, float sun_altitude));
ECS_DESCRIBE_QUERY(query_atm_lut_state, (int& atm_lut_state));

enum class AtmosphereLutState : int
{
  Preparing = 0,
  Ready = 1
};

ECS_EVENT_SYSTEM()
static void atmosphere_creation_handler(const ecs::OnEntityCreated& evt,
                                        const AtmosphereComponent& atmosphere)
{
  ecs::EntityComponents init;
  init["atm_lut_state"] = (int)AtmosphereLutState::Preparing;
  ecs::get_registry().createEntity("AtmosphereRender", std::move(init));
}

constexpr
auto get_envi_specular_mips() -> uint
{
  return std::min(std::bit_width((uint)ENVI_SPECULAR_LUT_SIZE.x), 5u);
}

NODE_BEGIN(atm_res_import)
  ORDER_ME_AFTER(frame_preparing)
  READ_BLOB(camera_data, Engine::CameraData)
  
  CREATE_TEX_2D_PERSISTENT(atm_tr_lut, TEX_SIZE2(TR_LUT_SIZE),
                           R16G16B16A16_SFLOAT, TEX_USAGE2(RT,SRV), TEX_STATE(RenderTarget))
  
  CREATE_TEX_2D_PERSISTENT(atm_ms_lut, TEX_SIZE2(MS_LUT_SIZE),
                           R16G16B16A16_SFLOAT, TEX_USAGE2(RT,SRV), TEX_STATE(RenderTarget))
  
  CREATE_TEX_2D_PERSISTENT(atm_sky_lut, TEX_SIZE2(SKY_LUT_SIZE),
                           R16G16B16A16_SFLOAT, TEX_USAGE2(RT,SRV), TEX_STATE(RenderTarget))

  CREATE_TEX_2D_PERSISTENT(atm_ap_lut, TEX_SIZE(AP_LUT_SIZE.x, AP_LUT_SIZE.y, AP_LUT_SIZE.z),
                           R16G16B16A16_SFLOAT, TEX_USAGE2(SRV,UAV), TEX_STATE(ShaderReadWrite))

  CREATE_TEX_2D_EX(atm_envi_specular, TEX_SIZE2(ENVI_SPECULAR_LUT_SIZE),
                  R16G16B16A16_SFLOAT, TEX_MIPS(get_envi_specular_mips()), TEX_USAGE2(RT,SRV), TEX_STATE(RenderTarget),
                  TEX_PERSISTENT)

  CREATE_TEX_2D_PERSISTENT(atm_envi_brdf, TEX_SIZE2(ENVI_BRDF_SIZE),
                           R16G16B16A16_SFLOAT, TEX_USAGE2(RT,SRV), TEX_STATE(RenderTarget))

  CREATE_BLOB(atm_envi_mips, int)
  CREATE_BLOB(sun_azimuth_altitude, float2)

  EXEC(atm_res_import_exec)
NODE_END()

NODE_EXEC()
static
void atm_res_import_exec(gapi::CmdEncoder& encoder,
                         int& atm_envi_mips,
                         const Engine::CameraData& camera_data,
                         float2& sun_azimuth_altitude)
{
  atm_envi_mips = get_envi_specular_mips();

  const AtmosphereComponent* atmosphere = get_atmosphere();
  if (!atmosphere)
    return;

  query_sun_params([&](float sun_azimuth, float sun_altitude){
    sun_azimuth_altitude = float2(sun_azimuth, sun_altitude);

    float4 sunAzimuth_sunAltitude_rTopMM_rBotMM{
      math::radians(sun_azimuth),
      math::radians(sun_altitude),
      atmosphere->getTopRadiusKm() / 1000.0,
      atmosphere->getBotRadiusKm() / 1000.0
    };

    float cameraH = sunAzimuth_sunAltitude_rTopMM_rBotMM.w + camera_data.pos.y;
    cameraH = (cameraH < 10.0 ? 10.0 : cameraH) / 1e6;

    tfx::set_extern("atmPosMM", float3(0.0f, sunAzimuth_sunAltitude_rTopMM_rBotMM.w + cameraH, 0.0f));
    tfx::set_extern("maxAerialDist_mm", 0.032f);
    tfx::set_channel("sunAzimuth_sunAltitude_rTopMM_rBotMM", sunAzimuth_sunAltitude_rTopMM_rBotMM);
    tfx::activate_scope("AtmosphereScope", encoder);
  });
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

ECS_DESCRIBE_QUERY(query_sun, (float& sun_azimuth, float& sun_altitude));

static bool show_wnd = false;
void imgui_draw_sun_params()
{
  if (!show_wnd)
    return;

  float altitude = 0.0;
  float azimuth = 0.0;
  query_sun([&](float& _azimuth, float& _altitude){
    azimuth = _azimuth;
    altitude = _altitude;
  });

  ImGui::Begin("sun", nullptr, 0);
  ImGui::SliderFloat("altitude", &altitude, -90.0, 90.0);
  ImGui::SliderFloat("azimuth", &azimuth, 0.0, 360.0);
  ImGui::End();

  query_sun([&](float& _azimuth, float& _altitude){
    _azimuth = azimuth;
    _altitude = altitude;
  });
}

static
void fg_show(eastl::span<string_view>)
{
  show_wnd = !show_wnd;
}

CONSOLE_CMD("sun.edit", 0, 0, fg_show);
IMGUI_REG_WND(imgui_draw_sun_params);