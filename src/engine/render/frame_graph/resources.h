#pragma once

#include "handles.h"

#include <engine/gapi/resource_wrappers.h>

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

  struct TextureResource
  {
    std::string_view name;
    gapi::TextureAllocationDescription allocDesc;
    gapi::TextureWrapper handle;
    bool isImported;
  };

  struct BufferResource
  {
    std::string_view name;
    gapi::BufferAllocationDescription allocDesc;
    gapi::BufferWrapper handle;
    bool isImported;
  };

  using Resource = std::variant<TextureResource, BufferResource>;
}
