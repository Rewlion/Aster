#include "assets_manager.h"

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
#include <engine/log.h>
#include <engine/settings.h>
#include <engine/utils/fs.h>
#include <engine/materials/materials.h>
#include <engine/materials/storage.h>

#include <algorithm>
#include <filesystem>

namespace Engine
{
  AssetsManager assets_manager;

  void AssetsManager::init()
  {
    loadAssetsFromFs();
  }

  void AssetsManager::loadAssetsFromFs()
  {
    DataBlock* assets = get_app_settings()->getChildBlock("assets");
    for (const DataBlock& asset: assets->getChildBlocks())
    {
      const string assetType = asset.getName();

      if (assetType == "texture")
        loadTextureAsset(asset);
      else if (assetType == "static_mesh")
        loadStaticMesh(asset);
      else if (assetType == "model_asset")
        loadModelAsset(asset);
    }
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

    ASSERT(!"asset not found");

    return false;
  }

  void AssetsManager::loadTextureAsset(const DataBlock& asset)
  {
    const string file = asset.getText("bin");
    const string name = asset.getText("name");

    log("asset manager: loading texture: {} as {}", file, name);
    const string_hash nameHash = str_hash(name.c_str());

    TextureAsset textureAsset = loadTexture(file);
      m_Textures.insert({
        nameHash,
        textureAsset
      });
  }

  void AssetsManager::loadStaticMesh(const DataBlock& asset)
  {
    const string file = asset.getText("bin");
    const string name = asset.getText("name");

    log("asset manager: loading static_mesh: {} as {}", file, name);
    const string_hash nameHash = str_hash(name.c_str());

    StaticMesh staticMeshAsset = loadGltf(file);
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

    log("asset manager: loading model_asset: {}", name);

    const auto meshIt = m_StaticMeshes.find(meshHash);
    ASSERT(meshIt != m_StaticMeshes.end());

    ModelAsset model;
    model.mesh = &meshIt->second;

    Utils::FixedStack<const DataBlock*, MAX_SUBMESH_COUNT> materials;
    for (const DataBlock& material: asset.getChildBlocks())
      if (material.getName() == "material")
        materials.push(&material);

    ASSERT(materials.getSize() == model.mesh->submeshes.getSize());

    for (size_t i = 0; i < model.mesh->submeshes.getSize(); ++i)
      model.materials[i] = createMaterial(*materials.get(i));

    m_ModelAssets.insert({
      str_hash(name.c_str()),
      std::move(model)
    });
  }

  Material* AssetsManager::createMaterial(const DataBlock& matBlk)
  {
    const string matName = matBlk.getAnnotation();
    Material* material = MaterialsStorage::constructMaterial(matName);

    Material::Params matParams;

    for (const auto& param: matBlk.getChildBlocks())
    {
      if (param.getName() == "texture")
      {
        const string textureName = param.getText("name");
        TextureAsset texture;
        bool r = getTexture(str_hash(textureName.c_str()), texture);
        ASSERT(r != false);

        Material::Param matParam;
        matParam.name = param.getText("binding");
        matParam.type = Material::BindingType::Texture;
        matParam.handler = (gapi::ResourceHandler)texture.texture;

        matParams.push(matParam);
        continue;
      }

      ASSERT(!"unsupported type");
    }

    material->addParams(matParams);
    return material;
  }
}