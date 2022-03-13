#include "assets_manager.h"

#include <engine/assert.h>
#include <engine/log.h>
#include <engine/render/gapi/gapi.h>

#include <ktx/ktx.h>

namespace Engine
{
  TextureAsset AssetsManager::LoadTexture(const string& file)
  {
    ktxTexture* texture;

    const KTX_error_code error = ktxTexture_CreateFromNamedFile(
      file.c_str(),
      KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
      &texture);

    if (error)
    {
      logerror("failed to load texture {}", file);
      return {};
    }

    TextureAsset asset;
    asset.texture = gapi::AllocateTexture(
      gapi::TextureAllocationDescription{
        .format = gapi::TextureFormat::R8G8B8A8_UNORM,
        .extent = int3{texture->baseWidth, texture->baseHeight, 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage = gapi::TextureUsage::Uniform
      }
    );

    gapi::CopyToTextureSync(texture->pData, texture->dataSize, asset.texture);

    ktxTexture_Destroy(texture);

    return asset;
  }
}
