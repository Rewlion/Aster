// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "tr_lut.ecs.cpp" 

using namespace ecs;

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
