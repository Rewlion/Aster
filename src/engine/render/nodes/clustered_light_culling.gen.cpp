// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "clustered_light_culling.ecs.cpp" 

using namespace ecs;

const static DirectQueryRegistration query_clustered_lights_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("clustered_lights", Engine::Render::ClusteredLights)
  },
  "query_clustered_lights"};
const static query_id_t query_clustered_lights_queryId = query_clustered_lights_queryReg.getId();


void query_clustered_lights (eastl::function<
  void(
    Engine::Render::ClusteredLights& clustered_lights)> cb)
{
  ecs::get_registry().query(query_clustered_lights_queryId, [&](ComponentsAccessor& accessor)
  {
    Engine::Render::ClusteredLights& clustered_lights = accessor.get<Engine::Render::ClusteredLights>(compile_ecs_name_hash("clustered_lights"));
    cb(clustered_lights);
  });
}


static void clustered_render_prepare_internal(Event* event, ComponentsAccessor& accessor)
{
  const Engine::OnBeforeRender* casted_event = reinterpret_cast<const Engine::OnBeforeRender*>(event);
  Engine::Render::ClusteredLights& clustered_lights = accessor.get<Engine::Render::ClusteredLights>(compile_ecs_name_hash("clustered_lights"));
  clustered_render_prepare(*casted_event, clustered_lights);
}


static EventSystemRegistration clustered_render_prepare_registration(
  clustered_render_prepare_internal,
  compile_ecs_name_hash("OnBeforeRender"),
  {
    DESCRIBE_QUERY_COMPONENT("clustered_lights", Engine::Render::ClusteredLights)
  },
  "clustered_render_prepare"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_clustered_light_resources(Event*, ComponentsAccessor&)
{
  fg::register_node("clustered_light_resources", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto clusters_info = reg.createBuffer(
      "clusters_info",
      gapi::BufferAllocationDescription{
        .size = (16 * 8 * 24) * sizeof(ClusterInfo),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW
    );
    auto clusters_indirecion = reg.createBuffer(
      "clusters_indirecion",
      gapi::BufferAllocationDescription{
        .size = (16 * 8 * 24) * 128 * sizeof(ClusterIndirection),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW
    );
    auto clusters_indirecion_offset = reg.createBuffer(
      "clusters_indirecion_offset",
      gapi::BufferAllocationDescription{
        .size = sizeof(uint),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_TRANSFER_DST
    );
    auto clustered_lights = reg.importBufferProducer("clustered_lights", import_clustered_lights);

    return [clusters_indirecion_offset](gapi::CmdEncoder& encoder)
    {
      clustered_light_resources(encoder, clusters_indirecion_offset.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_clustered_light_resources_registration(
  mk_fg_node_clustered_light_resources,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_clustered_light_resources"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_build_light_clusters(Event*, ComponentsAccessor&)
{
  fg::register_node("build_light_clusters", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto frustum_clusters = reg.createBuffer(
      "frustum_clusters",
      gapi::BufferAllocationDescription{
        .size = (16 * 8 * 24) * sizeof(Cluster),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW
    );

    return [frustum_clusters](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("frustumClusters", frustum_clusters.get());
      build_light_clusters(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_build_light_clusters_registration(
  mk_fg_node_build_light_clusters,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_build_light_clusters"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_clustered_lights_culling(Event*, ComponentsAccessor&)
{
  fg::register_node("clustered_lights_culling", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto frustum_clusters = reg.readBuffer("frustum_clusters", gapi::BufferState::BF_STATE_SRV, false);
    auto clustered_lights = reg.readBuffer("clustered_lights", gapi::BufferState::BF_STATE_SRV, false);
    auto clusters_info = reg.modifyBuffer("clusters_info", gapi::BufferState::BF_STATE_UAV_RW);
    auto clusters_indirecion = reg.modifyBuffer("clusters_indirecion", gapi::BufferState::BF_STATE_UAV_RW);
    auto clusters_indirecion_offset = reg.modifyBuffer("clusters_indirecion_offset", gapi::BufferState::BF_STATE_UAV_RW);

    return [frustum_clusters,clustered_lights,clusters_info,clusters_indirecion,clusters_indirecion_offset](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("frustumClusters", frustum_clusters.get());
      tfx::set_extern("clustersLightBuffer", clustered_lights.get());
      tfx::set_extern("clustersInfoBuffer", clusters_info.get());
      tfx::set_extern("clustersIndirectionBuffer", clusters_indirecion.get());
      tfx::set_extern("clustersIndirectionBufferNextPos", clusters_indirecion_offset.get());
      clustered_lights_culling(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_clustered_lights_culling_registration(
  mk_fg_node_clustered_lights_culling,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_clustered_lights_culling"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_dbg_clustered_render(Event*, ComponentsAccessor&)
{
  fg::register_node("dbg_clustered_render", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeBefore("ui");
    auto clusters_info = reg.readBuffer("clusters_info", gapi::BufferState::BF_STATE_SRV, false);
    auto rt = reg.modifyTexture("final_target", gapi::TextureState::RenderTarget);

    return [clusters_info,rt](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("clustersInfoBuffer", clusters_info.get());
      dbg_clustered_render_exec(encoder, rt.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_dbg_clustered_render_registration(
  mk_fg_node_dbg_clustered_render,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_dbg_clustered_render"
);
