// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "atmosphere.ecs.cpp" 

using namespace ecs;

const static DirectQueryRegistration query_atm_tr_lut_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("atm_tr_lut", gapi::TextureWrapper)
  },
  "query_atm_tr_lut"};
const static query_id_t query_atm_tr_lut_queryId = query_atm_tr_lut_queryReg.getId();


void query_atm_tr_lut (eastl::function<
  void(
    const gapi::TextureWrapper& atm_tr_lut)> cb)
{
  ecs::get_registry().query(query_atm_tr_lut_queryId, [&](ComponentsAccessor& accessor)
  {
    const gapi::TextureWrapper& atm_tr_lut = accessor.get<gapi::TextureWrapper>(compile_ecs_name_hash("atm_tr_lut"));
    cb(atm_tr_lut);
  });
}


const static DirectQueryRegistration query_atm_params_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("atm_radius_bot_km", float),
    DESCRIBE_QUERY_COMPONENT("atm_radius_top_km", float)
  },
  "query_atm_params"};
const static query_id_t query_atm_params_queryId = query_atm_params_queryReg.getId();


void query_atm_params (eastl::function<
  void(
    float atm_radius_bot_km,
    float atm_radius_top_km)> cb)
{
  ecs::get_registry().query(query_atm_params_queryId, [&](ComponentsAccessor& accessor)
  {
    float atm_radius_bot_km = accessor.get<float>(compile_ecs_name_hash("atm_radius_bot_km"));
    float atm_radius_top_km = accessor.get<float>(compile_ecs_name_hash("atm_radius_top_km"));
    cb(atm_radius_bot_km,atm_radius_top_km);
  });
}


const static DirectQueryRegistration query_sun_params_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("sun_azimuth", float),
    DESCRIBE_QUERY_COMPONENT("sun_altitude", float)
  },
  "query_sun_params"};
const static query_id_t query_sun_params_queryId = query_sun_params_queryReg.getId();


void query_sun_params (eastl::function<
  void(
    float sun_azimuth,
    float sun_altitude)> cb)
{
  ecs::get_registry().query(query_sun_params_queryId, [&](ComponentsAccessor& accessor)
  {
    float sun_azimuth = accessor.get<float>(compile_ecs_name_hash("sun_azimuth"));
    float sun_altitude = accessor.get<float>(compile_ecs_name_hash("sun_altitude"));
    cb(sun_azimuth,sun_altitude);
  });
}


const static DirectQueryRegistration query_atm_lut_state_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("atm_lut_state", int)
  },
  "query_atm_lut_state"};
const static query_id_t query_atm_lut_state_queryId = query_atm_lut_state_queryReg.getId();


void query_atm_lut_state (eastl::function<
  void(
    int& atm_lut_state)> cb)
{
  ecs::get_registry().query(query_atm_lut_state_queryId, [&](ComponentsAccessor& accessor)
  {
    int& atm_lut_state = accessor.get<int>(compile_ecs_name_hash("atm_lut_state"));
    cb(atm_lut_state);
  });
}


static void atmosphere_creation_handler_internal(Event* event, ComponentsAccessor& accessor)
{
  const ecs::OnEntityCreated* casted_event = reinterpret_cast<const ecs::OnEntityCreated*>(event);
  float atm_radius_bot_km = accessor.get<float>(compile_ecs_name_hash("atm_radius_bot_km"));
  float atm_radius_top_km = accessor.get<float>(compile_ecs_name_hash("atm_radius_top_km"));
  atmosphere_creation_handler(*casted_event, atm_radius_bot_km,atm_radius_top_km);
}


