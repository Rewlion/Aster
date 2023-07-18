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
#include <engine/tfx/tfx.h>

#include <engine/shaders/shaders/atmosphere/consts.hlsl>

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
    .usage = gapi::TEX_USAGE_RT | gapi::TEX_USAGE_SRV
  };
  gapi::TextureHandle trLUT = gapi::allocate_texture(allocDesc);

  allocDesc.extent = uint3{SKY_LUT_SIZE, 1};
  gapi::TextureHandle skyLUT = gapi::allocate_texture(allocDesc);

  allocDesc.extent = uint3{MS_LUT_SIZE, 1};
  gapi::TextureHandle msLUT = gapi::allocate_texture(allocDesc);

  std::unique_ptr<gapi::CmdEncoder> encoder{gapi::allocate_cmd_encoder()};
  encoder->transitTextureState(trLUT, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
  encoder->transitTextureState(msLUT, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
  encoder->transitTextureState(skyLUT, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
  encoder->flush();

  ecs::EntityComponents init;
  init["atm_tr_lut"] = gapi::TextureWrapper{trLUT};
  init["atm_ms_lut"] = gapi::TextureWrapper{msLUT};
  init["atm_sky_lut"] = gapi::TextureWrapper{skyLUT};
  init["atm_lut_state"] = (int)AtmosphereLutState::Preparing;
  ecs::get_registry().createEntity("AtmosphereRender", std::move(init));
}
 
ECS_EVENT_SYSTEM()
static void atmosphere_render_creation_handler(
  const ecs::OnEntityCreated& evt,
  const gapi::TextureWrapper& atm_tr_lut,
  const gapi::TextureWrapper& atm_sky_lut,
  const gapi::TextureWrapper& atm_ms_lut)
{
  fg::register_node("atm_tr_lut_render", FG_FILE_DECL, [&atm_tr_lut](fg::Registry& reg)
  {
    reg.orderMeAfter("frame_preparing");

    auto trLutTex = reg.importTextureProducer("atm_tr_lut", [&atm_tr_lut](){
      AtmosphereLutState lutState;
      query_atm_lut_state([&](const int& atm_lut_state){
        lutState = (AtmosphereLutState)atm_lut_state;
      });

      return fg::TextureImport{
        .tex = atm_tr_lut,
        .initState = lutState == AtmosphereLutState::Preparing ?
                    gapi::TextureState::RenderTarget :
                    gapi::TextureState::ShaderRead
      };
    });

    reg.requestRenderPass()
       .addTarget(trLutTex, gapi::LoadOp::DontCare, gapi::StoreOp::Store);

    return [](gapi::CmdEncoder& encoder)
    {
      query_atm_params([&](float atm_radius_bot_km, float atm_radius_top_km){
      query_sun_params([&](float sun_azimuth, float sun_altitude){
        float2 atmRTopMM_atmRBotMM{atm_radius_top_km / 1000.0, atm_radius_bot_km / 1000.0};
        float2 sunAzimuth_Altitude{math::radians(sun_azimuth),math::radians(sun_altitude)};

        tfx::set_channel("atmRTopMM_atmRBotMM", atmRTopMM_atmRBotMM);
        tfx::set_channel("sunAzimuth_Altitude", sunAzimuth_Altitude);
        tfx::activate_scope("AtmosphereScope", encoder);
        tfx::activate_technique("TransmittanceLUT", encoder);

        encoder.updateResources();
        encoder.draw(4, 1, 0, 0);
      });});
    };
  });

  fg::register_node("atm_ms_lut_render", FG_FILE_DECL, [&atm_ms_lut](fg::Registry& reg)
  {
    auto msLutTex = reg.importTextureProducer("atm_ms_lut", [&atm_ms_lut](){
      AtmosphereLutState lutState;
      query_atm_lut_state([&](const int& atm_lut_state){
        lutState = (AtmosphereLutState)atm_lut_state;
      });

      return fg::TextureImport{
        .tex = atm_ms_lut,
        .initState = lutState == AtmosphereLutState::Preparing ?
                    gapi::TextureState::RenderTarget :
                    gapi::TextureState::ShaderRead
      };
    });

    reg.requestRenderPass()
       .addTarget(msLutTex, gapi::LoadOp::DontCare, gapi::StoreOp::Store);

    auto trLUTtex = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead);

    return [trLUTtex](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUTtex.get());
      tfx::activate_scope("MsLUTScope", encoder);
      tfx::activate_technique("MultipleScatteringLUT", encoder);

      encoder.updateResources();
      encoder.draw(4, 1, 0, 0);
    };
  });

  fg::register_node("atm_sky_lut_render", FG_FILE_DECL, [&atm_sky_lut](fg::Registry& reg)
  {
    auto skyLutTex = reg.importTextureProducer("atm_sky_lut", [&atm_sky_lut](){
      AtmosphereLutState lutState;
      query_atm_lut_state([&](const int& atm_lut_state){
        lutState = (AtmosphereLutState)atm_lut_state;
      });

      return fg::TextureImport{
        .tex = atm_sky_lut,
        .initState = lutState == AtmosphereLutState::Preparing ?
                    gapi::TextureState::RenderTarget :
                    gapi::TextureState::ShaderRead
      };
    });

    reg.requestRenderPass()
       .addTarget(skyLutTex, gapi::LoadOp::DontCare, gapi::StoreOp::Store);

    auto trLUTtex = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead);
    auto msLUTtex = reg.readTexture("atm_ms_lut", gapi::TextureState::ShaderRead);

    return [trLUTtex, msLUTtex](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUTtex.get());
      tfx::set_extern("msLUT", msLUTtex.get());
      tfx::activate_scope("SkyLUTScope", encoder);
      tfx::activate_technique("SkyLUT", encoder);

      encoder.updateResources();
      encoder.draw(4, 1, 0, 0);
    };
  });

  fg::register_node("atm_sky_apply", FG_FILE_DECL, [](fg::Registry& reg)
  {
    auto depth = reg.readTexture("gbuffer_depth", gapi::TextureState::DepthReadStencilRead);
    auto rt = reg.modifyTexture("resolve_target", gapi::TextureState::RenderTarget);

    reg.requestRenderPass()
      .addTarget(rt, gapi::LoadOp::Load, gapi::StoreOp::Store)
      .addDepth(depth, gapi::LoadOp::Load, gapi::StoreOp::Store,
                       gapi::LoadOp::Load, gapi::StoreOp::Store);

    auto skyLUTtex = reg.readTexture("atm_sky_lut", gapi::TextureState::ShaderRead);

    return [skyLUTtex](gapi::CmdEncoder& encoder)
    {
       tfx::set_extern("skyLUT", skyLUTtex.get());
       tfx::activate_scope("SkyApplyScope", encoder);
       tfx::activate_technique("SkyApply", encoder);

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