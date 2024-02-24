#pragma once

#include "vertex.h"

#include <engine/gapi/gapi.h>
#include <engine/tfx/tfx.h>

#include <EASTL/hash_map.h>

#include <memory>

namespace ed
{
  class Scope;
}

namespace gapi
{
  class CmdEncoder;
}

namespace Engine
{
  struct CpuSubmesh
  {
    eastl::vector<StaticMeshVertex> vertices;
    eastl::vector<gapi::index_type> indices;
  };

  struct GpuSubmesh
  {
    gapi::BufferHandler       vertexBuffer;
    gapi::BufferHandler       indexBuffer;
    gapi::index_type          indexCount = 0;
  };

  constexpr size_t MAX_SUBMESH_COUNT = 32;

  struct UnpackedStaticMesh
  {
    Utils::FixedStack<CpuSubmesh, MAX_SUBMESH_COUNT> cpuSubmeshes;
  };

  struct StaticMesh
  {
    Utils::FixedStack<GpuSubmesh, MAX_SUBMESH_COUNT> gpuSubmeshes;
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
      auto uploadStaticMeshToGpu(const UnpackedStaticMesh&, gapi::CmdEncoder&) -> StaticMesh;
      UnpackedStaticMesh loadGltf(const string_view file, gapi::CmdEncoder& encoder);
      TextureAsset loadTexture(const string_view file, gapi::CmdEncoder& encoder, const gapi::TextureFormat);
      TextureAsset loadCubeMapTexture(const string_view file, gapi::CmdEncoder& encoder, const gapi::TextureFormat);
      void loadTextureAsset(const ed::Scope& asset, gapi::CmdEncoder& encoder);
      void loadStaticMesh(const ed::Scope& asset, gapi::CmdEncoder& encoder);
      void loadModelAsset(const ed::Scope& asset);
      tfx::Material createMaterial(const ed::Scope& matBlk);
    private:
      eastl::hash_map<string_hash, StaticMesh> m_StaticMeshes;
      eastl::hash_map<string_hash, TextureAsset> m_Textures;
      eastl::hash_map<string_hash, ModelAsset> m_ModelAssets;
  };

  extern AssetsManager assets_manager;
}