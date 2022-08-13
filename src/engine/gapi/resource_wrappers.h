#pragma once

#include "gapi.h"

namespace gapi
{
  class TextureWrapper
  {
    public:
      TextureWrapper(const TextureHandler h);
      ~TextureWrapper();

      inline operator TextureHandler() const { return m_Handle; }

    private:
      gapi::TextureHandler m_Handle;
  };

  class BufferWrapper
  {
    public:
      BufferWrapper(const BufferHandler h);
      ~BufferWrapper();

      inline operator BufferHandler() const { return m_Handle; }

    private:
      gapi::BufferHandler m_Handle;
  };
}
