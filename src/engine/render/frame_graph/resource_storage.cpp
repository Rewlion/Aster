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
        gapi::TextureViewWithState* texView = texRes->producer();
        importTexture(res_id, texView);
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
        gapi::BufferViewWithState* bufView = bufferRes->producer();
        importBuffer(res_id, bufView);
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
      .texture = gapi::TextureViewWithState{
        gapi::allocate_texture(alloc_desc),
        gapi::TextureState::Undefined
      },
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
      .buffer = gapi::BufferViewWithState{
        gapi::allocate_buffer(alloc_desc.size, alloc_desc.usage, alloc_desc.name),
        init_state
      },
      .imported = false
    };
  }

  void ResourceStorage::importTextureTmp(const res_id_t res_id, gapi::TextureViewWithState tex_view)
  {
    Resource& res = m_Resources[res_id];
    ASSERT(std::holds_alternative<std::monostate>(res));

    res = TextureResource{
      .texture = tex_view,
      .imported = true
    };
  }

  void ResourceStorage::importTexture(const res_id_t res_id, gapi::TextureViewWithState* tex_view)
  {
    Resource& res = m_Resources[res_id];
    ASSERT(std::holds_alternative<std::monostate>(res));

    res = TextureResource{
      .texture = tex_view,
      .imported = true
    };
  }

  void ResourceStorage::importBufferTmp(const res_id_t res_id, gapi::BufferViewWithState buf_view)
  {
    Resource& res = m_Resources[res_id];
    ASSERT(std::holds_alternative<std::monostate>(res));

    res = BufferResource{
      .buffer = buf_view,
      .imported = true
    };
  }

  void ResourceStorage::importBuffer(const res_id_t res_id, gapi::BufferViewWithState* buf_view)
  {
    Resource& res = m_Resources[res_id];
    ASSERT(std::holds_alternative<std::monostate>(res));

    res = BufferResource{
      .buffer = buf_view,
      .imported = true
    };
  }

  void ResourceStorage::transitTextureState(const res_id_t res_id, const gapi::TextureState to_state, gapi::CmdEncoder& encoder)
  {
    TextureResource& res = std::get<TextureResource>(m_Resources.get(res_id));
    gapi::TextureViewWithState& texView = res.unwrap();

    const bool isSameState = texView.getState() == to_state;
    const bool isModificationState = gapi::is_modification_state(to_state);

    if (!isSameState || isModificationState)
      texView.transitState(encoder,to_state);
  }

  void ResourceStorage::transitBufferState(const res_id_t res_id, const gapi::BufferState to_state, gapi::CmdEncoder& encoder)
  {
    BufferResource& buf = std::get<BufferResource>(m_Resources.get(res_id));
    gapi::BufferViewWithState& bufView = buf.unwrap();

    const bool isSameState = bufView.getState() == to_state;
    const bool isModificationState = gapi::is_modification_state(to_state);
    
    if (!isSameState || isModificationState)
      bufView.transitState(encoder, to_state);
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
    return std::get<TextureResource>(m_Resources.get(res_id)).unwrap();
  }

  auto ResourceStorage::getTextureAndCurrentState(const res_id_t res_id)
    -> gapi::TextureViewWithState
  {
    const auto& res = std::get<TextureResource>(m_Resources.get(res_id));
    return res.unwrap();
  }

  auto ResourceStorage::accessTextureAndCurrentState(const res_id_t res_id)
    -> gapi::TextureViewWithState&
  {
    auto& res = std::get<TextureResource>(m_Resources.get(res_id));
    return res.unwrap();
  }

  auto ResourceStorage::getSampler(const res_id_t res_id) -> gapi::SamplerHandler
  {
    return std::get<SamplerResource>(m_Resources.get(res_id)).sampler;
  }

  auto ResourceStorage::getBuffer(const res_id_t res_id) -> gapi::BufferHandler
  {
    return std::get<BufferResource>(m_Resources.get(res_id)).unwrap();
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
      gapi::TextureViewWithState& texView = tex->unwrap();
      if (!tex->imported && texView != gapi::TextureHandle::Invalid)
        gapi::free_resource(texView);
    }
    else if (auto* smp = std::get_if<SamplerResource>(&res))
    {
      if (smp->sampler != gapi::SamplerHandler::Invalid)
        gapi::free_resource(smp->sampler);
    }
    else if (auto* buf = std::get_if<BufferResource>(&res))
    {
      gapi::BufferViewWithState& bufView = buf->unwrap();
      if (!buf->imported && bufView != gapi::BufferHandler::Invalid)
        gapi::free_resource(bufView);
    }
    res = std::monostate{};
  }

  void PersistentResourceStorage::importResTo(const res_id_t res_id, ResourceStorage& to)
  {
    if (auto* tex = std::get_if<ResourceStorage::TextureResource>(&m_Storage.m_Resources[res_id]))
    {
      gapi::TextureViewWithState& texView = tex->unwrap();
      ASSERT(texView != gapi::TextureHandle::Invalid);
      to.importTextureTmp(res_id, texView);
    }
    else if (auto* buf = std::get_if<ResourceStorage::BufferResource>(&m_Storage.m_Resources[res_id]))
    {
      gapi::BufferViewWithState& bufView = buf->unwrap();
      ASSERT(bufView != gapi::BufferHandler::Invalid);
      to.importBufferTmp(res_id, bufView);
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

  void PersistentResourceStorage::updateTexturesStateFrom(const ResourceStorage& from)
  {
    const res_id_t resourcesCount = (res_id_t)m_Storage.m_Resources.size();
    for (res_id_t i{0}; i < resourcesCount; ++i)
    {
      if (auto* persistentTex = std::get_if<ResourceStorage::TextureResource>(&m_Storage.m_Resources.get(i)))
      {
        const auto& updatedTex = std::get<ResourceStorage::TextureResource>(from.m_Resources.get(i));
        persistentTex->texture = updatedTex.unwrap();
      }
    }
  }
}
