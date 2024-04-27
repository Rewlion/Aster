#include "resource_wrappers.h"
#include "cmd_encoder.h"

#include <engine/ecs/type_meta.h>

namespace gapi
{
  TextureWrapper::TextureWrapper()
    : m_Handle(TextureHandle::Invalid)
  {
  }

  TextureWrapper::TextureWrapper(const TextureHandle h)
    : m_Handle(h)
  {
  }

  TextureWrapper::TextureWrapper(TextureWrapper&& rvl)
    : m_Handle(rvl.m_Handle)
  {
    rvl.m_Handle = TextureHandle::Invalid;
  }

  TextureWrapper::~TextureWrapper()
  {
    if (m_Handle != TextureHandle::Invalid)
    {
      free_resource(m_Handle);
      m_Handle = TextureHandle::Invalid;
    }
  }

  TextureWrapper& TextureWrapper::operator=(TextureWrapper&& rvl)
  {
    this->~TextureWrapper();
    std::swap(m_Handle, rvl.m_Handle);
    return *this;
  }

  TextureViewWithState::TextureViewWithState()
    : UniqueTextureWithState()
  {
  }

  TextureViewWithState::TextureViewWithState(const TextureHandle h,
                                             const TextureState st)
    : UniqueTextureWithState(h,st)
  {
  }

  TextureViewWithState::~TextureViewWithState()
  {
    m_Handle = gapi::TextureHandle::Invalid;
  }

  UniqueTextureWithState::UniqueTextureWithState()
    : m_Handle(gapi::TextureHandle::Invalid)
    , m_State(gapi::TextureState::Undefined)
  {
  }

  UniqueTextureWithState::UniqueTextureWithState(const TextureHandle h,
                                                 const gapi::TextureState state)
    : m_Handle(h)
    , m_State(state)
  {
  }

  UniqueTextureWithState::UniqueTextureWithState(UniqueTextureWithState&& r)
  {
    this->~UniqueTextureWithState();
    std::swap(m_Handle, r.m_Handle);
    std::swap(m_State, r.m_State);
  }

  UniqueTextureWithState::~UniqueTextureWithState()
  {
    if (m_Handle != gapi::TextureHandle::Invalid)
    {
      gapi::free_resource(m_Handle);
      m_Handle = gapi::TextureHandle::Invalid;
    }
  }

  auto UniqueTextureWithState::operator=(UniqueTextureWithState&& r) -> UniqueTextureWithState&
  {
    this->~UniqueTextureWithState();
    std::swap(m_Handle, r.m_Handle);
    std::swap(m_State, r.m_State);

    return *this;
  }

  void UniqueTextureWithState::transitState(gapi::CmdEncoder& encoder,
                                            const gapi::TextureState to_state)
  {
    encoder.transitTextureState(m_Handle, m_State, to_state);
    m_State = to_state;
  }

  BufferWrapper::BufferWrapper()
    : m_Handle(BufferHandler::Invalid)
  {
  }

  BufferWrapper::BufferWrapper(const BufferHandler h)
    : m_Handle(h)
  {
  }

  BufferWrapper::BufferWrapper(BufferWrapper&& rvl)
    : m_Handle(rvl.m_Handle)
  {
    rvl.m_Handle = BufferHandler::Invalid;
  }


  BufferWrapper::~BufferWrapper()
  {
    if (m_Handle != BufferHandler::Invalid)
    {
      gapi::free_resource(m_Handle);
      m_Handle = BufferHandler::Invalid;
    }
  }

  BufferWrapper& BufferWrapper::operator=(BufferWrapper&& rvl)
  {
    this->~BufferWrapper();
    std::swap(m_Handle, rvl.m_Handle);
    return *this;
  }

  UniqueBufferWithState::UniqueBufferWithState()
    : m_Handle(BufferHandler::Invalid)
    , m_State(BufferState::BF_STATE_SRV)
  {
  }

  UniqueBufferWithState::UniqueBufferWithState(const BufferHandler h,
                                               const BufferState st)
    : m_Handle(h)
    , m_State(st)
  {
  }

  UniqueBufferWithState::UniqueBufferWithState(UniqueBufferWithState&& rvl)
    : m_Handle(rvl.m_Handle)
    , m_State(rvl.m_State)
  {
    this->~UniqueBufferWithState();
    std::swap(m_Handle, rvl.m_Handle);
    std::swap(m_State, rvl.m_State);
  }

  UniqueBufferWithState::~UniqueBufferWithState()
  {
    if (m_Handle != BufferHandler::Invalid)
    {
      gapi::free_resource(m_Handle);
      m_Handle = BufferHandler::Invalid;
    }
  }

  auto UniqueBufferWithState::operator=(UniqueBufferWithState&& rvl)
    -> UniqueBufferWithState&
  {
    std::swap(m_Handle, rvl.m_Handle);
    std::swap(m_State, rvl.m_State);
    return *this;
  }

  void UniqueBufferWithState::transitState(gapi::CmdEncoder& encoder,
                                          const gapi::BufferState to_state)
  {
    encoder.insertGlobalBufferBarrier(m_State, to_state);
    m_State = to_state;
  }

  BufferViewWithState::BufferViewWithState()
    : UniqueBufferWithState()
  {
  }

  BufferViewWithState::BufferViewWithState(const BufferViewWithState& r)
    : UniqueBufferWithState(r.m_Handle, r.m_State)
  {
  }

  BufferViewWithState::BufferViewWithState(const gapi::BufferHandler h,
                                           const gapi::BufferState s)
    : UniqueBufferWithState(h, s)
  {
  }

  BufferViewWithState::~BufferViewWithState()
  {
    m_Handle = gapi::BufferHandler::Invalid;
  }

  auto BufferViewWithState::operator=(const BufferViewWithState& r) -> BufferViewWithState&
  {
    m_Handle = r.m_Handle;
    m_State = r.m_State;
    return *this;
  }

  ScopedScissor::ScopedScissor(const Scissor sc, CmdEncoder& encoder)
    : m_Encoder(encoder)
  {
    m_Scissor = encoder.getScissor();
    encoder.setScissor(sc);
  }

  ScopedScissor::~ScopedScissor()
  {
    m_Encoder.setScissor(m_Scissor);
  }

  DECLARE_ECS_COMPONENT_NO_ED(TextureWrapper, "texture", (ecs::HandlerTypeManager<TextureWrapper>), false, true);
}
