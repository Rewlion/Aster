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

    DataBlock* assets = get_app_settings()->getChildBlock("assets");
    for (const DataBlock& asset: assets->getChildBlocks())
    {
      const string assetType = asset.getName();

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
  auto get_texture_asset_format(const DataBlock& asset) -> gapi::TextureFormat
  {
    const string format = asset.getText("format");
    return format == "r16" ? gapi::TextureFormat::R16_UNORM
                           : gapi::TextureFormat::R8G8B8A8_UNORM;
  }

  void AssetsManager::loadTextureAsset(const DataBlock& asset, gapi::CmdEncoder& encoder)
  {
    const bool isCubeMap = asset.getBool("cubemap", false);
    const string file = asset.getText("bin");
    const string name = asset.getText("name");
    const gapi::TextureFormat format = get_texture_asset_format(asset);

    loginfo("asset manager: loading texture: {} as {}", file, name);
    const string_hash nameHash = str_hash(name.c_str());

    TextureAsset textureAsset = isCubeMap ?
                                  loadCubeMapTexture(file,encoder, format) :
                                  loadTexture(file, encoder, format);

      m_Textures.insert({
        nameHash,
        textureAsset
      });
  }

  void AssetsManager::loadStaticMesh(const DataBlock& asset, gapi::CmdEncoder& encoder)
  {
    const string file = asset.getText("bin");
    const string name = asset.getText("name");

    loginfo("asset manager: loading static_mesh: {} as {}", file, name);
    const string_hash nameHash = str_hash(name.c_str());

    StaticMesh staticMeshAsset = loadGltf(file, encoder);
    m_StaticMeshes.insert({
      nameHash,
      std::move(staticMeshAsset)
    });
  }

  void AssetsManager::loadModelAsset(const DataBlock& asset)
  {
    const string name = asset.getText("name");
    const string mesh = asset.getText("mesh");
    const string_hash meshHash = str_hash(mesh.c_str());

    loginfo("asset manager: loading model_asset: {}", name);

    const auto meshIt = m_StaticMeshes.find(meshHash);
    ASSERT(meshIt != m_StaticMeshes.end());

    ModelAsset model;
    model.mesh = &meshIt->second;

    for (const auto& material: asset.getChildBlocks())
    {
      if (material.getName() != "material")
        continue;
      tfx::Material m = createMaterial(material);
      model.materials.push_back(std::move(m));
    }

    m_ModelAssets.insert({
      str_hash(name.c_str()),
      std::move(model)
    });
  }

  tfx::Material AssetsManager::createMaterial(const DataBlock& matBlk)
  {
    tfx::Material material;
    material.technique = matBlk.getAnnotation();

    for (const auto& attribute: matBlk.getAttributes())
    {
      if (attribute.type != DataBlock::Attribute::Type::Text || attribute.annotation != "texture")
        continue;

      const string textureName = std::get<string>(attribute.as);
      TextureAsset asset;
      bool r = getTexture(str_hash(textureName.c_str()), asset);
      ASSERT(r != false);

      material.params.push_back(tfx::ParamDescription{
        .name = attribute.name,
        .value = asset.texture
      });
    }

    return material;
  }
}