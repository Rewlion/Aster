#pragma once

#include "transform.h"
#include "static_mesh_component.h"

#include <ecs/BaseComponent.h>

namespace Vulkan
{
  class Image;
  struct StaticMesh;

  struct SkyBoxComponent : public BaseComponent
  {
    Transform transform;
    Image* cubeMap;
    StaticMesh* skyboxMesh;
    Material material;
  };
}
