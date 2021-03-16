#include "asset_storage.h"
#include <engine/components/static_mesh_component.h>
#include <engine/rendering/vulkan/core.h>

#include <iostream>
#include <array>
#include <map>
#include <tuple>
#include <stack>
#include <optional>

#include <ktx/ktx.h>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tinygltf/tiny_gltf.h>

namespace
{
  inline std::string GetFolderPath(const std::string& filePath)
  {
    return filePath.substr(0, filePath.find_last_of("\\/"));
  }

  enum class AttributeType
  {
    Index,
    Position,
    UV,
    Normal
  };

  inline std::string GetTypeString(AttributeType type)
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

    AttributeAccessor(const tinygltf::Model& model, const tinygltf::Primitive& primitive, AttributeType type)
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


  std::tuple<std::vector<Vulkan::StaticMeshVertex>, std::vector<uint32_t>> GatherVertices(const tinygltf::Model& model, const tinygltf::Mesh& mesh)
  {
    std::vector<Vulkan::StaticMeshVertex> vertices;
    std::vector<uint32_t> indices;

    for (tinygltf::Primitive primitive : mesh.primitives)
    {
      if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
        throw std::runtime_error("unsupported primitive type.");

      AttributeAccessor posAccessor(model, primitive, AttributeType::Position);
      AttributeAccessor uvAccessor(model, primitive, AttributeType::UV);
      AttributeAccessor indexAccessor(model, primitive, AttributeType::Index);

      for (int i = 0; i < posAccessor.GetCount(); ++i)
      {
        Vulkan::StaticMeshVertex vertex;
        vertex.position = posAccessor++;
        vertex.uv = uvAccessor++;

        vertices.push_back(vertex);
      }

      for (int i = 0; i < indexAccessor.GetCount(); ++i)
        indices.push_back(indexAccessor++);
    }

    /*
      from gltf2 specification(Coordinate System and Units): https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/README.md#coordinate-system-and-units
      glTF uses a right-handed coordinate system, that is, the cross product of +X and +Y yields +Z. 
      glTF defines +Y as up. The front of a glTF asset faces +Z.
      Positive rotation is counterclockwise.

      Engine uses clockwise as positive rotation where -Y is down
      to achieve this one need to invert X and Y for every vertex
      and swap first and last index in the face.
    */

    for (auto& v : vertices)
    {
      v.position.x *= -1.0f;
      v.position.y *= -1.0f;
    }

    for (size_t i = 0; i < indices.size(); i += 3)
      std::swap(indices[i], indices[i + 2]);

    return { std::move(vertices), std::move(indices) };
  }


  std::vector<Vulkan::TBNVectors> GenerateTBNVectors(const std::vector<Vulkan::StaticMeshVertex>& vertices, const std::vector<uint32_t>& indices)
  {
    std::vector<Vulkan::TBNVectors> tbnVectors;
    tbnVectors.resize(vertices.size());

    std::vector<std::vector<Vulkan::TBNVectors>> averageVectors;
    averageVectors.resize(vertices.size());

    for (size_t i = 0; i < indices.size(); i += 3)
    {
      const auto& p0 = vertices[indices[i]];
      const auto& p1 = vertices[indices[i + 1]];
      const auto& p2 = vertices[indices[i + 2]];

      const float dU1 = p1.uv.x - p0.uv.x;
      const float dV1 = p1.uv.y - p0.uv.y;

      const float dU2 = p2.uv.x - p0.uv.x;
      const float dV2 = p2.uv.y - p0.uv.y;

      const glm::vec3 p1p0 = p1.position - p0.position;
      const glm::vec3 p2p0 = p2.position - p0.position;

      glm::mat3x2 edges{
        {p1p0.x, p2p0.x}, {p1p0.y, p2p0.y}, {p1p0.z, p2p0.z}
      };

      const glm::mat2x2 m = glm::inverse(glm::mat2x2{ {dU1, dU2}, {dV1, dV2} });
      const glm::mat3x2 tb = m * edges;

      const glm::vec3 tangent = glm::normalize(glm::vec3{tb[0][0], tb[1][0], tb[2][0]});
      const glm::vec3 bitangent = glm::normalize(glm::vec3{ tb[0][1], tb[1][1], tb[2][1] });
      const glm::vec3 normal = glm::normalize(glm::cross(p2p0, p1p0));

      //each vector inside a face will have the same t b n
      averageVectors[indices[i]].push_back({ tangent, bitangent, normal });
      averageVectors[indices[i+1]].push_back({ tangent, bitangent, normal });
      averageVectors[indices[i+2]].push_back({ tangent, bitangent, normal });
    }

    for (size_t i = 0; i < averageVectors.size(); ++i)
    {
      const std::vector<Vulkan::TBNVectors>& perVertexVectors = averageVectors[i];
      Vulkan::TBNVectors& average = tbnVectors[i];

      for (const auto& vectors : perVertexVectors)
      {
        average.tangent += vectors.tangent;
        average.bitangent += vectors.bitangent;
        average.normal += vectors.normal;
      }

      average.tangent = glm::normalize(average.tangent);
      average.bitangent = glm::normalize(average.bitangent);
      average.normal = glm::normalize(average.normal);
    }

    return tbnVectors;
  }
}

