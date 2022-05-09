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
    gapi::BufferHandler       vertexBuffer;
    gapi::BufferHandler       indexBuffer;
    gapi::index_type          indexCount = 0;
  };

  constexpr size_t MAX_SUBMESH_COUNT = 32;

  struct StaticMesh
  {
    Utils::FixedStack<Submesh, MAX_SUBMESH_COUNT> submeshes;
  };

  struct ModelAsset
  {
    StaticMesh* mesh;
    Material* materials[MAX_SUBMESH_COUNT];
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

      ModelAsset* getModel(const string& assetName);

      bool getTexture(const string_hash assetUri, TextureAsset& asset);

    private:
      void loadAssetsFromFs();
      StaticMesh loadGltf(const string& file);
      TextureAsset loadTexture(const string& file);
      void loadTextureAsset(const DataBlock& asset);
      void loadStaticMesh(const DataBlock& asset);
      void loadModelAsset(const DataBlock& asset);
      Material* createMaterial(const DataBlock& matBlk);
    private:
      eastl::hash_map<string_hash, StaticMesh> m_StaticMeshes;
      eastl::hash_map<string_hash, TextureAsset> m_Textures;
      eastl::hash_map<string_hash, ModelAsset> m_ModelAssets;
  };

  extern AssetsManager assets_manager;
}