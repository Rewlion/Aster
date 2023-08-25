#include <engine/console/cmd.h>
#include <engine/ecs/ecs.h>
#include <engine/ecs/ecs_events.h>
#include <engine/ecs/macros.h>
#include <engine/gapi/gapi.h>
#include <engine/gapi/resource_wrappers.h>
#include <engine/log.h>
#include <engine/math.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/imgui/imgui.h>
#include <engine/render/ecs_utils.h>
#include <engine/tfx/tfx.h>
#include <engine/work_cycle/camera.h>

#include <engine/shaders/shaders/atmosphere/consts.hlsl>
#include <engine/shaders/shaders/atmosphere/sph.hlsl>

#include <bit>

ECS_QUERY()
static
void query_atm_tr_lut(eastl::function<void(const gapi::TextureWrapper& atm_tr_lut)>);

ECS_QUERY()
static
void query_atm_params(eastl::function<void(float atm_radius_bot_km, float atm_radius_top_km)>);

ECS_QUERY()
static
void query_sun_params(eastl::function<void(float sun_azimuth, float sun_altitude)>);

ECS_QUERY()
static
void query_atm_lut_state(eastl::function<void(int& atm_lut_state)>);

enum class AtmosphereLutState : int
{
  Preparing = 0,
  Ready = 1
};

