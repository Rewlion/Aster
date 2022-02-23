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

  enum class AssetHandler: uint64_t { Invalid = -1 };

  class AssetsManager
  {
    public:
      void Init();

      bool GetStaticModel(const string_hash assetUri, StaticModelAsset& asset);

    private:
      void LoadAssetsFromFs();
      void LoadAsset(const string& file);
      StaticModelAsset LoadGltf(const string& file);
    private:
      eastl::hash_map<string_hash, StaticModelAsset> m_StaticModels;
  };

  extern AssetsManager assets_manager;
}