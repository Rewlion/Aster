#include "clustered_lights.h"

#include <engine/components/point_light.h>
#include <engine/debug_marks.h>
#include <engine/ecs/macros.h>
#include <engine/ecs/type_meta.h>
#include <engine/events.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/render/debug/render.h>
#include <engine/tfx/tfx.h>

ECS_DESCRIBE_QUERY(query_clustered_lights, Engine::Render::ClusteredLights& clustered_lights);

ECS_EVENT_SYSTEM()
void point_light_dbg_draw(Engine::OnGameTick&,
                          const PointLightComponent& point_light)
{
  Engine::dbg::draw_line_sphere(point_light.getWorldPosition(), point_light.getAttenuationRadius(), point_light.getColor(), 0.0f);
}

ECS_EVENT_SYSTEM()
void listen_point_lights_creation(ecs::OnEntityCreated&,
                                  const PointLightComponent& point_light)
{
  query_clustered_lights([&](auto& clustered_lights){
    clustered_lights.addPointLight(PointLight{
      .pos = point_light.getWorldPosition(),
      .color = point_light.getColor(),
      .attenuationRadius = point_light.getAttenuationRadius(),
    });
  });
}

namespace Engine::Render
{
  ClusteredLights::ClusteredLights()
  {
    m_Dirty = false;
  }

  void ClusteredLights::beforeRender(gapi::CmdEncoder& encoder)
  {
    if (m_Dirty)
    {
      packLights(encoder);
      m_Dirty = false;
    }
  }

  void ClusteredLights::addPointLight(PointLight&& pl)
  {
    *reinterpret_cast<PointLight*>(m_PointLights.push_back_uninitialized()) = std::move(pl);
    m_Dirty = true;
  }

  void ClusteredLights::packLights(gapi::CmdEncoder& encoder)
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

  DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT_WITH_NAME(ClusteredLights, "ClusteredLights");
}
