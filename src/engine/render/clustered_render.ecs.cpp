#include "clustered_render.h"

#include <engine/debug_marks.h>
#include <engine/ecs/macros.h>
#include <engine/ecs/type_meta.h>
#include <engine/events.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/render/debug/render.h>
#include <engine/tfx/tfx.h>

ECS_DESCRIBE_QUERY(query_clustered_render, Engine::Render::ClusteredRenderer& clustered_render);

ECS_EVENT_SYSTEM()
void point_light_dbg_draw(Engine::OnGameTick&,
                          const float3& pos,
                          const float3& point_light_color,
                          const float point_light_attenuation_radius)
{
  Engine::dbg::draw_line_sphere(pos, point_light_attenuation_radius, point_light_color, 0.0f);
}

ECS_EVENT_SYSTEM()
void listen_point_lights_creation(ecs::OnEntityCreated&,
                                  const float3& pos,
                                  const float3& point_light_color,
                                  const float point_light_attenuation_radius)
{
  query_clustered_render([&](auto& clustered_render){
    clustered_render.addPointLight(PointLight{
      .pos = pos,
      .color = point_light_color,
      .attenuationRadius = point_light_attenuation_radius,
    });
  });
}

namespace Engine::Render
{
  ClusteredRenderer::ClusteredRenderer()
  {
    m_Dirty = false;
  }

  void ClusteredRenderer::buildClusters(gapi::CmdEncoder& encoder)
  {
    GAPI_MARK("build_clusters", encoder);

    const size_t clustersMemSize = CLUSTERS_NUM * sizeof(Cluster);
    m_FrustumClustersBuf.reset(gapi::allocate_buffer(clustersMemSize, gapi::BF_BindUAV | gapi::BF_GpuVisible));

    tfx::set_extern("frustumClusters", m_FrustumClustersBuf);
    tfx::activate_technique("BuildClusters", encoder);
    encoder.updateResources();
    encoder.dispatch(CLUSTERS_DIM_X / CLUSTERS_THREADS_X,
                     CLUSTERS_DIM_Y / CLUSTERS_THREADS_Y,
                     CLUSTERS_DIM_Z / CLUSTERS_THREADS_Z);
    encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_UAV_RW,
                                      gapi::BufferState::BF_STATE_SRV);
  }

  void ClusteredRenderer::cullLights(gapi::CmdEncoder& encoder)
  {
    GAPI_MARK("cull_lights", encoder);

    const size_t clustersInfoMemSize = CLUSTERS_NUM * sizeof(ClusterInfo);
    m_ClustersInfoBuf.reset(gapi::allocate_buffer(clustersInfoMemSize, gapi::BF_BindUAV | gapi::BF_GpuVisible));

    const size_t clusterIndirectionsMemSize = CLUSTERS_NUM * CLUSTER_MAX_ITEMS * sizeof(ClusterIndirection);
    m_IndirectionBuf.reset(gapi::allocate_buffer(clusterIndirectionsMemSize,  gapi::BF_BindUAV | gapi::BF_GpuVisible));

    m_IndirectionOffsetBuf.reset(gapi::allocate_buffer(sizeof(uint), gapi::BF_GpuVisible | gapi::BF_BindUAV));
    uint indOffset = 0;
    encoder.writeBuffer(m_IndirectionOffsetBuf, &indOffset, 0, sizeof(indOffset));
    encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_TRANSFER_DST,
                                      gapi::BufferState::BF_STATE_UAV_RW);

    tfx::set_extern("clustersInfoBuffer", m_ClustersInfoBuf);
    tfx::set_extern("clustersIndirectionBuffer", m_IndirectionBuf);
    tfx::set_extern("clustersIndirectionBufferNextPos", m_IndirectionOffsetBuf);
    tfx::set_extern("clustersLightBuffer", m_LightsBuf);
    tfx::activate_technique("ClusterLightCulling", encoder);

    encoder.updateResources();
    encoder.dispatch(CLUSTERS_DIM_X / CLUSTERS_THREADS_X,
                     CLUSTERS_DIM_Y / CLUSTERS_THREADS_Y,
                     CLUSTERS_DIM_Z / CLUSTERS_THREADS_Z);

    encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_UAV_RW,
                                      gapi::BufferState::BF_STATE_SRV);
  }

  void ClusteredRenderer::beforeRender(gapi::CmdEncoder& encoder)
  {
    buildClusters(encoder);
    if (m_Dirty)
    {
      packLights(encoder);
      m_Dirty = false;
    }
    cullLights(encoder);
  }

  void ClusteredRenderer::addPointLight(PointLight&& pl)
  {
    *reinterpret_cast<PointLight*>(m_PointLights.push_back_uninitialized()) = std::move(pl);
    m_Dirty = true;
  }

  void ClusteredRenderer::packLights(gapi::CmdEncoder& encoder)
  {
    GAPI_MARK("pack_lights", encoder);

    eastl::vector<Light> m_Lights;
    m_Lights.reserve(m_PointLights.size());
    m_Lights.clear();

    const size_t NPointLights = m_PointLights.size();
    size_t lightsNextPos = 0;
    for (const PointLight& pl: m_PointLights)
    {
      Light& l = *reinterpret_cast<Light*>(m_Lights.push_back_uninitialized());
      pack_point_light(pl, m_Lights[lightsNextPos++]);
    }

    const size_t newLightBufSize = m_Lights.size() * sizeof(m_Lights[0]);
    m_LightsBuf.reset(gapi::allocate_buffer(newLightBufSize, gapi::BufferUsage::BF_GpuVisible | gapi::BufferUsage::BF_BindUAV)); //support srv sturctured buffer in compiler

    encoder.writeBuffer(m_LightsBuf, m_Lights.data(), 0, newLightBufSize);
    encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_TRANSFER_DST,
                                      gapi::BufferState::BF_STATE_SRV);
  }

  void render(gapi::CmdEncoder& encoder)
  {

  }

  DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT_WITH_NAME(ClusteredRenderer, "ClusteredRenderer");
}
