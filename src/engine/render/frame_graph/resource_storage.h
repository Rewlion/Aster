#pragma once

#include "id.h"
#include "registry.h"

#include <engine/utils/resizable_vector.hpp>

#include <EASTL/vector.h>
#include <EASTL/utility.h>

#include <variant>

namespace fg
{
  class ResourceStorage
  {
      friend class PersistentResourceStorage;

      struct TextureResource;
      struct SamplerResource;
      struct BufferResource;
      using Resource = std::variant<std::monostate, TextureResource, SamplerResource, BufferResource>;

    public:
      void create(const res_id_t, const Registry::Resource&);
      void importTexture(const res_id_t, const gapi::TextureHandle, const gapi::TextureState init_state);
      void transitTextureState(const res_id_t, const gapi::TextureState to_state, gapi::CmdEncoder& encoder);
      void transitBufferState(const res_id_t, const gapi::BufferState to_state, gapi::CmdEncoder& encoder);
      
      void setBlobsStorageSize(const size_t);

      auto getBlob(const size_t offset) -> std::byte*;
      auto getTexture(const res_id_t) -> gapi::TextureHandle;
      auto getTextureAndCurrentState(const res_id_t) -> eastl::pair<gapi::TextureHandle, gapi::TextureState>;
      auto getSampler(const res_id_t) -> gapi::SamplerHandler;
      auto getBuffer(const res_id_t) -> gapi::BufferHandler;

      void reset();

    private:
      void freeResource(Resource&);

      void createTexture(const res_id_t, const gapi::TextureAllocationDescription&);
      void createSampler(const res_id_t, const gapi::SamplerAllocationDescription&);
      void createBuffer(const res_id_t, const gapi::BufferAllocationDescription&, const gapi::BufferState);

    private:
      Utils::ResizableVector<Resource, res_id_t> m_Resources;
      eastl::vector<std::byte> m_Blobs;

    private:
      struct TextureResource
      {
        gapi::TextureHandle texture = gapi::TextureHandle::Invalid;
        bool imported = false;
        gapi::TextureState currentState = gapi::TextureState::Undefined;
      };

      struct SamplerResource
      {
        gapi::SamplerHandler sampler = gapi::SamplerHandler::Invalid;
      };

      struct BufferResource
      {
        gapi::BufferHandler buffer = gapi::BufferHandler::Invalid;
        gapi::BufferState currentState = (gapi::BufferState)0;
      };
  };

  class PersistentResourceStorage
  {
    public:
      void importResTo(const res_id_t, ResourceStorage&);
      void create(const res_id_t, const Registry::Resource&);
      void reset() { m_Storage.reset(); }
    private:

      template<class T>
      static
      auto hasAllocDescChanged(const Registry::Resource& prev,
                               const Registry::Resource& cur)
      {
        if (const auto* prevBufInfo = std::get_if<T>(&prev))
        {
          const auto& oldAllocDesc = prevBufInfo->allocDesc;
          const auto& newAllocDesc = std::get<T>(cur).allocDesc;

          return std::memcmp(&oldAllocDesc, &newAllocDesc, sizeof(newAllocDesc)) > 0;
        }
        return false;
      }

    private:

      ResourceStorage m_Storage;
      Utils::ResizableVector<Registry::Resource, res_id_t> m_CreateInfos;
  };
}
