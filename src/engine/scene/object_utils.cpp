#include "object_utils.h"
#include "object.h"

#include <glm/gtx/transform.hpp>

namespace Engine
{
  auto calc_object_to_world_tm(const SceneObject& obj) -> float4x4
  {
    const mat4 rotTm = glm::rotate(obj.rot.z, float3{0.0, 0.0, 1.0}) *
                       glm::rotate(obj.rot.y, float3{0.0, 1.0, 0.0}) *
                       glm::rotate(obj.rot.x, float3{1.0, 0.0, 0.0});
    const mat4 scaleTm = glm::scale(obj.scale);
    const mat4 trTm = glm::translate(obj.pos);

    const float4x4 modelTm = trTm * scaleTm * rotTm;

    return modelTm;
  }
}
