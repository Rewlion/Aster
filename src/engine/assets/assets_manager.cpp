#include "assets_manager.h"

#include <engine/log.h>
#include <engine/utils/fs.h>

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
          log("asset manager: loading asset `{}`", file);
          LoadAsset(file);
        }
      }
  }

  void AssetsManager::LoadAsset(const string& file)
  {
    if (Utils::CheckFileExt(file, "gltf"))
    {
      LoadGltf(file);
    }
    else
    {
      logerror("asset manager: failed to load `{}` asset: unknown extension.", file);
    }
  }
}