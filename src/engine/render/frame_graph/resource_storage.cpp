#include "resource_storage.h"

#include <engine/assert.h>
#include <engine/gapi/gapi.h>
#include <engine/gapi/cmd_encoder.h>

namespace fg
{
  void ResourceStorage::create(const res_id_t res_id, const Registry::Resource& res_info)
  {
    if (auto* texRes = std::get_if<Registry::TextureResource>(&res_info))
    {
      if (texRes->producer)
      {
        const auto [tex, initState] = texRes->producer();
        importTexture(res_id, tex, initState);
      }
      else
        createTexture(res_id, texRes->allocDesc);
    }
    else if (auto* samplerRes = std::get_if<Registry::SamplerResource>(&res_info))
    {
      createSampler(res_id, samplerRes->allocDesc);
    }
    else if (auto* bufferRes = std::get_if<Registry::BufferResource>(&res_info))
    {
      createBuffer(res_id, bufferRes->allocDesc, bufferRes->initState);
    }
    else if (auto* blobRes = std::get_if<Registry::BlobResource>(&res_info))
    {
      std::byte* storage = m_Blobs.data() + blobRes->bufferStart;
      blobRes->constructor(storage);
    }
  }

  void ResourceStorage::createTexture(const res_id_t res_id, const gapi::TextureAllocationDescription& alloc_desc)
  {
    Resource& res = m_Resources[res_id];
    ASSERT(std::holds_alternative<std::monostate>(res));

    res = TextureResource{
      .texture = gapi::allocate_texture(alloc_desc),
      .imported = false,
      .currentState = gapi::TextureState::Undefined
    };
  }

  void ResourceStorage::createSampler(const res_id_t res_id, const gapi::SamplerAllocationDescription& alloc_desc)
  {
    Resource& res = m_Resources[res_id];
    ASSERT(std::holds_alternative<std::monostate>(res));

    res = SamplerResource{
      .sampler = gapi::allocate_sampler(alloc_desc)
    };
  }

  void ResourceStorage::createBuffer(const res_id_t res_id,
                                     const gapi::BufferAllocationDescription& alloc_desc,
                                     const gapi::BufferState init_state)
  {
    Resource& res = m_Resources[res_id];
    ASSERT(std::holds_alternative<std::monostate>(res));

    res = BufferResource{
      .buffer = gapi::allocate_buffer(alloc_desc.size, alloc_desc.usage, alloc_desc.name),
      .currentState = init_state
    };
  }

  void ResourceStorage::importTexture(const res_id_t res_id, const gapi::TextureHandle h, const gapi::TextureState init_state)
  {
    Resource& res = m_Resources[res_id];
    ASSERT(std::holds_alternative<std::monostate>(res));

    res = TextureResource{
      .texture = h,
      .imported = true,
      .currentState = init_state
    };
  }

  void ResourceStorage::transitTextureState(const res_id_t res_id, const gapi::TextureState to_state, gapi::CmdEncoder& encoder)
  {
    TextureResource& tex = std::get<TextureResource>(m_Resources[res_id]);
    if (tex.currentState != to_state)
    {
      encoder.transitTextureState(tex.texture, tex.currentState, to_state);
      tex.currentState = to_state;
    }
  }

  void ResourceStorage::transitBufferState(const res_id_t res_id, const gapi::BufferState to_state, gapi::CmdEncoder& encoder)
  {
    BufferResource& buf = std::get<BufferResource>(m_Resources[res_id]);
    if (buf.currentState != to_state)
    {
      encoder.insertGlobalBufferBarrier(buf.currentState, to_state);
      buf.currentState = to_state;
    }
  }

  void ResourceStorage::setBlobsStorageSize(const size_t size)
  {
    m_Blobs.resize(size);
  }

  auto ResourceStorage::getBlob(const size_t offset) -> std::byte*
  {
    return m_Blobs.data() + offset;
  }

  auto ResourceStorage::getTexture(const res_id_t res_id) -> gapi::TextureHandle
  {
    return std::get<TextureResource>(m_Resources[res_id]).texture;
  }

  auto ResourceStorage::getTextureAndCurrentState(const res_id_t res_id)
    -> eastl::pair<gapi::TextureHandle, gapi::TextureState>
  {
    const auto& tex = std::get<TextureResource>(m_Resources[res_id]);
    return {tex.texture, tex.currentState};
  }

  auto ResourceStorage::getSampler(const res_id_t res_id) -> gapi::SamplerHandler
  {
    return std::get<SamplerResource>(m_Resources[res_id]).sampler;
  }

  auto ResourceStorage::getBuffer(const res_id_t res_id) -> gapi::BufferHandler
  {
    return std::get<BufferResource>(m_Resources[res_id]).buffer;
  }

  void ResourceStorage::reset()
  {
    for (auto& res: m_Resources)
    {
      if (auto* tex = std::get_if<TextureResource>(&res))
      {
        if (!tex->imported && tex->texture != gapi::TextureHandle::Invalid)
          gapi::free_resource(tex->texture);
      }
      else if (auto* smp = std::get_if<SamplerResource>(&res))
      {
        if (smp->sampler != gapi::SamplerHandler::Invalid)
          gapi::free_resource(smp->sampler);
      }
      else if (auto* buf = std::get_if<BufferResource>(&res))
      {
        if (buf->buffer != gapi::BufferHandler::Invalid)
          gapi::free_resource(buf->buffer);
      }
    }

    m_Resources.clear();
    m_Blobs.clear();
  }
}
