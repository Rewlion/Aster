#pragma once

#include "mesh.h"

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
      auto uploadStaticMeshToGpu(const UnpackedStaticMesh&, gapi::CmdEncoder&) -> SubmeshStack<GpuSubmesh>;
      auto createBVHForSubmeshes(const UnpackedStaticMesh&) -> SubmeshStack<BVH>;
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