#include <engine/assert.h>
#include <engine/assets/assets_manager.h>
#include <engine/ecs/ecs.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/render/terrain_render.h>
#include <engine/tfx/tfx.h>
#include <engine/utils/bits.h>
#include <engine/work_cycle/camera.h>

ECS_QUERY()
static
void query_vterrain(eastl::function<void(Engine::Render::TerrainRender& vterrain_render)> cb);

ECS_EVENT_SYSTEM()
static
void virtual_terrain_creation_handler(
  const ecs::OnEntityCreated& evt,
  const string& vterrain_name,
  const float2& vterrain_heightmapMinMaxBorder,
  const float vterrain_heightmapMaxHeight,
  const int vterrain_patchSideBits,
  const int vterrain_worldSize_meters,
  const int vterrain_detailSize,
  const string& vterrain_detail
  )
{
  ASSERT_FMT(
    Utils::is_pow_of_2(vterrain_worldSize_meters) &&
    vterrain_worldSize_meters > 0,
    "Virtual Terrain has invalid world size[{}]. It must be: power of 2, > 0",
    vterrain_worldSize_meters);

  uint tileSize = 1 << vterrain_patchSideBits;
  ASSERT(tileSize < vterrain_worldSize_meters);

  ecs::EntityComponents init;
  init["vterrain_render"] = Engine::Render::TerrainRender {
    vterrain_name, vterrain_patchSideBits, vterrain_worldSize_meters,
    vterrain_heightmapMinMaxBorder, vterrain_heightmapMaxHeight,
    vterrain_detail, vterrain_detailSize};
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
  vterrain_render.setView(cameraPos);
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
  query_vterrain([&](auto& terrain)
  {
    terrain.updateGpuData(encoder);
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
    terrain.render(encoder, terrainFeedback, uint2(gapi::get_texture_extent(terrainFeedback)));
    encoder.endRenderpass();
  });
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
  query_vterrain([&](auto& terrain)
  {
    terrain.updateVTex(encoder, terrainFeedback, gapi::get_texture_extent(terrainFeedback));
  });
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
      query_vterrain([&](auto& terrain)
      {
        tex = terrain.getVTexTilesStorage();
      });

      return fg::TextureImport{tex, gapi::TextureState::ShaderRead};
    });
    return [](gapi::CmdEncoder&) {};
  });
}
