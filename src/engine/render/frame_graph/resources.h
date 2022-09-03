#pragma once

#include "handles.h"

#include <engine/gapi/resources.h>

namespace fg
{
  class Node;

  struct VirtualResource
  {
    ResourceHandle resourceId = ResourceHandle::Invalid;
    uint32_t version = 0;

    Node* producer;
    Node* lastAccessor;
  };

  struct TextureResourceView
  {
    const std::string_view& name;
    const gapi::TextureHandler& handle;
    const gapi::TextureAllocationDescription& desc;
  };

  struct TextureResource
  {
    TextureResource(const std::string_view name,
                    const gapi::TextureAllocationDescription& alloc_desc,
                    const gapi::TextureHandler h,
                    const gapi::TextureState current_state,
                    const bool is_imported);
    TextureResource(TextureResource&& rvl);
    ~TextureResource();

    const TextureResource& operator=(const TextureResource&) = delete;
    TextureResource& operator=(TextureResource&&);

    TextureResourceView getView() const;

    std::string_view name;
    gapi::TextureAllocationDescription allocDesc;
    gapi::TextureHandler handle;
    gapi::TextureState currentState;
    bool isImported;
  };

  struct BufferResource
  {
    BufferResource(const std::string_view name,
                    const gapi::BufferAllocationDescription& alloc_desc,
                    const gapi::BufferHandler h,
                    const bool is_imported);
    BufferResource(BufferResource&& rvl);
    ~BufferResource();

    const BufferResource& operator=(const BufferResource&) = delete;
    BufferResource& operator=(BufferResource&&);

    std::string_view name;
    gapi::BufferAllocationDescription allocDesc;
    gapi::BufferHandler handle;
    bool isImported;
  };

  using Resource = std::variant<TextureResource, BufferResource>;
}
