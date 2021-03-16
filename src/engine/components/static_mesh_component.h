#pragma once

#include <engine/components/transform.h>
#include <engine/rendering/vulkan/buffer.h>
#include <engine/rendering/vulkan/vertex.h>
#include <engine/rendering/vulkan/image.h>

#include <ecs/BaseComponent.h>

#include <vector>

namespace Vulkan
{
  struct StaticMesh
  {
    Buffer vertices;
    Buffer indices;
    Buffer tbnVectorsBuffer;
    unsigned int indexCount = 0;
  };

  struct Material
  {
    Image* colorTexture = nullptr;
    Image* metallicRoughnessTexture = nullptr;
    Image* normalTexture = nullptr;
  };

  struct StaticModel
  {
    std::vector<StaticMesh> meshes;
    std::vector<Material> materials;
  };

  struct StaticMeshComponent : public BaseComponent
  {
    Transform transform;
    StaticModel* model;
  };
}