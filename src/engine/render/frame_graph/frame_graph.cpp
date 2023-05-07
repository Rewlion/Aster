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

  auto register_node(const char* name, const char* file, BuildFunction build_cb) -> NodeHandle
  {
    return manager->registerNode(name, file, build_cb);
  }

  void exec_new_frame()
  {
    manager->execNewFrame();
  }

  void set_closing_node(const char* node)
  {
    manager->setClosingNode(node);
  }
}
