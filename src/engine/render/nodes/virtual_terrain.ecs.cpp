#include <engine/assert.h>
#include <engine/assets/assets_manager.h>
#include <engine/ecs/ecs.h>
#include <engine/ecs/macros.h>
#include <engine/events.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/frame_graph/frame_graph.h>
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

ECS_EVENT_SYSTEM()
static
void virtual_terrain_node(
  const Engine::OnFrameGraphInit&
)
{
  fg::register_node("terrain", FG_FILE_DECL,
    [](fg::Registry& reg)
  {
    auto gbuf0 = reg.modifyTexture("gbuf0", gapi::TextureState::RenderTarget);
    auto gbuf1 = reg.modifyTexture("gbuf1", gapi::TextureState::RenderTarget);
    auto gbuf2 = reg.modifyTexture("gbuf2", gapi::TextureState::RenderTarget);
    auto gbufDepth = reg.modifyTexture("opaque_depth", gapi::TextureState::DepthWriteStencilWrite);

    const int2 feedbackSize = Engine::Render::get_render_size() / 10;
    auto feedbackBuf = reg.createTexture("terrainFeedback", gapi::TextureAllocationDescription{
      .format = gapi::TextureFormat::R32_UINT,
      .extent = {feedbackSize, 1.0},
      .usage = gapi::TEX_USAGE_UAV | gapi::TEX_USAGE_TRANSFER_SRC
    }, gapi::TextureState::ShaderReadWrite);

    auto feedbackSizeBlob = reg.createBlob<int2>("feedbackSize");

    return [gbuf0, gbuf1, gbuf2, gbufDepth, feedbackBuf, feedbackSizeBlob, feedbackSize](gapi::CmdEncoder& encoder)
    {
      feedbackSizeBlob.get() = feedbackSize;

      query_vterrain([&](auto& terrain)
      {
        terrain.updateGpuData(encoder);
        encoder.clearColorTexture(feedbackBuf.get(), gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite, {(uint32_t)~0}, {});

        encoder.beginRenderpass(
          {
            gapi::RenderPassAttachment{.texture = gbuf0.get()},
            gapi::RenderPassAttachment{.texture = gbuf1.get()},
            gapi::RenderPassAttachment{.texture = gbuf2.get()}
          },
          gapi::RenderPassDepthStencilAttachment{
            .texture = gbufDepth.get(),
            .initialState = gapi::TextureState::DepthWriteStencilWrite,
            .finalState = gapi::TextureState::DepthWriteStencilWrite,
          }
        );
        terrain.render(encoder, feedbackBuf.get(), feedbackSize);
        encoder.endRenderpass();
      });
    };
  });
}

ECS_EVENT_SYSTEM()
static
void virtual_terrain_update_vtex_node(
  const Engine::OnFrameGraphInit&
)
{
  fg::register_node("terrain_update_vtex", FG_FILE_DECL,
    [](fg::Registry& reg)
  {
    reg.orderMeAfter("terrain");
    auto feedbackBuf = reg.readTexture("terrainFeedback", gapi::TextureState::TransferSrc);
    auto feedbackSize = reg.readBlob<int2>("feedbackSize");
    
    auto gbuf0 = reg.modifyTexture("gbuf0", gapi::TextureState::RenderTarget);
    // auto gbuf1 = reg.modifyTexture("gbuf1", gapi::TextureState::RenderTarget);
    // auto gbuf2 = reg.modifyTexture("gbuf2", gapi::TextureState::RenderTarget);

    return [feedbackBuf, feedbackSize](gapi::CmdEncoder& encoder)
    {
      query_vterrain([&](auto& terrain)
      {
        terrain.updateVTex(encoder, feedbackBuf.get(), feedbackSize.get());
      });
    };
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