static EventSystemRegistration atmosphere_creation_handler_registration(
  atmosphere_creation_handler_internal,
  compile_ecs_name_hash("OnEntityCreated"),
  {
    DESCRIBE_QUERY_COMPONENT("atm_radius_bot_km", float),
    DESCRIBE_QUERY_COMPONENT("atm_radius_top_km", float)
  },
  "atmosphere_creation_handler"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_res_import(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_res_import", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeAfter("frame_preparing");
    auto camera_data = reg.readBlob<Engine::CameraData>("camera_data");

    auto atm_tr_lut = reg.createTexture("atm_tr_lut",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(int2(256, 64), 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::Undefined,
      fg::PERSISTENT
    );


    auto atm_ms_lut = reg.createTexture("atm_ms_lut",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(int2(32, 32), 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::Undefined,
      fg::PERSISTENT
    );


    auto atm_sky_lut = reg.createTexture("atm_sky_lut",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(int2(256, 128), 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::Undefined,
      fg::PERSISTENT
    );


    auto atm_ap_lut = reg.createTexture("atm_ap_lut",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(int3(32, 32, 32).x, int3(32, 32, 32).y, int3(32, 32, 32).z),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_UAV)
      },
      gapi::TextureState::Undefined,
      fg::PERSISTENT
    );


    auto atm_envi_specular = reg.createTexture("atm_envi_specular",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(int2(512, 256), 1),
        .mipLevels =       get_envi_specular_mips(),
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::Undefined,
      fg::PERSISTENT
    );


    auto atm_envi_brdf = reg.createTexture("atm_envi_brdf",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(int2(256, 256), 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::Undefined,
      fg::PERSISTENT
    );

    auto atm_envi_mips = reg.createBlob<int>("atm_envi_mips");

    return [atm_envi_mips,camera_data](gapi::CmdEncoder& encoder)
    {
      atm_res_import_exec(encoder, atm_envi_mips.get(), camera_data.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_atm_res_import_registration(
  mk_fg_node_atm_res_import,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_atm_res_import"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_ap_lut_render(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_ap_lut_render", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto trLUT = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead, false);
    auto msLUT = reg.readTexture("atm_ms_lut", gapi::TextureState::ShaderRead, false);
    auto apLUT = reg.modifyTexture("atm_ap_lut", gapi::TextureState::ShaderReadWrite);

    return [trLUT,msLUT,apLUT](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUT.get());
      tfx::set_extern("msLUT", msLUT.get());
      tfx::set_extern("apLUT", apLUT.get());
      atm_ap_lut_render_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_atm_ap_lut_render_registration(
  mk_fg_node_atm_ap_lut_render,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_atm_ap_lut_render"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_tr_lut_render(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_tr_lut_render", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeAfter("frame_preparing");
    reg.requestRenderPass()
      .addTarget("atm_tr_lut", gapi::LoadOp::DontCare, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
    ;


    return [](gapi::CmdEncoder& encoder)
    {
      atm_tr_lut_render_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_atm_tr_lut_render_registration(
  mk_fg_node_atm_tr_lut_render,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_atm_tr_lut_render"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_ms_lut_render(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_ms_lut_render", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto trLUT = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead, false);
    reg.requestRenderPass()
      .addTarget("atm_ms_lut", gapi::LoadOp::DontCare, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
    ;


    return [trLUT](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUT.get());
      atm_ms_lut_render_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_atm_ms_lut_render_registration(
  mk_fg_node_atm_ms_lut_render,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_atm_ms_lut_render"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_sky_lut_render(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_sky_lut_render", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto trLUT = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead, false);
    auto msLUT = reg.readTexture("atm_ms_lut", gapi::TextureState::ShaderRead, false);
    reg.requestRenderPass()
      .addTarget("atm_sky_lut", gapi::LoadOp::DontCare, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
    ;


    return [trLUT,msLUT](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUT.get());
      tfx::set_extern("msLUT", msLUT.get());
      atm_sky_lut_render_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_atm_sky_lut_render_registration(
  mk_fg_node_atm_sky_lut_render,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_atm_sky_lut_render"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_sph_render(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_sph_render", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto atmParamsBuffer = reg.modifyBuffer("sph_buf", gapi::BufferState::BF_STATE_UAV_RW);
    auto trLUT = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead, false);
    auto skyLUT = reg.readTexture("atm_sky_lut", gapi::TextureState::ShaderRead, false);

    return [atmParamsBuffer,trLUT,skyLUT](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("atmParamsBuffer", atmParamsBuffer.get());
      tfx::set_extern("trLUT", trLUT.get());
      tfx::set_extern("skyLUT", skyLUT.get());
      atm_sph_render_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_atm_sph_render_registration(
  mk_fg_node_atm_sph_render,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_atm_sph_render"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_sky_apply(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_sky_apply", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto apLUT = reg.readTexture("atm_ap_lut", gapi::TextureState::ShaderRead, false);
    auto skyLUT = reg.readTexture("atm_sky_lut", gapi::TextureState::ShaderRead, false);
    auto trLUT = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead, false);
    auto gbufferDepth = reg.readTexture("gbuffer_depth", gapi::TextureState::DepthReadStencilRead, false);
    reg.requestRenderPass()
      .addTarget("resolve_target", gapi::LoadOp::Load, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
    ;


    return [apLUT,skyLUT,trLUT,gbufferDepth](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("apLUT", apLUT.get());
      tfx::set_extern("skyLUT", skyLUT.get());
      tfx::set_extern("trLUT", trLUT.get());
      tfx::set_extern("gbufferDepth", gbufferDepth.get());
      atm_sky_apply_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_atm_sky_apply_registration(
  mk_fg_node_atm_sky_apply,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_atm_sky_apply"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_envi_specular(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_envi_specular", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto trLUT = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead, false);
    auto skyLUT = reg.readTexture("atm_sky_lut", gapi::TextureState::ShaderRead, false);
    auto atm_envi_specular = reg.modifyTexture("atm_envi_specular", gapi::TextureState::RenderTarget);
    auto atm_envi_mips = reg.readBlob<int>("atm_envi_mips");

    return [trLUT,skyLUT,atm_envi_specular,atm_envi_mips](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", trLUT.get());
      tfx::set_extern("skyLUT", skyLUT.get());
      atm_envi_specular_exec(encoder, atm_envi_specular.get(), atm_envi_mips.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_atm_envi_specular_registration(
  mk_fg_node_atm_envi_specular,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_atm_envi_specular"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_envi_brdf(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_envi_brdf", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.requestRenderPass()
      .addTarget("atm_envi_brdf", gapi::LoadOp::DontCare, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
    ;


    return [](gapi::CmdEncoder& encoder)
    {
      atm_envi_brdf_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_atm_envi_brdf_registration(
  mk_fg_node_atm_envi_brdf,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_atm_envi_brdf"
);


const static DirectQueryRegistration query_sun_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("sun_azimuth", float),
    DESCRIBE_QUERY_COMPONENT("sun_altitude", float)
  },
  "query_sun"};
const static query_id_t query_sun_queryId = query_sun_queryReg.getId();


void query_sun (eastl::function<
  void(
    float& sun_azimuth,
    float& sun_altitude)> cb)
{
  ecs::get_registry().query(query_sun_queryId, [&](ComponentsAccessor& accessor)
  {
    float& sun_azimuth = accessor.get<float>(compile_ecs_name_hash("sun_azimuth"));
    float& sun_altitude = accessor.get<float>(compile_ecs_name_hash("sun_altitude"));
    cb(sun_azimuth,sun_altitude);
  });
}
