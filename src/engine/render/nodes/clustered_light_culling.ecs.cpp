#include <engine/console/cmd.h>
#include <engine/render/clustered_lights.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/tfx/tfx.h>

#include <engine/shaders/shaders/clustered_light/utils.hlsl>


ECS_DESCRIBE_QUERY(query_clustered_lights, Engine::Render::ClusteredLights& clustered_lights);

ECS_EVENT_SYSTEM()
void clustered_render_prepare(const Engine::OnBeforeRender& evt,
                              Engine::Render::ClusteredLights& clustered_lights)
{
  clustered_lights.beforeRender(*evt.encoder);
}

auto import_clustered_lights() -> fg::BufferImport
{
  gapi::BufferHandler buf = gapi::BufferHandler::Invalid;
  query_clustered_lights([&](Engine::Render::ClusteredLights& clustered_lights){
    buf = clustered_lights.getLightsBuffer();
  });

  return {.buf = buf, .initState = gapi::BufferState::BF_STATE_SRV};
}

NODE_BEGIN(clustered_light_resources)
  CREATE_GPU_BUF(clusters_info, BUF_USAGE(UAV), BUF_STATE(UAV_RW), BUF_SIZE(CLUSTERS_NUM * sizeof(ClusterInfo)))
  CREATE_GPU_BUF(clusters_indirecion, BUF_USAGE(UAV), BUF_STATE(UAV_RW), BUF_SIZE(CLUSTERS_NUM * CLUSTER_MAX_ITEMS * sizeof(ClusterIndirection)))
  CREATE_GPU_BUF(clusters_indirecion_offset, BUF_USAGE(UAV), BUF_STATE(TRANSFER_DST), BUF_SIZE(sizeof(uint)))

  IMPORT_BUF(clustered_lights, import_clustered_lights)

  EXEC(clustered_light_resources)
NODE_END()

NODE_EXEC()
static
void clustered_light_resources(gapi::CmdEncoder& encoder,
                               gapi::BufferHandler clusters_indirecion_offset)
{
  uint indOffset = 0;
  encoder.writeBuffer(clusters_indirecion_offset, &indOffset, 0, sizeof(indOffset));
}

NODE_BEGIN(build_light_clusters)
  CREATE_GPU_BUF(frustum_clusters, BUF_USAGE(UAV), BUF_STATE(UAV_RW), BUF_SIZE(CLUSTERS_NUM * sizeof(Cluster)))
  BIND_SHADER_VAR_AS(frustum_clusters, frustumClusters)
  EXEC(build_light_clusters)
NODE_END()

NODE_EXEC()
static
void build_light_clusters(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("BuildClusters", encoder);
  encoder.updateResources();
  encoder.dispatch(CLUSTERS_DIM_X / CLUSTERS_THREADS_X,
                   CLUSTERS_DIM_Y / CLUSTERS_THREADS_Y,
                   CLUSTERS_DIM_Z / CLUSTERS_THREADS_Z);
}

NODE_BEGIN(clustered_lights_culling)
  BIND_BUF_SRV_AS(frustum_clusters, frustumClusters)
  BIND_BUF_SRV_AS(clustered_lights, clustersLightBuffer)
  BIND_BUF_RW_UAV_AS(clusters_info, clustersInfoBuffer)
  BIND_BUF_RW_UAV_AS(clusters_indirecion, clustersIndirectionBuffer)
  BIND_BUF_RW_UAV_AS(clusters_indirecion_offset, clustersIndirectionBufferNextPos)

  EXEC(clustered_lights_culling)
NODE_END()

NODE_EXEC()
static
void clustered_lights_culling(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("ClusterLightCulling", encoder);

  encoder.updateResources();
  encoder.dispatch(CLUSTERS_DIM_X / CLUSTERS_THREADS_X,
                   CLUSTERS_DIM_Y / CLUSTERS_THREADS_Y,
                   CLUSTERS_DIM_Z / CLUSTERS_THREADS_Z);
}

static bool is_clustered_render_dbg_enabled = true;

NODE_BEGIN(dbg_clustered_render)
  ORDER_ME_BEFORE(ui)

  BIND_BUF_SRV_AS(clusters_info, clustersInfoBuffer)
  MODIFY_TEX_RT_AS(final_target, rt)
  EXEC(dbg_clustered_render_exec)
NODE_END()

NODE_EXEC()
static
void dbg_clustered_render_exec(gapi::CmdEncoder& encoder,
                               const gapi::TextureHandle rt)
{
  if (is_clustered_render_dbg_enabled)
  {
    encoder.beginRenderpass(gapi::RenderTargets{gapi::RenderPassAttachment{rt}}, {});
    tfx::activate_technique("DebugClusters", encoder);
    encoder.updateResources();
    encoder.draw(4,1,0,0);
    encoder.endRenderpass();
  }
}

static
void toggle_clustered_render_dbg(eastl::span<string_view>)
{
  is_clustered_render_dbg_enabled = !is_clustered_render_dbg_enabled;
}

CONSOLE_CMD("render.debug_clusters", 0, 0, toggle_clustered_render_dbg);