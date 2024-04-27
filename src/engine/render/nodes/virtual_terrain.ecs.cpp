#include <engine/assert.h>
#include <engine/assets/assets_manager.h>
#include <engine/components/terrain.h>
#include <engine/ecs/ecs.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/render/terrain_render.h>
#include <engine/tfx/tfx.h>
#include <engine/utils/bits.h>
#include <engine/work_cycle/camera.h>

ECS_COMP_GETTER(Engine::Render::TerrainRender, vterrain_render);
ECS_COMP_GETTER(VTerrainComponent, vterrain);

ECS_EVENT_SYSTEM()
static
void virtual_terrain_creation_handler(
  const ecs::OnEntityCreated& evt,
  const VTerrainComponent& vterrain
  )
{
  ecs::EntityComponents init;
  init["vterrain_render"] = Engine::Render::TerrainRender {vterrain};
  ecs::get_registry().createEntity("VirtualTerrainRender", std::move(init));
}

ECS_EVENT_SYSTEM()
static
void virtual_terrain_on_tick(
  const Engine::OnGameTick& evt,
  Engine::Render::TerrainRender& vterrain_render
)
{
  float3 cameraPos = Engine::get_camera_pos();
  const VTerrainComponent* vt = get_vterrain();
  if (vt)
    vterrain_render.setView(*vt, cameraPos);
}

NODE_BEGIN(terrain)
  MODIFY_TEX_RT(gbuf0)
  MODIFY_TEX_RT(gbuf1)
  MODIFY_TEX_RT(gbuf2)
  MODIFY_TEX_DEPTH(opaque_depth)

  CREATE_TEX_2D(terrainFeedback, TEX_SIZE_RELATIVE_DIV(10), R32_UINT,  TEX_USAGE2(UAV, TRANSFER_SRC), TEX_STATE(ShaderReadWrite))
  
  EXEC(terrain_exec)
NODE_END()

NODE_EXEC()
static
void terrain_exec(gapi::CmdEncoder& encoder,
                  const gapi::TextureHandle gbuf0,
                  const gapi::TextureHandle gbuf1,
                  const gapi::TextureHandle gbuf2,
                  const gapi::TextureHandle opaque_depth,
                  const gapi::TextureHandle terrainFeedback)
{
  Engine::Render::TerrainRender* terrain = get_vterrain_render();
  const VTerrainComponent* vt = get_vterrain();
  if (terrain && vt)
  {
    terrain->updateGpuData(encoder);
    encoder.clearColorTexture(terrainFeedback, gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite, {(uint32_t)~0}, {});

    encoder.beginRenderpass(
      {
        gapi::RenderPassAttachment{.texture = gbuf0},
        gapi::RenderPassAttachment{.texture = gbuf1},
        gapi::RenderPassAttachment{.texture = gbuf2}
      },
      gapi::RenderPassDepthStencilAttachment{
        .texture = opaque_depth,
        .initialState = gapi::TextureState::DepthWriteStencilWrite,
        .finalState = gapi::TextureState::DepthWriteStencilWrite,
      }
    );
    terrain->render(*vt, encoder, terrainFeedback, uint2(gapi::get_texture_extent(terrainFeedback)));
    encoder.endRenderpass();
  };
}

NODE_BEGIN(terrain_update_vtex)
  ORDER_ME_AFTER(terrain)

  READ_TEX_TRANSFER_SRC(terrainFeedback)
  MODIFY_TEX_RT(gbuf0) // ??

  EXEC(terrain_update_vtex_exec)
NODE_END()

NODE_EXEC()
static
void terrain_update_vtex_exec(gapi::CmdEncoder& encoder,
                              const gapi::TextureHandle terrainFeedback)
{
  Engine::Render::TerrainRender* terrain = get_vterrain_render();
  const VTerrainComponent* vt = get_vterrain();
  if (terrain && vt)
  {
    terrain->updateVTex(*vt, encoder, terrainFeedback, gapi::get_texture_extent(terrainFeedback));
  };
}

ECS_EVENT_SYSTEM()
static
void virtual_terrain_vtx_dbg_import_node(
  const Engine::OnFrameGraphInit&
)
{
  fg::register_node("terrain_vtx_dbg_import", FG_FILE_DECL,
    [](fg::Registry& reg)
  {
    reg.orderMeAfter("terrain_update_vtex");
    reg.importTextureProducer("terrain_vtex", [&](){
      gapi::TextureHandle tex = gapi::TextureHandle::Invalid;
      Engine::Render::TerrainRender* terrain;
      if (terrain)
      {
        tex = terrain->getVTexTilesStorage();
      };

      return fg::TextureImport{tex, gapi::TextureState::ShaderRead};
    });
    return [](gapi::CmdEncoder&) {};
  });
}
