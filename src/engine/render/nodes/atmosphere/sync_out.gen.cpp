// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "sync_out.ecs.cpp" 

using namespace ecs;

const static DirectQueryRegistration query_atmosphere_render_state_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("atmosphere_render_state", AtmosphereRenderState)
  },
  "query_atmosphere_render_state"};
const static query_id_t query_atmosphere_render_state_queryId = query_atmosphere_render_state_queryReg.getId();


void query_atmosphere_render_state (eastl::function<
  void(
    AtmosphereRenderState& atmosphere_render_state)> cb)
{
  ecs::get_registry().query(query_atmosphere_render_state_queryId, [&](ComponentsAccessor& accessor)
  {
    AtmosphereRenderState& atmosphere_render_state = accessor.get<AtmosphereRenderState>(compile_ecs_name_hash("atmosphere_render_state"));
    cb(atmosphere_render_state);
  });
}


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_sync_out(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_sync_out", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeAfter("frame_preparing");
    auto atm_envi_specular_srv = reg.renameTexture("atm_envi_specular", "atm_envi_specular_srv", gapi::TextureState::ShaderRead);
    auto atm_envi_brdf_srv = reg.renameTexture("atm_envi_brdf", "atm_envi_brdf_srv", gapi::TextureState::ShaderRead);
    auto atm_tr_lut_srv = reg.renameTexture("atm_tr_lut", "atm_tr_lut_srv", gapi::TextureState::ShaderRead);
    auto atm_ms_lut_srv = reg.renameTexture("atm_ms_lut", "atm_ms_lut_srv", gapi::TextureState::ShaderRead);
    auto atm_sky_lut_srv = reg.renameTexture("atm_sky_lut", "atm_sky_lut_srv", gapi::TextureState::ShaderRead);
    auto atm_ap_lut_srv = reg.renameTexture("atm_ap_lut", "atm_ap_lut_srv", gapi::TextureState::ShaderRead);

    return [](gapi::CmdEncoder& )
    {
      atm_sync_out_exec();
    };
  });
}

static
EventSystemRegistration mk_fg_node_atm_sync_out_registration(
  mk_fg_node_atm_sync_out,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_atm_sync_out"
);