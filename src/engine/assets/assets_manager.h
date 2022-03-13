#pragma once


#include <engine/render/gapi/gapi.h>


#include <EASTL/hash_map.h>

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
      void Init();

      bool GetStaticModel(const string_hash assetUri, StaticModelAsset& asset);

      bool GetTexture(const string_hash assetUri, TextureAsset& asset);

    private:
      void LoadAssetsFromFs();
      void LoadAsset(const string& file);
      StaticModelAsset LoadGltf(const string& file);
      TextureAsset LoadTexture(const string& file);
    private:
      eastl::hash_map<string_hash, StaticModelAsset> m_StaticModels;
      eastl::hash_map<string_hash, TextureAsset> m_Textures;
  };

  extern AssetsManager assets_manager;
}