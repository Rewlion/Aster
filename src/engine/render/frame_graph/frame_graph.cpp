#include "frame_graph.h"
#include "manager.h"

#include <engine/assert.h>

#include <EASTL/vector.h>

namespace fg
{
  static Manager* manager = nullptr;
  auto get_manager() -> Manager*
  {
    return manager;
  }

  void init()
  {
    ASSERT(manager == nullptr);
    manager = new Manager;
  }

  void destroy()
  {
    ASSERT(manager != nullptr);
    delete manager;
  }

  void register_node(const char* name, const char* file, BuildFunction build_cb)
  {
    manager->registerNode(name, file, build_cb);
  }

  void exec_new_frame()
  {
    manager->execNewFrame();
  }

  void set_closing_node(const char* node)
  {
    manager->setClosingNode(node);
  }

  void set_render_size(const uint2 size)
  {
    manager->setRenderSize(size);
  }

  auto get_cur_frame_texture(const char* name) -> gapi::TextureViewWithState*
  {
    return manager->getCurFrameTexture(name);
  }
}
