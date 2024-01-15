#include "assets_manager.h"

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/log.h>
#include <engine/settings.h>
#include <engine/utils/fs.h>

#include <algorithm>
#include <filesystem>
#include <memory>

namespace Engine
{
  AssetsManager assets_manager;

  void AssetsManager::init()
  {
    loadAssetsFromFs();
  }

  void AssetsManager::loadAssetsFromFs()
  {
    std::unique_ptr<gapi::CmdEncoder> encoder{gapi::allocate_cmd_encoder()};

    const ed::Scope& assets = get_app_settings().getScope("assets");
    for (const ed::Scope& asset: assets.getScopes())
    {
      const string_view assetType = asset.getName();

      if (assetType == "texture")
        loadTextureAsset(asset, *encoder);
      else if (assetType == "static_mesh")
        loadStaticMesh(asset, *encoder);
      else if (assetType == "model_asset")
        loadModelAsset(asset);
    }

    std::unique_ptr<gapi::Fence> fence{gapi::allocate_fence()};
    encoder->flush(fence.get());
    fence->wait();
  }

  ModelAsset* AssetsManager::getModel(const string& assetName)
  {
    const string_hash assetNameHash = str_hash(assetName.c_str());

    const auto it = m_ModelAssets.find(assetNameHash);
    if (it != m_ModelAssets.end())
      return &it->second;

    ASSERT(!"asset not found");

    return nullptr;
  }

  bool AssetsManager::getTexture(const string_hash assetUri, TextureAsset& asset)
  {
    const auto it = m_Textures.find(assetUri);
    if (it != m_Textures.end())
    {
      asset = it->second;
      return true;
    }

    ASSERT_FMT(false, "asset {} not found", assetUri);

    return false;
  }

  auto AssetsManager::getTexture(const string& assetUri, TextureAsset& asset) -> bool
  {
    return getTexture(str_hash(assetUri), asset);
  }

  static
  auto get_texture_asset_format(const ed::Scope& asset) -> gapi::TextureFormat
  {
    const string_view format = asset.getVariable<string_view>("format");
    return format == "r16" ? gapi::TextureFormat::R16_UNORM
                           : gapi::TextureFormat::R8G8B8A8_UNORM;
  }

  void AssetsManager::loadTextureAsset(const ed::Scope& asset, gapi::CmdEncoder& encoder)
  {
    const bool isCubeMap = asset.getVariableOr<bool>("cubemap", false);
    const string_view file = asset.getVariable<string_view>("bin");
    const string_view name = asset.getVariable<string_view>("name");
    const gapi::TextureFormat format = get_texture_asset_format(asset);

    loginfo("asset manager: loading texture: {} as {}", file, name);
    const string_hash nameHash = str_hash(name);

    TextureAsset textureAsset = isCubeMap ?
                                  loadCubeMapTexture(file,encoder, format) :
                                  loadTexture(file, encoder, format);

      m_Textures.insert({
        nameHash,
        textureAsset
      });
  }

  void AssetsManager::loadStaticMesh(const ed::Scope& asset, gapi::CmdEncoder& encoder)
  {
    const string_view file = asset.getVariable<string_view>("bin");
    const string_view name = asset.getVariable<string_view>("name");

    loginfo("asset manager: loading static_mesh: {} as {}", file, name);
    const string_hash nameHash = str_hash(name);

    StaticMesh staticMeshAsset = loadGltf(file, encoder);
    m_StaticMeshes.insert({
      nameHash,
      std::move(staticMeshAsset)
    });
  }

  void AssetsManager::loadModelAsset(const ed::Scope& asset)
  {
    const string_view name = asset.getVariable<string_view>("name");
    const string_view mesh = asset.getVariable<string_view>("mesh");
    const string_hash meshHash = str_hash(mesh);

    loginfo("asset manager: loading model_asset: {}", name);

    const auto meshIt = m_StaticMeshes.find(meshHash);
    ASSERT(meshIt != m_StaticMeshes.end());

    ModelAsset model;
    model.mesh = &meshIt->second;

    for (const ed::Scope& material: asset.getScopes())
    {
      if (material.getName() != "material")
        continue;
      tfx::Material m = createMaterial(material);
      model.materials.push_back(std::move(m));
    }

    m_ModelAssets.insert({
      str_hash(name),
      std::move(model)
    });
  }

  tfx::Material AssetsManager::createMaterial(const ed::Scope& mat)
  {
    tfx::Material material;
    material.technique = mat.getAnnotation();

    for (const ed::Variable& var: mat.getVariables())
    {
      if (var.getValueType() != ed::ValueType::Text || var.annotation != "texture")
        continue;

      const string_view textureName = std::get<string>(var.value);
      TextureAsset asset;
      bool r = getTexture(str_hash(textureName), asset);
      ASSERT(r != false);

      material.params.push_back(tfx::ParamDescription{
        .name = var.name,
        .value = asset.texture
      });
    }

    return material;
  }
}