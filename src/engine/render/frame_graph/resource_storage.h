#pragma once

#include "id.h"
#include "registry.h"

#include <engine/gapi/resource_wrappers.h>
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
      void importTextureTmp(const res_id_t, gapi::TextureViewWithState);
      void importTexture(const res_id_t, gapi::TextureViewWithState*);
      void importBufferTmp(const res_id_t, gapi::BufferViewWithState);
      void importBuffer(const res_id_t, gapi::BufferViewWithState*);
      void transitTextureState(const res_id_t, const gapi::TextureState to_state, gapi::CmdEncoder& encoder);
      void transitBufferState(const res_id_t, const gapi::BufferState to_state, gapi::CmdEncoder& encoder);
      
      void setBlobsStorageSize(const size_t);

      auto getBlob(const size_t offset) -> std::byte*;
      auto getTexture(const res_id_t) -> gapi::TextureHandle;
      auto getTextureAndCurrentState(const res_id_t) -> gapi::TextureViewWithState;
      auto accessTextureAndCurrentState(const res_id_t) -> gapi::TextureViewWithState&;
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
        std::variant<
          gapi::TextureViewWithState,
          gapi::TextureViewWithState*> texture;
        bool imported = false;

        auto unwrap() -> gapi::TextureViewWithState&
        {
          if (std::holds_alternative<gapi::TextureViewWithState>(texture))
            return std::get<gapi::TextureViewWithState>(texture);
          return *std::get<gapi::TextureViewWithState*>(texture);
        }

        auto unwrap() const -> const gapi::TextureViewWithState&
        {
          return const_cast<TextureResource*>(this)->unwrap();
        }
      };

      struct SamplerResource
      {
        gapi::SamplerHandler sampler = gapi::SamplerHandler::Invalid;
      };

      struct BufferResource
      {
        std::variant<
          gapi::BufferViewWithState,
          gapi::BufferViewWithState*> buffer;

         auto unwrap() -> gapi::BufferViewWithState&
        {
          if (std::holds_alternative<gapi::BufferViewWithState>(buffer))
            return std::get<gapi::BufferViewWithState>(buffer);
          return *std::get<gapi::BufferViewWithState*>(buffer);
        }

        auto unwrap() const -> const gapi::BufferViewWithState&
        {
          return const_cast<BufferResource*>(this)->unwrap();
        }

        bool imported = false;
      };
  };

  class PersistentResourceStorage
  {
    public:
      void importResTo(const res_id_t, ResourceStorage&);
      void create(const res_id_t, const Registry::Resource&);
      void updateTexturesStateFrom(const ResourceStorage&);
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
