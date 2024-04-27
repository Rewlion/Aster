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

  class UniqueTextureWithState
  {
    public:
      UniqueTextureWithState();
      UniqueTextureWithState(const UniqueTextureWithState&) = default;
      UniqueTextureWithState(const TextureHandle, const gapi::TextureState);
      UniqueTextureWithState(UniqueTextureWithState&& rvl);

      ~UniqueTextureWithState();

      auto operator=(const UniqueTextureWithState&) -> UniqueTextureWithState& = default;
      auto operator=(UniqueTextureWithState&&) -> UniqueTextureWithState&;
      operator TextureHandle() const { return m_Handle; }

      auto getState() const -> gapi::TextureState { return m_State; }
      void transitState(gapi::CmdEncoder& encoder, const gapi::TextureState to_state);

      auto unwrap() const
        -> eastl::pair<gapi::TextureHandle, gapi::TextureState>
      {
        return {m_Handle, m_State};
      }

    protected:
      gapi::TextureHandle m_Handle;
      gapi::TextureState m_State;
  };

  class TextureViewWithState : public UniqueTextureWithState
  {
    public:
      TextureViewWithState();
      TextureViewWithState(const TextureViewWithState&) = default;
      TextureViewWithState(const TextureHandle h, gapi::TextureState m_State);

      ~TextureViewWithState();

      auto operator=(const TextureViewWithState&) -> TextureViewWithState& = default;
      operator TextureHandle() const { return m_Handle; }
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

  class UniqueBufferWithState
  {
    public:
      UniqueBufferWithState();
      UniqueBufferWithState(const UniqueBufferWithState&) = default;
      UniqueBufferWithState(const gapi::BufferHandler, const gapi::BufferState);
      UniqueBufferWithState(UniqueBufferWithState&& rvl);

      ~UniqueBufferWithState();

      auto operator=(const UniqueBufferWithState&) -> UniqueBufferWithState& = delete;
      auto operator=(UniqueBufferWithState&&) -> UniqueBufferWithState&;
      operator gapi::BufferHandler() const { return m_Handle; }

      auto getState() const -> gapi::BufferState { return m_State; }
      void transitState(gapi::CmdEncoder& encoder, const gapi::BufferState to_state);

      auto unwrap() const
        -> eastl::pair<gapi::BufferHandler, gapi::BufferState>
      {
        return {m_Handle, m_State};
      }

    protected:
      gapi::BufferHandler m_Handle;
      gapi::BufferState m_State;
  };

  class BufferViewWithState : public UniqueBufferWithState
  {
    public:
      BufferViewWithState();
      BufferViewWithState(const BufferViewWithState&);
      BufferViewWithState(const gapi::BufferHandler, const gapi::BufferState);

      ~BufferViewWithState();

      auto operator=(const BufferViewWithState&) -> BufferViewWithState&;
      operator gapi::BufferHandler() const { return m_Handle; }
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
