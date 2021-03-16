#pragma once

#include <engine/components/static_mesh_component.h>

#include <unordered_map>
#include <string>
#include <optional>
#include <tuple>

namespace Vulkan
{
  class Core;
}

namespace tinygltf
{
  class Model;
}

class AssetStorage
{
public:
  AssetStorage(Vulkan::Core& vkCore);

  inline Vulkan::StaticModel* GetStaticModel(const std::string name)
  {
    const auto it = staticModels.find(name);
    
    if (it != staticModels.end())
      return &it->second;

    return nullptr;
  }

  Vulkan::Image* LoadCubeMap(const std::string& file, const std::string& cubeMapName);

  void LoadTexture(const std::string& file, const std::string& textureName);

  inline Vulkan::Image* GetTexture(const std::string& cubeMapName)
  {
    const auto it = textures.find(cubeMapName);

    if (it != textures.end())
      return &it->second;

    return nullptr;
  }

  Vulkan::StaticModel* LoadModel(const std::string& file, const std::string& modelName);

  void LoadStaticMesh(void* vertexSrc, size_t vertexSrcSize, void* indexSrc, uint32_t indexSrcSize, uint32_t indexCount, const std::string& meshName);
  
  inline Vulkan::StaticMesh* GetStaticMesh(const std::string& meshName)
  {
    const auto it = staticMeshes.find(meshName);

    if (it != staticMeshes.end())
      return &it->second;

    return nullptr;
  }


private:
  void LoadAllTextures(const tinygltf::Model& model, const std::string& rootUri);
  Vulkan::StaticModel AssetStorage::ProcessModel(const tinygltf::Model& model, const std::string& rootUri);

private:
  Vulkan::Core& vkCore;

  std::unordered_map<std::string, Vulkan::StaticMesh> staticMeshes;
  std::unordered_map<std::string, Vulkan::StaticModel> staticModels;
  std::unordered_map<std::string, Vulkan::Image> textures;
  std::unordered_map<std::string, Vulkan::Image> cubeMaps;
};