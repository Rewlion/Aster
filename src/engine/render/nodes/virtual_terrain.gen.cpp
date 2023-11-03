// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "virtual_terrain.ecs.cpp" 

using namespace ecs;

const static DirectQueryRegistration query_vterrain_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("vterrain_render", Engine::Render::TerrainRender)
  },
  "query_vterrain"};
const static query_id_t query_vterrain_queryId = query_vterrain_queryReg.getId();


void query_vterrain (eastl::function<
  void(
    Engine::Render::TerrainRender& vterrain_render)> cb)
{
  ecs::get_registry().query(query_vterrain_queryId, [&](ComponentsAccessor& accessor)
  {
    Engine::Render::TerrainRender& vterrain_render = accessor.get<Engine::Render::TerrainRender>(compile_ecs_name_hash("vterrain_render"));
    cb(vterrain_render);
  });
}


static void virtual_terrain_creation_handler_internal(Event* event, ComponentsAccessor& accessor)
{
  const ecs::OnEntityCreated* casted_event = reinterpret_cast<const ecs::OnEntityCreated*>(event);
  const string& vterrain_name = accessor.get<string>(compile_ecs_name_hash("vterrain_name"));
  const float2& vterrain_heightmapMinMaxBorder = accessor.get<float2>(compile_ecs_name_hash("vterrain_heightmapMinMaxBorder"));
  float vterrain_heightmapMaxHeight = accessor.get<float>(compile_ecs_name_hash("vterrain_heightmapMaxHeight"));
  int vterrain_patchSideBits = accessor.get<int>(compile_ecs_name_hash("vterrain_patchSideBits"));
  int vterrain_worldSize_meters = accessor.get<int>(compile_ecs_name_hash("vterrain_worldSize_meters"));
  int vterrain_detailSize = accessor.get<int>(compile_ecs_name_hash("vterrain_detailSize"));
  const string& vterrain_detail = accessor.get<string>(compile_ecs_name_hash("vterrain_detail"));
  virtual_terrain_creation_handler(*casted_event, vterrain_name,vterrain_heightmapMinMaxBorder,vterrain_heightmapMaxHeight,vterrain_patchSideBits,vterrain_worldSize_meters,vterrain_detailSize,vterrain_detail);
}


static EventSystemRegistration virtual_terrain_creation_handler_registration(
  virtual_terrain_creation_handler_internal,
  compile_ecs_name_hash("OnEntityCreated"),
  {
    DESCRIBE_QUERY_COMPONENT("vterrain_name", string),
    DESCRIBE_QUERY_COMPONENT("vterrain_heightmapMinMaxBorder", float2),
    DESCRIBE_QUERY_COMPONENT("vterrain_heightmapMaxHeight", float),
    DESCRIBE_QUERY_COMPONENT("vterrain_patchSideBits", int),
    DESCRIBE_QUERY_COMPONENT("vterrain_worldSize_meters", int),
    DESCRIBE_QUERY_COMPONENT("vterrain_detailSize", int),
    DESCRIBE_QUERY_COMPONENT("vterrain_detail", string)
  },
  "virtual_terrain_creation_handler"
);


static void virtual_terrain_on_tick_internal(Event* event, ComponentsAccessor& accessor)
{
  const Engine::OnGameTick* casted_event = reinterpret_cast<const Engine::OnGameTick*>(event);
  Engine::Render::TerrainRender& vterrain_render = accessor.get<Engine::Render::TerrainRender>(compile_ecs_name_hash("vterrain_render"));
  virtual_terrain_on_tick(*casted_event, vterrain_render);
}


static EventSystemRegistration virtual_terrain_on_tick_registration(
  virtual_terrain_on_tick_internal,
  compile_ecs_name_hash("OnGameTick"),
  {
    DESCRIBE_QUERY_COMPONENT("vterrain_render", Engine::Render::TerrainRender)
  },
  "virtual_terrain_on_tick"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_terrain(Event*, ComponentsAccessor&)
{
  fg::register_node("terrain", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    auto gbuf0 = reg.modifyTexture("gbuf0", gapi::TextureState::RenderTarget);
    auto gbuf1 = reg.modifyTexture("gbuf1", gapi::TextureState::RenderTarget);
    auto gbuf2 = reg.modifyTexture("gbuf2", gapi::TextureState::RenderTarget);
    auto opaque_depth = reg.modifyTexture("opaque_depth", gapi::TextureState::DepthWriteStencilWrite);

    auto terrainFeedback = reg.createTexture("terrainFeedback",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32_UINT,
        .extent =          uint3(__renderSize__ / uint(10), 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_UAV | gapi::TextureUsage::TEX_USAGE_TRANSFER_SRC)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );


    return [opaque_depth,gbuf0,gbuf2,terrainFeedback,gbuf1](gapi::CmdEncoder& encoder)
    {
      terrain_exec(encoder, gbuf0.get(), gbuf1.get(), gbuf2.get(), opaque_depth.get(), terrainFeedback.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_terrain_registration(
  mk_fg_node_terrain,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_terrain"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_terrain_update_vtex(Event*, ComponentsAccessor&)
{
  fg::register_node("terrain_update_vtex", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeAfter("terrain");
    auto terrainFeedback = reg.readTexture("terrainFeedback", gapi::TextureState::TransferSrc, false);
    auto gbuf0 = reg.modifyTexture("gbuf0", gapi::TextureState::RenderTarget);

    return [terrainFeedback](gapi::CmdEncoder& encoder)
    {
      terrain_update_vtex_exec(encoder, terrainFeedback.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_terrain_update_vtex_registration(
  mk_fg_node_terrain_update_vtex,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_terrain_update_vtex"
);


static void virtual_terrain_vtx_dbg_import_node_internal(Event* event, ComponentsAccessor& accessor)
{
  const Engine::OnFrameGraphInit* casted_event = reinterpret_cast<const Engine::OnFrameGraphInit*>(event);

  virtual_terrain_vtx_dbg_import_node(*casted_event);
}


static EventSystemRegistration virtual_terrain_vtx_dbg_import_node_registration(
  virtual_terrain_vtx_dbg_import_node_internal,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {

  },
  "virtual_terrain_vtx_dbg_import_node"
);