ECS_EVENT_SYSTEM()
static void atmosphere_creation_handler(const ecs::OnEntityCreated& evt, float atm_radius_bot_km, float atm_radius_top_km)
{
  gapi::TextureAllocationDescription allocDesc{
    .format = gapi::TextureFormat::R16G16B16A16_SFLOAT,
    .extent = uint3{TR_LUT_SIZE, 1},
    .mipLevels = 1,
    .arrayLayers = 1,
    .usage = gapi::TEX_USAGE_RT | gapi::TEX_USAGE_SRV,
    .name = "trLUT"
  };
  gapi::TextureHandle trLUT = gapi::allocate_texture(allocDesc);

  allocDesc.extent = uint3{SKY_LUT_SIZE, 1};
  allocDesc.name = "skyLUT";
  gapi::TextureHandle skyLUT = gapi::allocate_texture(allocDesc);

  allocDesc.extent = uint3{MS_LUT_SIZE, 1};
  allocDesc.name = "msLUT";
  gapi::TextureHandle msLUT = gapi::allocate_texture(allocDesc);

  allocDesc.extent = uint3{256, 256, 1};
  allocDesc.name = "enviBRDF";
  gapi::TextureHandle enviBRDF = gapi::allocate_texture(allocDesc);

  const int2 enviSpecularSize = int2(512,256);
  int enviMips = std::min(std::bit_width((uint)enviSpecularSize.x), 5u);
  allocDesc.extent = uint3{enviSpecularSize, 1};
  allocDesc.mipLevels = enviMips;
  allocDesc.name = "enviSpecularLUT";
  gapi::TextureHandle enviSpecularLUT = gapi::allocate_texture(allocDesc);
  allocDesc.mipLevels = 1;

  allocDesc.extent = uint3{AP_LUT_SIZE};
  allocDesc.usage = gapi::TEX_USAGE_SRV | gapi::TEX_USAGE_UAV;
  allocDesc.name = "apLUT";
  gapi::TextureHandle apLUT = gapi::allocate_texture(allocDesc);

  std::unique_ptr<gapi::CmdEncoder> encoder{gapi::allocate_cmd_encoder()};
  encoder->transitTextureState(trLUT, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
  encoder->transitTextureState(msLUT, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
  encoder->transitTextureState(skyLUT, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
  encoder->transitTextureState(apLUT, gapi::TextureState::Undefined, gapi::TextureState::ShaderReadWrite);
  encoder->transitTextureState(enviSpecularLUT, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
  encoder->transitTextureState(enviBRDF, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
  encoder->flush();

  ecs::EntityComponents init;
  init["atm_tr_lut"] = gapi::TextureWrapper{trLUT};
  init["atm_ms_lut"] = gapi::TextureWrapper{msLUT};
  init["atm_sky_lut"] = gapi::TextureWrapper{skyLUT};
  init["atm_ap_lut"] = gapi::TextureWrapper{apLUT};
  init["atm_envi_specular"] = gapi::TextureWrapper{enviSpecularLUT};
  init["atm_envi_brdf"] = gapi::TextureWrapper{enviBRDF};
  init["atm_envi_mips"] = enviMips;
  init["atm_lut_state"] = (int)AtmosphereLutState::Preparing;
  ecs::get_registry().createEntity("AtmosphereRender", std::move(init));
}

gapi::TextureState get_lut_init_state(const gapi::TextureState init_state)
{
  AtmosphereLutState lutState;
  query_atm_lut_state([&](const int& atm_lut_state){
    lutState = (AtmosphereLutState)atm_lut_state;
  });

  return lutState == AtmosphereLutState::Preparing ?
          init_state :
          gapi::TextureState::ShaderRead;
}

ECS_EVENT_SYSTEM()
static void atmosphere_render_creation_handler(
  const ecs::OnEntityCreated& evt,
  const gapi::TextureWrapper& atm_tr_lut,
  const gapi::TextureWrapper& atm_sky_lut,
  const gapi::TextureWrapper& atm_ms_lut,
  const gapi::TextureWrapper& atm_ap_lut,
  const gapi::TextureWrapper& atm_envi_specular,
  const gapi::TextureWrapper& atm_envi_brdf,
  const int atm_envi_mips)
{
  fg::register_node("atm_res_import", FG_FILE_DECL,
    [&atm_tr_lut, &atm_ms_lut, &atm_sky_lut,
     &atm_ap_lut, &atm_envi_specular, &atm_envi_brdf, atm_envi_mips](fg::Registry& reg)
  {
    reg.orderMeAfter("frame_preparing");

    auto cameraData = reg.readBlob<Engine::CameraData>("camera_data");

    auto trLutTex = reg.importTextureProducer("atm_tr_lut", [&atm_tr_lut](){
      return fg::TextureImport{
        .tex = atm_tr_lut,
        .initState = get_lut_init_state(gapi::TextureState::RenderTarget)
      };
    });

    auto msLutTex = reg.importTextureProducer("atm_ms_lut", [&atm_ms_lut](){
      return fg::TextureImport{
        .tex = atm_ms_lut,
        .initState = get_lut_init_state(gapi::TextureState::RenderTarget)
      };
    });

    auto skyLutTex = reg.importTextureProducer("atm_sky_lut", [&atm_sky_lut](){
      return fg::TextureImport{
        .tex = atm_sky_lut,
        .initState = get_lut_init_state(gapi::TextureState::RenderTarget)
      };
    });

    auto apLutTex = reg.importTextureProducer("atm_ap_lut", [&atm_ap_lut](){
      return fg::TextureImport{
        .tex = atm_ap_lut,
        .initState = get_lut_init_state(gapi::TextureState::ShaderReadWrite)
      };
    });

    auto enviSpecularTex = reg.importTextureProducer("atm_envi_specular", [&atm_envi_specular](){
      return fg::TextureImport{
        .tex = atm_envi_specular,
        .initState = get_lut_init_state(gapi::TextureState::RenderTarget)
      };
    });

    auto enviBRDFTex = reg.importTextureProducer("atm_envi_brdf", [&atm_envi_brdf](){
      return fg::TextureImport{
        .tex = atm_envi_brdf,
        .initState = get_lut_init_state(gapi::TextureState::RenderTarget)
      };
    });

    auto enviMips = reg.createBlob<int>("atm_envi_mips");

    return [cameraData, enviMips, atm_envi_mips](gapi::CmdEncoder& encoder)
    {
      enviMips.get() = atm_envi_mips;

      query_atm_params([&](float atm_radius_bot_km, float atm_radius_top_km){
      query_sun_params([&](float sun_azimuth, float sun_altitude){
        float4 sunAzimuth_sunAltitude_rTopMM_rBotMM{
          math::radians(sun_azimuth),
          math::radians(sun_altitude),
          atm_radius_top_km / 1000.0,
          atm_radius_bot_km / 1000.0
        };

        float cameraH = sunAzimuth_sunAltitude_rTopMM_rBotMM.w + cameraData->pos.y;
        cameraH = (cameraH < 10.0 ? 10.0 : cameraH) / 1e6;

        tfx::set_extern("atmPosMM", float3(0.0f, sunAzimuth_sunAltitude_rTopMM_rBotMM.w + cameraH, 0.0f));
        tfx::set_extern("maxAerialDist_mm", 0.032f);
        tfx::set_channel("sunAzimuth_sunAltitude_rTopMM_rBotMM", sunAzimuth_sunAltitude_rTopMM_rBotMM);
        tfx::activate_scope("AtmosphereScope", encoder);
      });});
    };
  });

  fg::register_node("atm_ap_lut_render", FG_FILE_DECL, [](fg::Registry& reg)
  {
    auto trLUT = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead);
    auto msLUT = reg.readTexture("atm_ms_lut", gapi::TextureState::ShaderRead);
    auto apLUT = reg.modifyTexture("atm_ap_lut", gapi::TextureState::ShaderReadWrite);
    return [trLUT, msLUT, apLUT](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUT.get());
      tfx::set_extern("msLUT", msLUT.get());
      tfx::set_extern("apLUT", apLUT.get());
      tfx::activate_technique("AerialPerspective", encoder);

      encoder.updateResources();
      encoder.dispatch(1,1,32);
    };
  });

  fg::register_node("atm_tr_lut_render", FG_FILE_DECL, [&atm_tr_lut](fg::Registry& reg)
  {
    reg.orderMeAfter("frame_preparing");

    reg.requestRenderPass()
       .addTarget("atm_tr_lut", gapi::LoadOp::DontCare);

    return [](gapi::CmdEncoder& encoder)
    {
      tfx::activate_technique("TransmittanceLUT", encoder);

      encoder.updateResources();
      encoder.draw(4, 1, 0, 0);
    };
  });

  fg::register_node("atm_ms_lut_render", FG_FILE_DECL, [&atm_ms_lut](fg::Registry& reg)
  {
    auto trLUTtex = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead);

    reg.requestRenderPass()
       .addTarget("atm_ms_lut", gapi::LoadOp::DontCare);

    return [trLUTtex](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUTtex.get());
      tfx::activate_technique("MultipleScatteringLUT", encoder);

      encoder.updateResources();
      encoder.draw(4, 1, 0, 0);
    };
  });

  fg::register_node("atm_sky_lut_render", FG_FILE_DECL, [&atm_sky_lut](fg::Registry& reg)
  {
    auto trLUTtex = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead);
    auto msLUTtex = reg.readTexture("atm_ms_lut", gapi::TextureState::ShaderRead);

    reg.requestRenderPass()
       .addTarget("atm_sky_lut", gapi::LoadOp::DontCare);

    return [trLUTtex, msLUTtex](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUTtex.get());
      tfx::set_extern("msLUT", msLUTtex.get());
      tfx::activate_technique("SkyLUT", encoder);

      encoder.updateResources();
      encoder.draw(4, 1, 0, 0);
    };
  });

  fg::register_node("atm_sph_render", FG_FILE_DECL, [](fg::Registry& reg)
  {
    auto paramsBuf = reg.modifyBuffer("sph_buf", gapi::BufferState::BF_STATE_UAV_RW);
    auto trLUTtex = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead);
    auto skyLUTtex = reg.readTexture("atm_sky_lut", gapi::TextureState::ShaderRead);

    return [paramsBuf, trLUTtex, skyLUTtex](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUTtex.get());
      tfx::set_extern("skyLUT", skyLUTtex.get());
      tfx::set_extern("atmParamsBuffer", paramsBuf.get());
      tfx::activate_technique("AtmSphericalHarmonics", encoder);

      encoder.updateResources();
      encoder.dispatch(1,1,1);
    };
  });

  fg::register_node("atm_sky_apply", FG_FILE_DECL, [](fg::Registry& reg)
  {
    reg.requestRenderPass()
      .addTarget("resolve_target");

    auto apLUTtex = reg.readTexture("atm_ap_lut", gapi::TextureState::ShaderRead);
    auto skyLUTtex = reg.readTexture("atm_sky_lut", gapi::TextureState::ShaderRead);
    auto trLUTtex = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead);
    auto gbufferDepth = reg.readTexture("gbuffer_depth", gapi::TextureState::ShaderRead);

    return [apLUTtex, skyLUTtex, trLUTtex, gbufferDepth](gapi::CmdEncoder& encoder)
    {
       tfx::set_extern("apLUT", apLUTtex.get());
       tfx::set_extern("skyLUT", skyLUTtex.get());
       tfx::set_extern("trLUT", trLUTtex.get());
       tfx::set_extern("gbufferDepth", gbufferDepth.get());

       tfx::activate_technique("SkyApply", encoder);
       encoder.updateResources();
       encoder.draw(4, 1, 0, 0);
    };
  });

  fg::register_node("atm_envi_specular", FG_FILE_DECL, [&atm_sky_lut](fg::Registry& reg){
    auto trLUTtex = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead);
    auto skyLUTtex = reg.readTexture("atm_sky_lut", gapi::TextureState::ShaderRead);
    auto enviSpecularTex = reg.modifyTexture("atm_envi_specular", gapi::TextureState::RenderTarget);
    auto enviMips = reg.readBlob<int>("atm_envi_mips");

    return [trLUTtex, skyLUTtex, enviSpecularTex, enviMips](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUTtex.get());
      tfx::set_extern("skyLUT", skyLUTtex.get());
      tfx::set_extern("enviMips", (float)enviMips.get());

      int mips = enviMips.get();
      for (int i = 0; i < mips; ++i)
      {
        gapi::RenderTargets mrt {gapi::RenderPassAttachment{
          .texture = enviSpecularTex.get(),
          .mipLevel = (uint32_t)i,
          .loadOp = gapi::LoadOp::DontCare
        }};
      
        encoder.beginRenderpass(mrt, {});

        float roughness = 1.0f / (float)mips * i;
        tfx::set_extern("enviRoughness", roughness);
        tfx::set_extern("renderSize", encoder.getRenderSize());

        tfx::activate_technique("EnviSpecular", encoder);
        encoder.updateResources();
        encoder.draw(4, 1, 0, 0);

        encoder.endRenderpass();
      }
    };
  });

  fg::register_node("atm_envi_brdf", FG_FILE_DECL, [&atm_envi_brdf](fg::Registry& reg){
    reg.requestRenderPass()
       .addTarget("atm_envi_brdf", gapi::LoadOp::DontCare);

    return [](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("renderSize", encoder.getRenderSize());
      tfx::activate_technique("EnviBRDF", encoder);
      encoder.updateResources();
      encoder.draw(4, 1, 0, 0);
    };
  });

  fg::register_node("atm_sky_cycle", FG_FILE_DECL, [&atm_sky_lut](fg::Registry& reg){
    reg.orderMeAfter("atm_sky_apply");
    reg.orderMeBefore("transparent_sync");

    auto trLUTtex = reg.readTexture("atm_tr_lut", gapi::TextureState::RenderTarget);
    auto msLUTtex = reg.readTexture("atm_ms_lut", gapi::TextureState::RenderTarget);
    auto skyLUTtex = reg.readTexture("atm_sky_lut", gapi::TextureState::RenderTarget);
    auto enviSpecularTex = reg.readTexture("atm_envi_specular", gapi::TextureState::RenderTarget);
    auto enviBRDFTex = reg.readTexture("atm_envi_brdf", gapi::TextureState::RenderTarget);
    auto apLUTtex = reg.readTexture("atm_ap_lut", gapi::TextureState::ShaderReadWrite);

    return [](gapi::CmdEncoder& encoder) {};
  });
}

ECS_QUERY()
static
void query_sun(eastl::function<void(float& sun_azimuth, float& sun_altitude)>);

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