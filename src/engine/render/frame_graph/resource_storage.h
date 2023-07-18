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
      struct TextureResource;
      struct SamplerResource;
      using Resource = std::variant<std::monostate, TextureResource, SamplerResource>;

    public:
      void create(const res_id_t, const Registry::Resource&);
      void importTexture(const res_id_t, const gapi::TextureHandle, const gapi::TextureState init_state);
      void transitTextureState(const res_id_t, const gapi::TextureState to_state, gapi::CmdEncoder& encoder);
      
      void setBlobsStorageSize(const size_t);

      auto getBlob(const size_t offset) -> std::byte*;
      auto getTexture(const res_id_t) -> gapi::TextureHandle;
      auto getTextureAndCurrentState(const res_id_t) -> eastl::pair<gapi::TextureHandle, gapi::TextureState>;
      auto getSampler(const res_id_t) -> gapi::SamplerHandler;

      void reset();

    private:
      void createTexture(const res_id_t, const gapi::TextureAllocationDescription&);
      void createSampler(const res_id_t, const gapi::SamplerAllocationDescription&);

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
  };
}
