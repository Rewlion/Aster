#include "resource_wrappers.h"

namespace gapi
{
  TextureWrapper::TextureWrapper(const TextureHandler h)
  : m_Handle(h)
  {
  }

  TextureWrapper::~TextureWrapper()
  {
    if (m_Handle != TextureHandler::Invalid)
    {
      free_resource(m_Handle);
      m_Handle = TextureHandler::Invalid;
    }
  }

  BufferWrapper::BufferWrapper(const BufferHandler h)
  : m_Handle(h)
  {
  }

  BufferWrapper::~BufferWrapper()
  {
    if (m_Handle != BufferHandler::Invalid)
    {
      gapi::free_resource(m_Handle);
      m_Handle = BufferHandler::Invalid;
    }
  }
}
