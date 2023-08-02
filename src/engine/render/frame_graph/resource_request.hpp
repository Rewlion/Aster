#pragma once

#include "id.h"

#include "timeline.h"

#include <engine/gapi/resources.h>

namespace fg
{
  template<class Accessor>
  class ResourceRequest
  {
      friend class Registry;
    public:
      ResourceRequest(virt_res_id_t id, const Timeline timeline = Timeline::Current)
        : m_Id(id)
        , m_Timeline(timeline)
      {
      }

      auto get() const -> typename Accessor::return_type
      {
        return Accessor::access(m_Id, m_Timeline);
      }
      auto describe() const -> typename Accessor::alloc_desc_return_type
      {
        return Accessor::getAllocDescription(m_Id);
      }

      auto operator->() const
        -> typename std::remove_reference<typename Accessor::return_type>::type*
      {
        return &get();
      }

    private:
      virt_res_id_t m_Id;
      Timeline m_Timeline;
  };

  class TextureAccessor
  {
    public:
      using alloc_desc_return_type = const gapi::TextureAllocationDescription&;
      using return_type = const gapi::TextureHandle;

      static
      auto access(const virt_res_id_t id, const Timeline) -> return_type;

      static
      auto getAllocDescription(const virt_res_id_t id) -> alloc_desc_return_type;
  };
  
  class SamplerAccessor
  {
    public:
      using alloc_desc_return_type = const gapi::SamplerAllocationDescription&;
      using return_type = const gapi::SamplerHandler;

      static
      auto access(const virt_res_id_t id, const Timeline) -> return_type;

      static
      auto getAllocDescription(const virt_res_id_t id) -> alloc_desc_return_type;
  };

  class BufferAccessor
  {
    public:
      using alloc_desc_return_type = const gapi::BufferAllocationDescription&;
      using return_type = const gapi::BufferHandler;

      static
      auto access(const virt_res_id_t id, const Timeline) -> return_type;

      static
      auto getAllocDescription(const virt_res_id_t id) -> alloc_desc_return_type;
  };

  class BlobAccessorInternal
  {
    template<class BlobType, class Access>
    friend class BlobAccessor;

    static
    auto getBlob(const virt_res_id_t id) -> std::byte*;
  };

  template<class BlobType, class Access>
  class BlobAccessor
  {
    public:
      using alloc_desc_return_type = void;
      using return_type = typename Access::return_type;

      static
      auto access(const virt_res_id_t id, const Timeline) -> return_type
      {
        std::byte* storage = BlobAccessorInternal::getBlob(id);
        return storage != nullptr ?
          reinterpret_cast<return_type>(*storage) : 
          getDefault();
      }

      static
      auto getAllocDescription(const virt_res_id_t id) -> alloc_desc_return_type {}
      
    private:
      static
      auto getDefault() -> return_type
      {
        static typename std::remove_cvref<return_type>::type def;
        return def;
      }
  };

  template<class T>
  struct BlobRAccess { using return_type = const T&; };
  template<class T>
  struct BlobRWAccess { using return_type = T&; };

  using TextureRequest = ResourceRequest<TextureAccessor>;
  using SamplerRequest = ResourceRequest<SamplerAccessor>;
  using BufferRequest  = ResourceRequest<BufferAccessor>;

  template<class BlobType>
  using BlobReadRequest = ResourceRequest<BlobAccessor<BlobType, BlobRAccess<BlobType>>>;
  template<class BlobType>
  using BlobReadWriteRequest = ResourceRequest<BlobAccessor<BlobType, BlobRWAccess<BlobType>>>;
}
