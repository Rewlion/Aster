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

  struct TextureResource
  {
    std::string_view name;
    gapi::TextureAllocationDescription allocDesc;
    gapi::TextureHandler handle;
    bool isImported;
  };

  struct BufferResource
  {
    std::string_view name;
    gapi::BufferAllocationDescription allocDesc;
    gapi::BufferHandler handle;
    bool isImported;
  };

  using Resource = std::variant<TextureResource, BufferResource>;
}
