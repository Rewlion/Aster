#include "assets_manager.h"

#include <engine/algorithm/hash.h>
#include <engine/log.h>
#include <engine/utils/fs.h>

#include <algorithm>
#include <filesystem>

namespace Engine
{
  AssetsManager assets_manager;

  void AssetsManager::Init()
  {
    LoadAssetsFromFs();
  }

  void AssetsManager::LoadAssetsFromFs()
  {
      const std::filesystem::path assetsDir{"bin/assets"};

      for (const auto& rootDirEntry : std::filesystem::directory_iterator{assetsDir})
      {
        if (!rootDirEntry.is_directory())
          continue;

        for (const auto& assetDirEntry : std::filesystem::directory_iterator{rootDirEntry})
        {
          if (!assetDirEntry.is_regular_file())
            continue;

          std::string file = assetDirEntry.path().string();
          std::replace(file.begin(), file.end(), '\\', '/');

          log("asset manager: loading asset `{}`", file);
          LoadAsset(file);
        }
      }
  }

  void AssetsManager::LoadAsset(const string& file)
  {
    const string_hash fileHash = str_hash(file.c_str());
    if (m_StaticModels.find(fileHash) != m_StaticModels.end())
    {
      logerror("asset manager: failed to load new asset. `{}:{}` has hash collision with already loaded file.", file.c_str(), fileHash);
      return;
    }

    if (Utils::CheckFileExt(file, "gltf"))
    {
      StaticModelAsset asset = LoadGltf(file);

      m_StaticModels.insert({
        fileHash,
        asset
      });
    }
    else
    {
      logerror("asset manager: failed to load `{}` asset: unknown extension.", file);
    }
  }

  bool AssetsManager::GetStaticModel(const string_hash assetUri, StaticModelAsset& asset)
  {
    const auto it = m_StaticModels.find(assetUri);
    if (it != m_StaticModels.end())
    {
      asset = it->second;
      return true;
    }

    return false;
  }
}