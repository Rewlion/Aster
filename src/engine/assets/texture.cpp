#include "assets_manager.h"

#include <engine/assert.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/log.h>

#include <ktx/ktx.h>

namespace Engine
{
  TextureAsset AssetsManager::loadTexture(const string& file, gapi::CmdEncoder& encoder)
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
    asset.texture = allocate_texture(
      gapi::TextureAllocationDescription{
        .format = gapi::TextureFormat::R8G8B8A8_UNORM,
        .extent = int3{texture->baseWidth, texture->baseHeight, 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage = gapi::TextureUsage::TEX_USAGE_UNIFORM
      }
    );

    encoder.transitTextureState(asset.texture, gapi::TextureState::Undefined, gapi::TextureState::TransferDst);
    encoder.copyBufferToTexture(asset.texture, texture->pData, texture->dataSize);
    encoder.transitTextureState(asset.texture, gapi::TextureState::TransferDst, gapi::TextureState::ShaderRead);

    ktxTexture_Destroy(texture);

    return asset;
  }
}
