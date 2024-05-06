// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "ap_lut.ecs.cpp" 

using namespace ecs;

//Engine::OnFrameGraphInit handler
static
void mk_fg_node_atm_ap_lut_render(Event*, ComponentsAccessor&)
{
  fg::register_node("atm_ap_lut_render", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto atm_tr_lut = reg.readTexture("atm_tr_lut", gapi::TextureState::ShaderRead, false);
    auto atm_ms_lut = reg.readTexture("atm_ms_lut", gapi::TextureState::ShaderRead, false);
    auto atm_ap_lut = reg.modifyTexture("atm_ap_lut", gapi::TextureState::ShaderReadWrite);

    return [atm_tr_lut,atm_ms_lut,atm_ap_lut](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("trLUT", atm_tr_lut.get());
      tfx::set_extern("msLUT", atm_ms_lut.get());
      tfx::set_extern("apLUT", atm_ap_lut.get());
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