AssetStorage::AssetStorage(Vulkan::Core& vkCore)
  : vkCore(vkCore)
{
}

Vulkan::Image* AssetStorage::LoadCubeMap(const std::string& file, const std::string& cubeMapName)
{
  ktxTexture* texture;
  KTX_error_code result;
  ktx_size_t offset;
  ktx_uint8_t* image;

  result = ktxTexture_CreateFromNamedFile(file.c_str(),
    KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
    &texture);

  if (texture->numFaces != 6)
    throw std::runtime_error("texture is not cube map.");

  std::array<vk::DeviceSize, 6> offsets;
  for (int i = 0; i < 6; ++i)
  {
    ktx_size_t offset;
    result = ktxTexture_GetImageOffset(texture, 0, 0, i, &offset);

    if (result)
      throw std::runtime_error("failed to get texture offset.");

    offsets[i] = offset;
  }

  ktx_uint8_t* src = texture->pData;
  ktx_size_t size = texture->dataSize;

  Vulkan::Image cubeMap = vkCore.AllocateCubeMap(vk::Format::eR8G8B8A8Unorm, texture->pData, texture->dataSize, texture->baseWidth, texture->baseHeight, offsets);

  textures.insert({ cubeMapName, std::move(cubeMap) });

  return &textures.at(cubeMapName);
}

void AssetStorage::LoadTexture(const std::string& file, const std::string& cubeMapName)
{
  ktxTexture* texture;

  const KTX_error_code result = ktxTexture_CreateFromNamedFile(file.c_str(),
    KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
    &texture);

  if (result)
    throw std::runtime_error("failed to load texture");

  textures.insert(
    {
      cubeMapName,
      vkCore.Allocate2DImage(texture->pData, texture->dataSize, vk::Format::eR8G8B8A8Unorm, vk::Extent2D{ texture->baseWidth, texture->baseHeight }, vk::ImageUsageFlagBits::eSampled)
    }
  );
}

Vulkan::StaticModel* AssetStorage::LoadModel(const std::string& file, const std::string& modelName)
{
  const std::string rootUri = GetFolderPath(file);

  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err;
  std::string warn;

  bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, file.c_str());

  if (!warn.empty()) {
    printf("Warn: %s\n", warn.c_str());
  }

  if (!err.empty()) {
    printf("Err: %s\n", err.c_str());
  }

  if (!ret) {
    throw std::runtime_error("Failed to parse glTF");
  }

  LoadAllTextures(model, rootUri);

  Vulkan::StaticModel staticModel = ProcessModel(model, rootUri);
  staticModels.insert({ modelName, std::move(staticModel) });

  return &staticModels.at(modelName);
}

