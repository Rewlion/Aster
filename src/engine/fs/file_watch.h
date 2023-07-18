#pragma once

#include <engine/types.h>
#include <engine/utils/resizable_vector.hpp>
#include <engine/utils/string_map.h>

#include <EASTL/vector_map.h>
#include <function2/function2.hpp>

namespace fs
{
  class FilesWatcher
  {
  public:
    using Callback = fu2::unique_function<void(string_view) const>;

    static
    void init();
    
    static
    void shutdown();

    static
    void watchFileWrites(string_view filePath, Callback&& cb);

    static
    void readNotifies();

  private:
    static FilesWatcher* m_This;

    using watch_handle_t = void*;
    struct FileWatch
    {
      string path;
      Callback cb;
    };

    struct DirWatch
    {
      watch_handle_t handle = nullptr;
      eastl::vector<FileWatch> fileWatches;
    };

    Utils::StringMap m_WatchedDirs;
    Utils::ResizableVector<DirWatch, name_id_t> m_Watches;
  };
}
