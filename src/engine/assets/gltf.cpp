#include "assets_manager.h"

#include <engine/assert.h>
#include <engine/gapi/cmd_encoder.h>
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

  inline string get_type_string(const AttributeType type)
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
        : primitive.attributes.at(get_type_string(type));

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

    inline int getCount() const
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
    float3 normal    = {0,0,0};
    float2 uv        = {0,0};
  };

  std::tuple<std::vector<MeshVertex>, std::vector<gapi::index_type>>
    gather_vertices(const tinygltf::Model& model, const tinygltf::Mesh& mesh)
  {
    std::vector<MeshVertex> vertices;
    std::vector<gapi::index_type> indices;

    size_t verticesCount = 0;
    for (tinygltf::Primitive primitive : mesh.primitives)
    {
      if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
        throw std::runtime_error("unsupported primitive type.");

      AttributeAccessor posAccessor(model, primitive, AttributeType::Position);
      verticesCount += posAccessor.getCount();
    }

    vertices.reserve(verticesCount);
    indices.reserve(verticesCount);

    for (tinygltf::Primitive primitive : mesh.primitives)
    {
      AttributeAccessor posAccessor(model, primitive, AttributeType::Position);
      AttributeAccessor normalAccessor(model, primitive, AttributeType::Normal);
      AttributeAccessor uvAccessor(model, primitive, AttributeType::UV);
      AttributeAccessor indexAccessor(model, primitive, AttributeType::Index);

      for (int i = 0; i < posAccessor.getCount(); ++i)
      {
        MeshVertex vertex;
        vertex.pos = posAccessor++;
        vertex.normal = normalAccessor++;
        vertex.uv = uvAccessor++;

        vertices.push_back(vertex);
      }

      for (int i = 0; i < indexAccessor.getCount(); ++i)
        indices.push_back(indexAccessor++);
    }

    return {vertices, indices};
  }
}

namespace Engine
{
  static StaticMesh process_model(const tinygltf::Model& model, gapi::CmdEncoder& encoder)
  {
    StaticMesh asset;

    for (tinygltf::Mesh mesh : model.meshes)
    {
      auto [vertices, indices] = gather_vertices(model, mesh);

      gapi::BufferAllocationDescription vbAlloc;
      const size_t verticesSize = vertices.size() * sizeof(vertices[0]);
      const size_t indicesSize = indices.size() * sizeof(indices[0]);

      Submesh submesh;
      submesh.vertexBuffer = gapi::allocate_buffer(verticesSize, gapi::BF_GpuVisible | gapi::BF_BindVertex);
      submesh.indexBuffer  = gapi::allocate_buffer(indicesSize,  gapi::BF_GpuVisible | gapi::BF_BindIndex);

      encoder.writeBuffer(submesh.vertexBuffer, vertices.data(), 0, verticesSize);
      encoder.writeBuffer(submesh.indexBuffer, indices.data(), 0, indicesSize);
      submesh.indexCount = indices.size();

      asset.submeshes.push(std::move(submesh));
    }

    return asset;
  }

  StaticMesh AssetsManager::loadGltf(const string& file, gapi::CmdEncoder& encoder)
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

    StaticMesh asset = process_model(model, encoder);
    return asset;
  }
}
