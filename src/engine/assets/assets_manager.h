#pragma once

#include <engine/gapi/gapi.h>
#include <engine/tfx/tfx.h>

#include <EASTL/hash_map.h>

#include <memory>

class DataBlock;

namespace gapi
{
  class CmdEncoder;
}

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
    eastl::vector<tfx::Material> materials;
  };

  struct TextureAsset
  {
    gapi::TextureHandle texture;
  };

  enum class AssetHandler: uint64_t { Invalid = uint64_t(~0) };

  class AssetsManager
  {
    public:
      void init();

      ModelAsset* getModel(const string& assetName);

      bool getTexture(const string_hash assetUri, TextureAsset& asset);
      auto getTexture(const string& assetUri, TextureAsset& asset) -> bool;

    private:
      void loadAssetsFromFs();
      StaticMesh loadGltf(const string& file, gapi::CmdEncoder& encoder);
      TextureAsset loadTexture(const string& file, gapi::CmdEncoder& encoder, const gapi::TextureFormat);
      TextureAsset loadCubeMapTexture(const string& file, gapi::CmdEncoder& encoder, const gapi::TextureFormat);
      void loadTextureAsset(const DataBlock& asset, gapi::CmdEncoder& encoder);
      void loadStaticMesh(const DataBlock& asset, gapi::CmdEncoder& encoder);
      void loadModelAsset(const DataBlock& asset);
      tfx::Material createMaterial(const DataBlock& matBlk);
    private:
      eastl::hash_map<string_hash, StaticMesh> m_StaticMeshes;
      eastl::hash_map<string_hash, TextureAsset> m_Textures;
      eastl::hash_map<string_hash, ModelAsset> m_ModelAssets;
  };

  extern AssetsManager assets_manager;
}