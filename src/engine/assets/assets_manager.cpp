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

      if (assetType == "static_mesh")
        loadStaticMeshAsset(asset);
    }
  }

  bool AssetsManager::getStaticModel(const string_hash assetUri, StaticModelAsset*& asset)
  {
    const auto it = m_StaticModels.find(assetUri);
    if (it != m_StaticModels.end())
    {
      asset = &it->second;
      return true;
    }

    ASSERT(!"asset not found");

    return false;
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

  void AssetsManager::loadStaticMeshAsset(const DataBlock& asset)
  {
    const string file = asset.getText("bin");
    const string name = asset.getText("name");

    log("asset manager: loading static_mesh: {} as {}", file, name);
    const string_hash nameHash = str_hash(name.c_str());

    StaticModelAsset staticMeshAsset = loadGltf(file);
    Material* m = createMaterial(*asset.getChildBlock("material"));
    staticMeshAsset.material.reset(m);

    m_StaticModels.insert({
        nameHash,
        std::move(staticMeshAsset)
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