void AssetStorage::LoadAllTextures(const tinygltf::Model& model, const std::string& rootUri)
{
  for (const tinygltf::Image& imgSource : model.images)
  {
    if (imgSource.component != 4 || imgSource.pixel_type != TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
      throw std::runtime_error("only ub rgba textures are supported.");

    const vk::Extent2D imgExtent{ static_cast<uint32_t>(imgSource.width) , static_cast<uint32_t>(imgSource.height) };
    const int pixelSize = imgSource.component;
    const vk::DeviceSize srcSize = pixelSize * imgSource.width * imgSource.height;
    const void* src = reinterpret_cast<const void*>(imgSource.image.data());

    Vulkan::Image img = vkCore.Allocate2DImage(src, srcSize, vk::Format::eR8G8B8A8Unorm, imgExtent, vk::ImageUsageFlagBits::eSampled);

    textures.insert({ rootUri + "/" + imgSource.uri, std::move(img) });
  }
}

Vulkan::StaticModel AssetStorage::ProcessModel(const tinygltf::Model& model, const std::string& rootUri)
{
  Vulkan::StaticModel staticModel;

  for (tinygltf::Mesh mesh : model.meshes)
  {
    const auto [vertices, indices] = GatherVertices(model, mesh);
    const auto tbnVectors = GenerateTBNVectors(vertices, indices);

    Vulkan::Buffer vertexBuffer = vkCore.AllocateDeviceBuffer(vertices.data(), vertices.size() * sizeof(Vulkan::StaticMeshVertex), vk::BufferUsageFlagBits::eVertexBuffer);
    Vulkan::Buffer indexBuffer = vkCore.AllocateDeviceBuffer(indices.data(), indices.size() * sizeof(uint32_t), vk::BufferUsageFlagBits::eIndexBuffer);
    Vulkan::Buffer tbnVectorsBuffer = vkCore.AllocateDeviceBuffer(tbnVectors.data(), tbnVectors.size() * sizeof(Vulkan::TBNVectors), vk::BufferUsageFlagBits::eVertexBuffer);

    Vulkan::Material material;
    const tinygltf::Material& gltfMaterial = model.materials[0];

    const std::string colorTextureName = rootUri + "/" + model.images[gltfMaterial.pbrMetallicRoughness.baseColorTexture.index].uri;
    material.colorTexture = &textures.at(colorTextureName);

    const std::string metallicRoughnessTextureName = rootUri + "/" + model.images[gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index].uri;
    material.metallicRoughnessTexture = &textures.at(metallicRoughnessTextureName);

    const std::string normalTextureName = rootUri + "/" + model.images[gltfMaterial.normalTexture.index].uri;
    material.normalTexture = &textures.at(normalTextureName);

    staticModel.meshes.push_back(
      Vulkan::StaticMesh{
        std::move(vertexBuffer),
        std::move(indexBuffer),
        std::move(tbnVectorsBuffer),
        static_cast<uint32_t>(indices.size())
      }
    );
    staticModel.materials.push_back(material);
  }

  return std::move(staticModel);
}

void AssetStorage::LoadStaticMesh(void* vertexSrc, size_t vertexSrcSize, void* indexSrc, uint32_t indexSrcSize, uint32_t indexCount, const std::string& meshName)
{
  Vulkan::Buffer vertexBuffer = vkCore.AllocateDeviceBuffer(vertexSrc, vertexSrcSize, vk::BufferUsageFlagBits::eVertexBuffer);
  Vulkan::Buffer indexBuffer = vkCore.AllocateDeviceBuffer(indexSrc, indexSrcSize, vk::BufferUsageFlagBits::eIndexBuffer);

  staticMeshes.insert({
    meshName,
    Vulkan::StaticMesh{
      std::move(vertexBuffer),
      std::move(indexBuffer),
      Vulkan::Buffer{},
      static_cast<uint32_t>(indexCount)
    }
    });
}
