#pragma once

#include <engine/gapi/gapi.h>
#include <engine/gapi/resources.h>

#include <EASTL/vector.h>
#include <EASTL/functional.h>

#include <variant>

namespace Engine::Render
{
  class FrameGC
  {
    public:
      using Resource = std::variant<
        gapi::BufferHandler,
        gapi::TextureHandle>;

    public:
      template<class T>
      auto allocate(T constr) -> decltype(constr())
      {
        auto resourceHandle = constr();
        m_Resources[frame].push_back(resourceHandle);
        return resourceHandle;
      }

      void nextFrame()
      {
        frame = (frame + 1) % 2;
        auto& resources = m_Resources[frame];
        for(auto& resource: resources)
          std::visit([](const auto& c) {
            gapi::free_resource(c);
          }, resource);
        resources.clear();
      }

    private:
    size_t frame = 0;
    eastl::vector<Resource> m_Resources[2];
  };
}
