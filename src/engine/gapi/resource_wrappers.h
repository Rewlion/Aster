#pragma once

#include "gapi.h"

namespace gapi
{
  class TextureWrapper
  {
    public:
      TextureWrapper();
      TextureWrapper(const TextureHandle h);
      TextureWrapper(TextureWrapper&& rvl);
      ~TextureWrapper();

      const TextureWrapper& operator=(const TextureWrapper&) = delete;
      TextureWrapper& operator=(TextureWrapper&&);
      inline operator TextureHandle() const { return m_Handle; }

    private:
      gapi::TextureHandle m_Handle;
  };

  class BufferWrapper
  {
    public:
      BufferWrapper();
      BufferWrapper(const BufferHandler h);
      BufferWrapper(BufferWrapper&& h);
      ~BufferWrapper();

      const BufferWrapper& operator=(const BufferWrapper&) = delete;
      BufferWrapper& operator=(BufferWrapper&&);
      inline operator BufferHandler() const { return m_Handle; }

      auto valid() const -> bool { return m_Handle != gapi::BufferHandler::Invalid; }

    private:
      gapi::BufferHandler m_Handle;
  };

  class ScopedScissor
  {
    public:
      ScopedScissor(const Scissor, CmdEncoder&);
      ~ScopedScissor();
    private:
      Scissor m_Scissor;
      CmdEncoder& m_Encoder;
  };
}
