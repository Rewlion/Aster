#pragma once

#include "vertex_input_declaration.h"

namespace Vulkan
{
  struct TBNVectors
  {
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec3 normal;
  };

  struct StaticMeshVertex
  {
    glm::vec3 position;
    glm::vec2 uv;

    static inline VertexInputDeclaration GetVID()
    {
      VertexInputDeclaration vid;

      vid.AddBindingDescription(0, sizeof(StaticMeshVertex));
      vid.AddAttributeDescription(vk::Format::eR32G32B32Sfloat, 0, 0, offsetof(StaticMeshVertex, position));
      vid.AddAttributeDescription(vk::Format::eR32G32Sfloat, 0, 1, offsetof(StaticMeshVertex, uv));

      vid.AddBindingDescription(1, sizeof(TBNVectors));
      vid.AddAttributeDescription(vk::Format::eR32G32B32Sfloat, 1, 2, offsetof(TBNVectors, tangent));
      vid.AddAttributeDescription(vk::Format::eR32G32B32Sfloat, 1, 3, offsetof(TBNVectors, bitangent));
      vid.AddAttributeDescription(vk::Format::eR32G32B32Sfloat, 1, 4, offsetof(TBNVectors, normal));

      return vid;
    }
  };

  struct SkyBoxVertex
  {
    glm::vec3 position;

    static inline VertexInputDeclaration GetVID()
    {
      VertexInputDeclaration vid;
      vid.AddBindingDescription(0, sizeof(SkyBoxVertex));
      vid.AddAttributeDescription(vk::Format::eR32G32B32Sfloat, 0, 0, offsetof(SkyBoxVertex, position));

      return vid;
    }
  };

}