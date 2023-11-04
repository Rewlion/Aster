#include "file_watch.h"

#include <engine/assert.h>
#include <engine/log.h>
#include <engine/utils/fs.h>

#include <windows.h>
#include <fileapi.h>
#include <filesystem>
#include <handleapi.h>

namespace fs
{
  FilesWatcher* FilesWatcher::m_This = nullptr;

  void FilesWatcher::init()
  {
    ASSERT_FMT_RETURN(m_This == nullptr, , "FilesWatcher double init");
    m_This = new FilesWatcher;
  }

  void FilesWatcher::shutdown()
  {
    ASSERT_FMT(m_This != nullptr, "FilesWatcher is not initialized");
    delete m_This;
    m_This = nullptr;
  }

  void FilesWatcher::watchFileWrites(string_view filePath, Callback&& cb)
  {
    static_assert(sizeof(HANDLE) == sizeof(FilesWatcher::watch_handle_t));

    auto fileAbsPath = std::filesystem::absolute(filePath);
    string dir = fileAbsPath.parent_path().string();
    const name_id_t id = m_This->m_WatchedDirs.storeString(dir.data(), dir.size());

    DirWatch& watch = m_This->m_Watches[id];

    const auto fileWatchExist = eastl::find_if(watch.fileWatches.begin(), watch.fileWatches.end(),
      [filePath](const auto& v){
        return v.path == filePath;
      }) != watch.fileWatches.end();

    if (fileWatchExist)
    {
      logerror("FilesWatcher: watch for {} already registered", filePath);
      return;
    }

    if (watch.handle == nullptr)
    {
      HANDLE hndl = FindFirstChangeNotification(dir.data(), false, FILE_NOTIFY_CHANGE_LAST_WRITE);
      if (hndl == INVALID_HANDLE_VALUE || hndl == NULL)
      {
        logerror("FilesWatcher: failed to start watching for dir {} : {}", dir, GetLastError());
        return;
      }

      watch.handle = hndl;
    }

    watch.fileWatches.push_back({string{fileAbsPath.string()}, std::move(cb)});
  }

  void FilesWatcher::readNotifies()
  {
    const int watchesCount = m_This->m_Watches.size();
    for (int i = 0; i < watchesCount; ++i)
    {
      const name_id_t id{i};
      const auto& dirWatch = m_This->m_Watches[id];

      const HANDLE hndl = dirWatch.handle;
      DWORD dwWaitStatus;
      dwWaitStatus = WaitForMultipleObjects(1, &hndl, false, 0);
      if (dwWaitStatus == WAIT_OBJECT_0)
      {
        for (const auto& fileWatch: dirWatch.fileWatches)
          fileWatch.cb(fileWatch.path);

        if (FindNextChangeNotification(hndl) == FALSE)
          logerror("FilesWatcher: failed to continue listening notifications for {}", m_This->m_WatchedDirs.getString(id));
      }
    }
  }
}
