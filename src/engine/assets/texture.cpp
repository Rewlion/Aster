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
        .usage = gapi::TextureUsage::TEX_USAGE_SRV
      }
    );

    encoder.transitTextureState(asset.texture, gapi::TextureState::Undefined, gapi::TextureState::TransferDst);
    encoder.copyBufferToTexture(asset.texture, texture->pData, texture->dataSize);
    encoder.transitTextureState(asset.texture, gapi::TextureState::TransferDst, gapi::TextureState::ShaderRead);

    ktxTexture_Destroy(texture);

    return asset;
  }

  TextureAsset AssetsManager::loadCubeMapTexture(const string& file, gapi::CmdEncoder& encoder)
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

    ASSERT_FMT(texture->numFaces == 6,
      "{} is not a cubemap: it has {} layers but 6 is expected.",
      file, texture->numFaces);

    TextureAsset asset;
    asset.texture = allocate_texture(
      gapi::TextureAllocationDescription{
        .format = gapi::TextureFormat::R8G8B8A8_UNORM,
        .extent = int3{texture->baseWidth, texture->baseHeight, 1},
        .mipLevels = 1,
        .arrayLayers = 6,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage = gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_CUBE_MAP
      }
    );

    gapi::BufferTextureCopy copies[6];
    for (size_t i = 0; i < 6; ++i)
    {
      ktx_size_t offset;
      const KTX_error_code result = ktxTexture_GetImageOffset(texture, 0, 0, i, &offset);

      ASSERT_FMT(result == 0, "failed to get cubemap layer({}) offset for {}", i, file);

      auto& copy = copies[i];
      copy.bufferOffset = offset;
      copy.textureSubresource.aspects = gapi::ASPECT_COLOR;
      copy.textureSubresource.mipLevel = 0;
      copy.textureSubresource.baseArrayLayer = i;
      copy.textureSubresource.layerCount = 1;
      copy.extent = {texture->baseWidth, texture->baseHeight, 1};
    }

    encoder.transitTextureState(asset.texture, gapi::TextureState::Undefined, gapi::TextureState::TransferDst);
    encoder.copyBufferToTexture(texture->pData, texture->dataSize, asset.texture, copies, 6);
    encoder.transitTextureState(asset.texture, gapi::TextureState::TransferDst, gapi::TextureState::ShaderRead);

    ktxTexture_Destroy(texture);

    return asset;
  }
}
