#pragma once

#include "gapi.h"

namespace gapi
{
  class TextureWrapper
  {
    public:
      TextureWrapper(const TextureHandler h);
      TextureWrapper(TextureWrapper&& rvl);
      ~TextureWrapper();

      const TextureWrapper& operator=(const TextureWrapper&) = delete;
      TextureWrapper& operator=(TextureWrapper&&);
      inline operator TextureHandler() const { return m_Handle; }

    private:
      gapi::TextureHandler m_Handle;
  };

  class BufferWrapper
  {
    public:
      BufferWrapper(const BufferHandler h);
      BufferWrapper(BufferWrapper&& h);
      ~BufferWrapper();

      const BufferWrapper& operator=(const BufferWrapper&) = delete;
      BufferWrapper& operator=(BufferWrapper&&);
      inline operator BufferHandler() const { return m_Handle; }

    private:
      gapi::BufferHandler m_Handle;
  };
}
