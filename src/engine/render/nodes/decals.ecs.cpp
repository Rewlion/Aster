#include <engine/assets/assets_manager.h>
#include <engine/ecs/macros.h>
#include <engine/events.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/tfx/tfx.h>

#include <glm/gtx/transform.hpp>

ECS_DESCRIBE_QUERY(query_decals, (const string& decal_name, const float3& size, const float3& pos));

ECS_EVENT_SYSTEM()
static
void mk_decals_node(const Engine::OnFrameGraphInit&)
{
  fg::register_node("decals", FG_FILE_DECL, [](fg::Registry& reg)
  {
    auto depth = reg.readTexture("late_opaque_depth",  gapi::TextureState::DepthReadStencilRead);

    reg.requestRenderPass()
       .addTarget("gbuf0")
       .addRODepth("late_opaque_depth");

    return [depth](gapi::CmdEncoder& encoder)
    {
      query_decals([&encoder, depth](auto& decal_name, auto& size, auto& pos){
        
        const mat4 scale = glm::scale(size);
        const mat4 tr = glm::translate(pos);
        const mat4 tm = tr * scale;
        const mat4 iTm = glm::inverse(tm);

        Engine::TextureAsset asset;
        Engine::assets_manager.getTexture(decal_name+".albedo", asset);

        tfx::set_extern("depthTex", depth.get());
        tfx::set_channel("modelTm", tm);
        tfx::set_channel("invModelTm", iTm);
        tfx::set_channel("decalTex", asset.texture);
        tfx::activate_scope("DecalScope", encoder);
        tfx::activate_technique("Decal", encoder);
        
        encoder.updateResources();
        encoder.draw(14, 1, 0, 0);
      });
    };
  });
}