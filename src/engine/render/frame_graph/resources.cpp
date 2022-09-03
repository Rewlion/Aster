#include "resources.h"

#include <engine/gapi/gapi.h>

namespace fg
{
  TextureResource::TextureResource(const std::string_view name,
                    const gapi::TextureAllocationDescription& alloc_desc,
                    const gapi::TextureHandle h,
                    const gapi::TextureState current_state,
                    const bool is_imported)
    : name(name)
    , allocDesc(alloc_desc)
    , handle(h)
    , currentState(current_state)
    , isImported(is_imported)
  {
  }

  TextureResource::TextureResource(TextureResource&& rvl)
    : name(rvl.name)
    , allocDesc(rvl.allocDesc)
    , handle(rvl.handle)
    , currentState(rvl.currentState)
    , isImported(rvl.isImported)
  {
    this->~TextureResource();
    rvl.handle = gapi::TextureHandle::Invalid;
  }

  TextureResource& TextureResource::operator=(TextureResource&& rvl)
  {
    this->~TextureResource();

    name = rvl.name;
    allocDesc = rvl.allocDesc;
    handle = rvl.handle;
    currentState = rvl.currentState;
    isImported = rvl.isImported;

    rvl.handle = gapi::TextureHandle::Invalid;
    return *this;
  }

  TextureResourceView TextureResource::getView() const
  {
    return TextureResourceView{.name = name, .handle = handle, .desc = allocDesc};
  }

  TextureResource::~TextureResource()
  {
    if (!isImported && handle != gapi::TextureHandle::Invalid)
    {
      gapi::free_resource(handle);
      handle = gapi::TextureHandle::Invalid;
    }
  }

  BufferResource::BufferResource(const std::string_view name,
                    const gapi::BufferAllocationDescription& alloc_desc,
                    const gapi::BufferHandler h,
                    const bool is_imported)
    : name(name)
    , allocDesc(alloc_desc)
    , handle(h)
    , isImported(is_imported)
  {
  }

  BufferResource::BufferResource(BufferResource&& rvl)
    : name(rvl.name)
    , allocDesc(rvl.allocDesc)
    , handle(rvl.handle)
    , isImported(rvl.isImported)
  {
    this->~BufferResource();
    rvl.handle = gapi::BufferHandler::Invalid;
  }

  BufferResource& BufferResource::operator=(BufferResource&& rvl)
  {
    this->~BufferResource();

    name = rvl.name;
    allocDesc = rvl.allocDesc;
    handle = rvl.handle;
    isImported = rvl.isImported;

    rvl.handle = gapi::BufferHandler::Invalid;
    return *this;
  }

  BufferResource::~BufferResource()
  {
    if (!isImported && handle != gapi::BufferHandler::Invalid)
    {
      gapi::free_resource(handle);
      handle = gapi::BufferHandler::Invalid;
    }
  }
}
