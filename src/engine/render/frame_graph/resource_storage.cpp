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
      if (bufferRes->producer)
      {
        const auto [buf, initState] = bufferRes->producer();
        importBuffer(res_id, buf, initState);
      }
      else
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
      .currentState = gapi::TextureState::Undefined,
      .imported = false
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
      .currentState = init_state,
      .imported = false
    };
  }

  void ResourceStorage::importTexture(const res_id_t res_id, const gapi::TextureHandle h, const gapi::TextureState init_state)
  {
    Resource& res = m_Resources[res_id];
    ASSERT(std::holds_alternative<std::monostate>(res));

    res = TextureResource{
      .texture = h,
      .currentState = init_state,
      .imported = true
    };
  }

  void ResourceStorage::importBuffer(const res_id_t res_id, const gapi::BufferHandler h, const gapi::BufferState init_state)
  {
    Resource& res = m_Resources[res_id];
    ASSERT(std::holds_alternative<std::monostate>(res));

    res = BufferResource{
      .buffer = h,
      .currentState = init_state,
      .imported = true
    };
  }

  void ResourceStorage::transitTextureState(const res_id_t res_id, const gapi::TextureState to_state, gapi::CmdEncoder& encoder)
  {
    TextureResource& tex = std::get<TextureResource>(m_Resources.get(res_id));
    if (tex.currentState != to_state)
    {
      encoder.transitTextureState(tex.texture, tex.currentState, to_state);
      tex.currentState = to_state;
    }
  }

  void ResourceStorage::transitBufferState(const res_id_t res_id, const gapi::BufferState to_state, gapi::CmdEncoder& encoder)
  {
    BufferResource& buf = std::get<BufferResource>(m_Resources.get(res_id));
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
    return std::get<TextureResource>(m_Resources.get(res_id)).texture;
  }

  auto ResourceStorage::getTextureAndCurrentState(const res_id_t res_id)
    -> eastl::pair<gapi::TextureHandle, gapi::TextureState>
  {
    const auto& tex = std::get<TextureResource>(m_Resources.get(res_id));
    return {tex.texture, tex.currentState};
  }

  auto ResourceStorage::getSampler(const res_id_t res_id) -> gapi::SamplerHandler
  {
    return std::get<SamplerResource>(m_Resources.get(res_id)).sampler;
  }

  auto ResourceStorage::getBuffer(const res_id_t res_id) -> gapi::BufferHandler
  {
    return std::get<BufferResource>(m_Resources.get(res_id)).buffer;
  }

  void ResourceStorage::reset()
  {
    for (auto& res: m_Resources)
      freeResource(res);

    m_Resources.clear();
    m_Blobs.clear();
  }

  void ResourceStorage::freeResource(Resource& res)
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
      if (!buf->imported && buf->buffer != gapi::BufferHandler::Invalid)
        gapi::free_resource(buf->buffer);
    }
    res = std::monostate{};
  }

  void PersistentResourceStorage::importResTo(const res_id_t res_id, ResourceStorage& to)
  {
    if (auto* tex = std::get_if<ResourceStorage::TextureResource>(&m_Storage.m_Resources[res_id]))
    {
      ASSERT(tex->texture != gapi::TextureHandle::Invalid);
      to.importTexture(res_id, tex->texture, tex->currentState);
    }
    else if (auto* buf = std::get_if<ResourceStorage::BufferResource>(&m_Storage.m_Resources[res_id]))
    {
      ASSERT(buf->buffer != gapi::BufferHandler::Invalid);
      to.importBuffer(res_id, buf->buffer, buf->currentState);
    }
  }

  void PersistentResourceStorage::create(const res_id_t res_id, const Registry::Resource& res_info)
  {
    ResourceStorage::Resource& res = m_Storage.m_Resources[res_id];
    if (std::holds_alternative<std::monostate>(res))
    {
      m_Storage.create(res_id, res_info);
      m_CreateInfos[res_id] = res_info;
    }
    else
    {
      Registry::Resource& savedInfo = m_CreateInfos[res_id];
      if (hasAllocDescChanged<Registry::BufferResource>(res_info, savedInfo) ||
          hasAllocDescChanged<Registry::TextureResource>(res_info, savedInfo))
      {
        m_Storage.freeResource(res);
        m_Storage.create(res_id, res_info);
        savedInfo = res_info;
      }
    }

  }
}
