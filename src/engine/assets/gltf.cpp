#pragma once

#include "assets_manager.h"

#include <engine/assert.h>
#include <engine/log.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tinygltf/tiny_gltf.h>

#include <tuple>

namespace
{
  enum class AttributeType
  {
    Index,
    Position,
    UV,
    Normal
  };

  inline string GetTypeString(const AttributeType type)
  {
    switch (type)
    {
      case AttributeType::Index:
        return "__INDEX";
      case AttributeType::Position:
        return "POSITION";
      case AttributeType::UV:
        return "TEXCOORD_0";
      case AttributeType::Normal:
        return "NORMAL";
      default:
        return "UNKNOWN";
    }
  }

  class AttributeAccessor
  {
  public:

    AttributeAccessor(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const AttributeType type)
    {
      const int accessorIndex = type == AttributeType::Index
        ? primitive.indices
        : primitive.attributes.at(GetTypeString(type));

      const tinygltf::Accessor& accessor = model.accessors[accessorIndex];

      const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];

      componentType = accessor.componentType;

      buf = reinterpret_cast<const float*>(model.buffers[bufferView.buffer].data.data() + bufferView.byteOffset + accessor.byteOffset);

      stride = accessor.ByteStride(bufferView);
      if (stride == -1)
        throw std::runtime_error("failed to calculate stride.");

      byteOffset = accessor.byteOffset + bufferView.byteOffset;

      count = accessor.count;

      i = 0;
    }

    AttributeAccessor(int componentType, const float* buf, int stride, int byteOffset, int count, int i)
      : componentType(componentType)
      , buf(buf)
      , stride(stride)
      , byteOffset(byteOffset)
      , count(count)
      , i(i)
    {
    }

    inline int GetCount() const
    {
      return count;
    }

    AttributeAccessor operator++(int)
    {
      AttributeAccessor it{ componentType, buf, stride, byteOffset,count,i };
      ++i;

      return it;
    }

    operator glm::vec3() const
    {
      const int typedStride = stride / sizeof(float);
      return glm::vec3{ buf[i * typedStride + 0], buf[i * typedStride + 1], buf[i * typedStride + 2] };
    }

    operator glm::vec2() const
    {
      const int typedStride = stride / sizeof(float);
      return glm::vec2{ buf[i * typedStride + 0], buf[i * typedStride + 1] };
    }

    operator uint32_t() const
    {
      switch (componentType)
      {
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
      {
        return reinterpret_cast<const unsigned char*>(buf)[i];
      }

      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
      {
        return reinterpret_cast<const unsigned short*>(buf)[i];
      }

      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
      {
        return reinterpret_cast<const unsigned int*>(buf)[i];
      }

      default:
        throw std::runtime_error("unsupported index type.");
      }
    }

  private:
    int componentType;
    const float* buf;
    int stride;
    int byteOffset;
    int count;
    int i;
  };

  struct MeshVertex
  {
    float3 pos       = {0,0,0};
    float3 tangent   = {0,0,0};
    float3 bitangent = {0,0,0};
    float2 uv        = {0,0};
  };

  std::tuple<std::vector<MeshVertex>, std::vector<gapi::index_type>>
    GatherVertices(const tinygltf::Model& model, const tinygltf::Mesh& mesh)
  {
    std::vector<MeshVertex> vertices;
    std::vector<gapi::index_type> indices;

    size_t verticesCount = 0;
    for (tinygltf::Primitive primitive : mesh.primitives)
    {
      if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
        throw std::runtime_error("unsupported primitive type.");

      AttributeAccessor posAccessor(model, primitive, AttributeType::Position);
      verticesCount += posAccessor.GetCount();
    }

    vertices.reserve(verticesCount);
    indices.reserve(verticesCount);

    for (tinygltf::Primitive primitive : mesh.primitives)
    {
      AttributeAccessor posAccessor(model, primitive, AttributeType::Position);
      AttributeAccessor uvAccessor(model, primitive, AttributeType::UV);
      AttributeAccessor indexAccessor(model, primitive, AttributeType::Index);

      for (int i = 0; i < posAccessor.GetCount(); ++i)
      {
        MeshVertex vertex;
        vertex.pos = posAccessor++;
        vertex.uv = uvAccessor++;

        vertices.push_back(vertex);
      }

      for (int i = 0; i < indexAccessor.GetCount(); ++i)
        indices.push_back(indexAccessor++);
    }

    return {vertices, indices};
  }
}

