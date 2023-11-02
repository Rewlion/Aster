#include <engine/assets/assets_manager.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/tfx/tfx.h>

#include <glm/gtx/transform.hpp>

ECS_DESCRIBE_QUERY(query_decals, (const string& decal_name, const float3& size, const float3& pos));

NODE_BEGIN(decals)
  READ_TEX(late_opaque_depth, TEX_STATE(DepthReadStencilRead))
  RP_BEGIN()
    TARGET(gbuf0),
    DEPTH(late_opaque_depth)
  RP_END()
  EXEC(decals_exec)
NODE_END()

NODE_EXEC()
static
void decals_exec(const gapi::TextureHandle late_opaque_depth, gapi::CmdEncoder& encoder)
{
  query_decals([&encoder, late_opaque_depth](auto& decal_name, auto& size, auto& pos){
    const mat4 scale = glm::scale(size);
    const mat4 tr = glm::translate(pos);
    const mat4 tm = tr * scale;
    const mat4 iTm = glm::inverse(tm);

    Engine::TextureAsset asset;
    Engine::assets_manager.getTexture(decal_name+".albedo", asset);

    tfx::set_extern("depthTex", late_opaque_depth);
    tfx::set_channel("modelTm", tm);
    tfx::set_channel("invModelTm", iTm);
    tfx::set_channel("decalTex", asset.texture);
    tfx::activate_scope("DecalScope", encoder);
    tfx::activate_technique("Decal", encoder);
    
    encoder.updateResources();
    encoder.draw(14, 1, 0, 0);
  });
}
