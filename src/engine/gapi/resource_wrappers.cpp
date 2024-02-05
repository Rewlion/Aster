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
    : m_Handle(TextureHandle::Invalid)
    , m_State(TextureState::Undefined)
  {
  }

  TextureViewWithState::TextureViewWithState(const TextureHandle h,
                                             const TextureState st)
    : m_Handle(h)
    , m_State(st)
  {
  }

  TextureViewWithState::TextureViewWithState(TextureViewWithState&& rvl)
    : m_Handle(rvl.m_Handle)
    , m_State(rvl.m_State)
  {
    rvl.m_Handle = TextureHandle::Invalid;
    rvl.m_State = TextureState::Undefined;
  }

  auto TextureViewWithState::operator=(TextureViewWithState&& rvl)
    -> TextureViewWithState&
  {
    std::swap(m_Handle, rvl.m_Handle);
    std::swap(m_State, rvl.m_State);
    return *this;
  }

  void TextureViewWithState::transitState(gapi::CmdEncoder& encoder,
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

  DECLARE_ECS_COMPONENT(TextureWrapper, "texture", (ecs::HandlerTypeManager<TextureWrapper>), false, true);
}
