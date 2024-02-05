#pragma once

#include "gapi.h"

#include <EASTL/utility.h>

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

  class TextureViewWithState
  {
    public:
      TextureViewWithState();
      TextureViewWithState(const TextureViewWithState&) = default;
      TextureViewWithState(const TextureHandle h, gapi::TextureState m_State);
      TextureViewWithState(TextureViewWithState&& rvl);

      auto operator=(const TextureViewWithState&) -> TextureViewWithState& = default;
      auto operator=(TextureViewWithState&&) -> TextureViewWithState&;
      operator TextureHandle() const { return m_Handle; }

      auto getState() const -> gapi::TextureState { return m_State; }
      void transitState(gapi::CmdEncoder& encoder, const gapi::TextureState to_state);

      auto unwrap() const
        -> eastl::pair<gapi::TextureHandle, gapi::TextureState>
      {
        return {m_Handle, m_State};
      }

    private:
      gapi::TextureHandle m_Handle;
      gapi::TextureState m_State;
  };

  class BufferWrapper
  {
    public:
      BufferWrapper();
      BufferWrapper(const BufferHandler h);
      BufferWrapper(BufferWrapper&& h);
      ~BufferWrapper();

      auto operator=(const BufferWrapper&) -> const BufferWrapper& = delete;
      auto operator=(BufferWrapper&&) -> BufferWrapper&;
      operator BufferHandler() const { return m_Handle; }

      auto valid() const -> bool { return m_Handle != gapi::BufferHandler::Invalid; }
      void reset(const gapi::BufferHandler h = gapi::BufferHandler::Invalid) { this->~BufferWrapper(); m_Handle = h; }

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
