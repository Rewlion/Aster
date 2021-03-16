#pragma once

#include <engine/rendering/vulkan/buffer.h>

#include "mesh/halfedge_mesh.h"

namespace Editor
{
  class Scene
  {
  public:
    class Object
    {
    public:
      Object(const uint64_t id);

      HalfedgeMesh& GetMesh()
      {
        return m_Mesh;
      }

      inline void UpdateBuffers(Vulkan::Buffer&& vertexBuffer, Vulkan::Buffer&& indexBuffer, uint32_t indexCount)
      {
        m_VertexBuffer = std::move(vertexBuffer);
        m_IndexBuffer = std::move(indexBuffer);
        m_IndexCount = indexCount;
      }

      inline uint64_t GetId() const
      {
        return m_Id;
      }

      std::tuple<vk::Buffer, vk::Buffer, uint32_t> GetStaticMeshBuffers() const
      {
        return { m_VertexBuffer.GetBuffer(), m_IndexBuffer.GetBuffer(), m_IndexCount };
      }

    private:
      uint64_t m_Id;
      HalfedgeMesh m_Mesh;
      Vulkan::Buffer m_VertexBuffer;
      Vulkan::Buffer m_IndexBuffer;
      uint32_t m_IndexCount;
    };

    inline Object& AddNewObject()
    {
      m_Objects.push_back({m_FreeId++});
      return m_Objects.back();
    }

    inline std::vector<Object>& GetObjects()
    {
      return m_Objects;
    }

    inline const std::vector<Object>& GetObjects() const
    {
      return m_Objects;
    }

  private:
    uint64_t m_FreeId = 0;
    std::vector<Object> m_Objects;
  };
}