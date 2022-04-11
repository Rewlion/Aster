#pragma once

#include <engine/gapi/gapi.h>
#include <engine/materials/materials.h>
#include <engine/materials/storage.h>

#include <EASTL/hash_map.h>

#include <memory>

class DataBlock;

namespace Engine
{
  struct Submesh
  {
    gapi::BufferHandler vertexBuffer;
    gapi::BufferHandler indexBuffer;
    gapi::index_type    indexCount = 0;
  };

  struct StaticModelAsset
  {
    std::unique_ptr<Material> material;
    Utils::FixedStack<Submesh, 32>  submeshes;
  };

  struct TextureAsset
  {
    gapi::TextureHandler texture;
  };

  enum class AssetHandler: uint64_t { Invalid = -1 };

  class AssetsManager
  {
    public:
      void init();

      bool getStaticModel(const string_hash assetUri, StaticModelAsset*& asset);

      bool getTexture(const string_hash assetUri, TextureAsset& asset);

    private:
      void loadAssetsFromFs();
      StaticModelAsset loadGltf(const string& file);
      TextureAsset loadTexture(const string& file);
      void loadTextureAsset(const DataBlock& asset);
      void loadStaticMeshAsset(const DataBlock& asset);
      Material* createMaterial(const DataBlock& matBlk);
    private:
      eastl::hash_map<string_hash, StaticModelAsset> m_StaticModels;
      eastl::hash_map<string_hash, TextureAsset> m_Textures;
  };

  extern AssetsManager assets_manager;
}