namespace Engine
{
  static void GenerateTBVectors(std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices)
  {
    for (size_t i = 0; i < indices.size(); i += 3)
    {
      const auto& p0 = vertices[indices[i]];
      const auto& p1 = vertices[indices[i + 1]];
      const auto& p2 = vertices[indices[i + 2]];

      const float dU1 = p1.uv.x - p0.uv.x;
      const float dV1 = p1.uv.y - p0.uv.y;

      const float dU2 = p2.uv.x - p0.uv.x;
      const float dV2 = p2.uv.y - p0.uv.y;

      const float3 p1p0 = p1.pos - p0.pos;
      const float3 p2p0 = p2.pos - p0.pos;

      glm::mat3x2 edges{
        {p1p0.x, p2p0.x}, {p1p0.y, p2p0.y}, {p1p0.z, p2p0.z}
      };

      const glm::mat2x2 m = glm::inverse(glm::mat2x2{ {dU1, dU2}, {dV1, dV2} });
      const glm::mat3x2 tb = m * edges;

      const float3 tangent = glm::normalize(float3{tb[0][0], tb[1][0], tb[2][0]});
      const float3 bitangent = glm::normalize(float3{ tb[0][1], tb[1][1], tb[2][1] });
      const float3 normal = glm::normalize(glm::cross(p2p0, p1p0));

      vertices[indices[i]].tangent     += tangent;
      vertices[indices[i]].bitangent   += bitangent;

      vertices[indices[i+1]].tangent   += tangent;
      vertices[indices[i+1]].bitangent += bitangent;

      vertices[indices[i+2]].tangent   += tangent;
      vertices[indices[i+2]].bitangent += bitangent;
    }

    for (size_t i = 0; i < indices.size(); i += 3)
    {
      vertices[indices[i]].tangent     = glm::normalize(vertices[indices[i]].tangent);
      vertices[indices[i]].bitangent   = glm::normalize(vertices[indices[i]].bitangent);

      vertices[indices[i+1]].tangent   = glm::normalize(vertices[indices[i+1]].tangent);
      vertices[indices[i+1]].bitangent = glm::normalize(vertices[indices[i+1]].bitangent);

      vertices[indices[i+2]].tangent   = glm::normalize(vertices[indices[i+2]].tangent);
      vertices[indices[i+2]].bitangent = glm::normalize(vertices[indices[i+2]].bitangent);
    }
  }

  static StaticModelAsset ProcessModel(const tinygltf::Model& model)
  {
    StaticModelAsset asset;

    for (tinygltf::Mesh mesh : model.meshes)
    {
      auto [vertices, indices] = GatherVertices(model, mesh);
      GenerateTBVectors(vertices, indices);

      gapi::BufferAllocationDescription vbAlloc;
      vbAlloc.size = vertices.size() * sizeof(vertices[0]);
      vbAlloc.usage = gapi::BufferUsage::Vertex;

      gapi::BufferAllocationDescription ibAlloc;
      ibAlloc.size = indices.size() * sizeof(indices[0]);
      ibAlloc.usage = gapi::BufferUsage::Index;

      Submesh submesh;
      submesh.vertexBuffer = gapi::AllocateBuffer(vbAlloc);
      submesh.indexBuffer  = gapi::AllocateBuffer(ibAlloc);

      gapi::CopyToBufferSync(vertices.data(), 0, vertices.size() * sizeof(vertices[0]), submesh.vertexBuffer);
      gapi::CopyToBufferSync(indices.data(), 0, indices.size() * sizeof(indices[0]), submesh.indexBuffer);
      submesh.indexCount = indices.size();

      asset.submeshes.Push(submesh);
    }

    return asset;
  }

  StaticModelAsset AssetsManager::LoadGltf(const string& file)
  {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    string err;
    string warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, file.c_str());

    if (!warn.empty())
      logwarn("asset manager: failed to load `{}` :{}", file, warn);

    if (!err.empty())
      logerror("asset manager: `{}` {}", file, err);

    if (!ret)
      ASSERT(!"asset manager: failed to load asset");

    StaticModelAsset asset = ProcessModel(model);
    return asset;
  }
}